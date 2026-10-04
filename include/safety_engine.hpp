#ifndef SAFECORE_SAFETY_ENGINE_HPP
#define SAFECORE_SAFETY_ENGINE_HPP

#include <string>

#include "safecore_ioctl.h"

namespace safecore {

enum class SafetyCondition {
    NORMAL,
    WARNING,
    CRITICAL
};

struct SafetyResult {
    SafetyCondition condition;
    std::string reason;
};

class SafetyEngine {
public:
    SafetyResult evaluate(
        const safecore_sensor_data& data) const;

    static const char* conditionToString(
        SafetyCondition condition);
};

}

#endif
