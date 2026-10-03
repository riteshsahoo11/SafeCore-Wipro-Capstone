#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

#include "safety_engine.hpp"

namespace {

constexpr const char* DEVICE_PATH = "/dev/safecore";

void printData(const safecore_sensor_data& data)
{
    std::cout << "Temperature : "
              << data.temperature_c << " C\n";

    std::cout << "Motor RPM   : "
              << data.motor_rpm << "\n";

    std::cout << "Vibration   : "
              << data.vibration << "\n";
}

bool setSensor(
    int fd,
    const safecore_sensor_data& data)
{
    if (ioctl(fd, SAFECORE_IOC_SET_SENSOR, &data) < 0) {
        std::cerr << "SET_SENSOR failed: "
                  << std::strerror(errno) << "\n";
        return false;
    }

    return true;
}

bool getSensor(
    int fd,
    safecore_sensor_data& data)
{
    if (ioctl(fd, SAFECORE_IOC_GET_SENSOR, &data) < 0) {
        std::cerr << "GET_SENSOR failed: "
                  << std::strerror(errno) << "\n";
        return false;
    }

    return true;
}

void showEvaluation(
    const safecore_sensor_data& data)
{
    safecore::SafetyEngine engine;

    const auto result = engine.evaluate(data);

    std::cout << "State       : "
              << safecore::SafetyEngine::stateToString(result.state)
              << "\n";

    std::cout << "Reason      : "
              << result.reason
              << "\n";
}

int runDemo(int fd)
{
    const safecore_sensor_data scenarios[] = {

        // Normal
        {60, 1200, 2, 1},

        // Temperature warning
        {75, 1200, 2, 1},

        // Critical temperature
        {95, 1200, 2, 1},

        // Motor speed warning
        {60, 1600, 2, 1},

        // Critical vibration
        {60, 1200, 8, 1}
    };

    const char* names[] = {
        "Normal condition",
        "Temperature warning",
        "Critical temperature",
        "Motor speed warning",
        "Critical vibration"
    };

    for (std::size_t i = 0; i < 5; ++i) {

        std::cout << "\n--- "
                  << names[i]
                  << " ---\n";

        if (!setSensor(fd, scenarios[i])) {
            return 1;
        }

        safecore_sensor_data current{};

        if (!getSensor(fd, current)) {
            return 1;
        }

        printData(current);

        showEvaluation(current);
    }

    return 0;
}

void printHelp()
{
    std::cout
        << "\nCommands:\n"
        << "  set <temperature> <rpm> <vibration>\n"
        << "  read\n"
        << "  status\n"
        << "  help\n"
        << "  exit\n";
}

int runInteractive(int fd)
{
    std::cout << "SafeCore interactive mode\n";

    printHelp();

    std::string line;

    while (std::cout << "\nsafecore> "
           && std::getline(std::cin, line)) {

        std::istringstream iss(line);

        std::string command;

        iss >> command;

        if (command == "exit" || command == "quit") {
            break;
        }

        if (command == "help") {
            printHelp();
            continue;
        }

        if (command == "read" ||
            command == "status") {

            safecore_sensor_data current{};

            if (!getSensor(fd, current)) {
                continue;
            }

            printData(current);

            if (command == "status") {
                showEvaluation(current);
            }

            continue;
        }

        if (command == "set") {

            safecore_sensor_data data{};

            data.sensor_valid = 1;

            if (!(iss >> data.temperature_c
                      >> data.motor_rpm
                      >> data.vibration)) {

                std::cout
                    << "Usage: set <temperature> "
                    "<rpm> <vibration>\n";

                continue;
            }

            if (!setSensor(fd, data)) {
                continue;
            }

            safecore_sensor_data current{};

            if (getSensor(fd, current)) {

                printData(current);

                showEvaluation(current);
            }

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
         std::strcmp(argv[1], "interactive") != 0)) {

        std::cout
            << "Usage: "
            << argv[0]
            << " <demo|interactive>\n";

        return 1;
    }

    int fd = open(DEVICE_PATH, O_RDWR);

    if (fd < 0) {

        std::cerr
            << "Failed to open "
            << DEVICE_PATH
            << ": "
            << std::strerror(errno)
            << "\n";

        return 1;
    }

    int result;

    if (std::strcmp(argv[1], "demo") == 0) {
        result = runDemo(fd);
    }
    else {
        result = runInteractive(fd);
    }

    close(fd);

    return result;
}
