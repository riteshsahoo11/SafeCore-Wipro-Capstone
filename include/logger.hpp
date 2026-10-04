#ifndef SAFECORE_LOGGER_HPP
#define SAFECORE_LOGGER_HPP

#include <string>

#include "safety_engine.hpp"
#include "state_manager.hpp"

namespace safecore {

class Logger {
public:
    bool log(
        MachineState state,
        SafetyCondition condition,
        const std::string& message) const;
};

}

#endif
