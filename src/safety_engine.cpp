#include "safety_engine.hpp"

namespace safecore {

SafetyResult SafetyEngine::evaluate(
    const safecore_sensor_data& data) const
{
    // Critical conditions are checked first.
    if (data.temperature_c > 85) {
        return {
            SafetyState::CRITICAL,
            "Temperature is above the critical limit"
        };
    }

    if (data.vibration > 7) {
        return {
            SafetyState::CRITICAL,
            "Vibration is above the critical limit"
        };
    }

    if (data.motor_rpm > 1800) {
        return {
            SafetyState::CRITICAL,
            "Motor speed is above the critical limit"
        };
    }

    // Warning conditions are checked next.
    if (data.temperature_c >= 70) {
        return {
            SafetyState::WARNING,
            "Temperature is in the warning range"
        };
    }

    if (data.vibration >= 4) {
        return {
            SafetyState::WARNING,
            "Vibration is in the warning range"
        };
    }

    if (data.motor_rpm >= 1500) {
        return {
            SafetyState::WARNING,
            "Motor speed is in the warning range"
        };
    }

    return {
        SafetyState::NORMAL,
        "All readings are within normal limits"
    };
}

const char* SafetyEngine::stateToString(SafetyState state)
{
    switch (state) {

        case SafetyState::NORMAL:
            return "NORMAL";

        case SafetyState::WARNING:
            return "WARNING";

        case SafetyState::CRITICAL:
            return "CRITICAL";
    }

    return "UNKNOWN";
}

}
