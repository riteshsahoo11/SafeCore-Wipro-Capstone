#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

#include "logger.hpp"
#include "safety_engine.hpp"
#include "state_manager.hpp"

namespace {

constexpr const char* DEVICE_PATH = "/dev/safecore";

void printData(const safecore_sensor_data& data)
{
    std::cout
        << "Temperature : "
        << data.temperature_c
        << " C\n";

    std::cout
        << "Motor RPM   : "
        << data.motor_rpm
        << "\n";

    std::cout
        << "Vibration   : "
        << data.vibration
        << "\n";

    std::cout
        << "Sensor      : "
        << (data.sensor_valid ? "VALID" : "INVALID")
        << "\n";
}

bool setSensor(
    int fd,
    const safecore_sensor_data& data)
{
    if (ioctl(
            fd,
            SAFECORE_IOC_SET_SENSOR,
            &data) < 0) {

        std::cerr
            << "SET_SENSOR failed: "
            << std::strerror(errno)
            << "\n";

        return false;
    }

    return true;
}

bool getSensor(
    int fd,
    safecore_sensor_data& data)
{
    if (ioctl(
            fd,
            SAFECORE_IOC_GET_SENSOR,
            &data) < 0) {

        std::cerr
            << "GET_SENSOR failed: "
            << std::strerror(errno)
            << "\n";

        return false;
    }

    return true;
}

bool evaluateCurrentData(
    int fd,
    safecore::SafetyEngine& engine,
    safecore::StateManager& stateManager,
    safecore::Logger& logger)
{
    safecore_sensor_data data{};

    if (!getSensor(fd, data)) {
        return false;
    }

    const auto result = engine.evaluate(data);

    stateManager.update(result);

    const auto state =
        stateManager.currentState();

    printData(data);

    std::cout
        << "Condition   : "
        << safecore::SafetyEngine::conditionToString(
               result.condition)
        << "\n";

    std::cout
        << "Machine     : "
        << safecore::StateManager::stateToString(
               state)
        << "\n";

    std::cout
        << "Reason      : "
        << result.reason
        << "\n";

    logger.log(
        state,
        result.condition,
        result.reason
    );

    return true;
}

void printHelp()
{
    std::cout
        << "\nCommands:\n"
        << "  set <temperature> <rpm> <vibration>\n"
        << "  fault\n"
        << "  read\n"
        << "  status\n"
        << "  reset\n"
        << "  help\n"
        << "  exit\n";
}

int runDemo(
    int fd,
    safecore::SafetyEngine& engine,
    safecore::StateManager& stateManager,
    safecore::Logger& logger)
{
    std::cout
        << "\n===== SAFECORE DEMO =====\n";

    safecore_sensor_data normal{
        60, 1200, 2, 1
    };

    std::cout
        << "\n1. Normal operation\n";

    setSensor(fd, normal);

    evaluateCurrentData(
        fd,
        engine,
        stateManager,
        logger
    );

    safecore_sensor_data warning{
        75, 1200, 2, 1
    };

    std::cout
        << "\n2. Warning condition\n";

    setSensor(fd, warning);

    evaluateCurrentData(
        fd,
        engine,
        stateManager,
        logger
    );

    safecore_sensor_data critical{
        95, 1200, 2, 1
    };

    std::cout
        << "\n3. Critical condition\n";

    setSensor(fd, critical);

    evaluateCurrentData(
        fd,
        engine,
        stateManager,
        logger
    );

    std::cout
        << "\n4. Dangerous condition cleared\n";

    setSensor(fd, normal);

    evaluateCurrentData(
        fd,
        engine,
        stateManager,
        logger
    );

    std::cout
        << "\n5. Manual reset\n";

    if (stateManager.reset()) {
        std::cout
            << "System reset successful.\n";
    }
    else {
        std::cout
            << "Reset rejected. "
            << "System is not ready for reset.\n";
    }

    std::cout
        << "Machine     : "
        << safecore::StateManager::stateToString(
               stateManager.currentState())
        << "\n";

    safecore_sensor_data faulty{
        60, 1200, 2, 0
    };

    std::cout
        << "\n6. Sensor failure\n";

    setSensor(fd, faulty);

    evaluateCurrentData(
        fd,
        engine,
        stateManager,
        logger
    );

    std::cout
        << "\n7. Sensor recovered\n";

    setSensor(fd, normal);

    evaluateCurrentData(
        fd,
        engine,
        stateManager,
        logger
    );

    std::cout
        << "\n8. Manual reset after recovery\n";

    if (stateManager.reset()) {
        std::cout
            << "System reset successful.\n";
    }
    else {
        std::cout
            << "Reset rejected.\n";
    }

    std::cout
        << "Machine     : "
        << safecore::StateManager::stateToString(
               stateManager.currentState())
        << "\n";

    return 0;
}

int runInteractive(
    int fd,
    safecore::SafetyEngine& engine,
    safecore::StateManager& stateManager,
    safecore::Logger& logger)
{
    std::cout
        << "SafeCore interactive mode\n";

    printHelp();

    std::string line;

    while (
        std::cout << "\nsafecore> " &&
        std::getline(std::cin, line))
    {
        std::istringstream iss(line);

        std::string command;

        iss >> command;

        if (command == "exit" ||
            command == "quit") {

            break;
        }

        if (command == "help") {

            printHelp();
            continue;
        }

        if (command == "read") {

            safecore_sensor_data data{};

            if (getSensor(fd, data)) {
                printData(data);
            }

            continue;
        }

        if (command == "status") {

            evaluateCurrentData(
                fd,
                engine,
                stateManager,
                logger
            );

            continue;
        }

        if (command == "set") {

            safecore_sensor_data data{};

            data.sensor_valid = 1;

            if (!(iss >>
                  data.temperature_c >>
                  data.motor_rpm >>
                  data.vibration))
            {
                std::cout
                    << "Usage: set "
                    << "<temperature> "
                    << "<rpm> "
                    << "<vibration>\n";

                continue;
            }

            if (setSensor(fd, data)) {

                evaluateCurrentData(
                    fd,
                    engine,
                    stateManager,
                    logger
                );
            }

            continue;
        }

        if (command == "fault") {

            safecore_sensor_data data{};

            if (!getSensor(fd, data)) {
                continue;
            }

            data.sensor_valid = 0;

            if (setSensor(fd, data)) {

                evaluateCurrentData(
                    fd,
                    engine,
                    stateManager,
                    logger
                );
            }

            continue;
        }

        if (command == "reset") {

            if (stateManager.reset()) {

                std::cout
                    << "Manual reset successful.\n";

                logger.log(
                    stateManager.currentState(),
                    safecore::SafetyCondition::NORMAL,
                    "Manual reset completed"
                );
            }
            else {

                std::cout
                    << "Reset rejected. "
                    << "The system must be in SAFE_READY "
                    << "with safe sensor conditions.\n";
            }

            std::cout
                << "Machine     : "
                << safecore::StateManager::stateToString(
                       stateManager.currentState())
                << "\n";

            continue;
        }

        if (!command.empty()) {

            std::cout
                << "Unknown command. "
                << "Type help.\n";
        }
    }

    return 0;
}

}

int main(int argc, char* argv[])
{
    if (argc != 2 ||
        (std::strcmp(argv[1], "demo") != 0 &&
         std::strcmp(argv[1], "interactive") != 0))
    {
        std::cout
            << "Usage: "
            << argv[0]
            << " <demo|interactive>\n";

        return 1;
    }

    int fd = open(
        DEVICE_PATH,
        O_RDWR
    );

    if (fd < 0) {

        std::cerr
            << "Failed to open "
            << DEVICE_PATH
            << ": "
            << std::strerror(errno)
            << "\n";

        return 1;
    }

    safecore::SafetyEngine engine;
    safecore::StateManager stateManager;
    safecore::Logger logger;

    int result;

    if (std::strcmp(
            argv[1],
            "demo") == 0)
    {
        result = runDemo(
            fd,
            engine,
            stateManager,
            logger
        );
    }
    else
    {
        result = runInteractive(
            fd,
            engine,
            stateManager,
            logger
        );
    }

    close(fd);

    return result;
}
