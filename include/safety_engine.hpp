#ifndef SAFECORE_SAFETY_ENGINE_HPP
#define SAFECORE_SAFETY_ENGINE_HPP

#include <string>

#include "safecore_ioctl.h"

namespace safecore {

enum class SafetyState {
    NORMAL,
    WARNING,
    CRITICAL
};

struct SafetyResult {
    SafetyState state;
    std::string reason;
};

class SafetyEngine {
public:
    SafetyResult evaluate(const safecore_sensor_data& data) const;

    static const char* stateToString(SafetyState state);
};

}

#endif
