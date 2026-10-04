#include "logger.hpp"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace safecore {

bool Logger::log(
    MachineState state,
    SafetyCondition condition,
    const std::string& message) const
{
    std::ofstream file(
        "logs/safecore.log",
        std::ios::app
    );

    if (!file) {
        std::cerr << "Failed to open log file\n";
        return false;
    }

    const auto now = std::time(nullptr);

    std::tm localTime{};

    localtime_r(&now, &localTime);

    file << "["
         << std::put_time(
                &localTime,
                "%Y-%m-%d %H:%M:%S")
         << "] ";

    file << "Condition="
         << SafetyEngine::conditionToString(condition);

    file << ", State="
         << StateManager::stateToString(state);

    file << ", Message="
         << message
         << '\n';

    return true;
}

}
