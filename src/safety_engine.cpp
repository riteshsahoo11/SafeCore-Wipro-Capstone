#include "safety_engine.hpp"

namespace safecore {

SafetyResult SafetyEngine::evaluate(
    const safecore_sensor_data& data) const
{
    // A reading from an invalid sensor must not be trusted.
    if (data.sensor_valid != 1) {
        return {
            SafetyCondition::CRITICAL,
            "Sensor data is invalid"
        };
    }

    // Critical conditions are checked first.
    if (data.temperature_c > 85) {
        return {
            SafetyCondition::CRITICAL,
            "Temperature is above the critical limit"
        };
    }

    if (data.vibration > 7) {
        return {
            SafetyCondition::CRITICAL,
            "Vibration is above the critical limit"
        };
    }

    if (data.motor_rpm > 1800) {
        return {
            SafetyCondition::CRITICAL,
            "Motor speed is above the critical limit"
        };
    }

    // Warning conditions.
    if (data.temperature_c >= 70) {
        return {
            SafetyCondition::WARNING,
            "Temperature is in the warning range"
        };
    }

    if (data.vibration >= 4) {
        return {
            SafetyCondition::WARNING,
            "Vibration is in the warning range"
        };
    }

    if (data.motor_rpm >= 1500) {
        return {
            SafetyCondition::WARNING,
            "Motor speed is in the warning range"
        };
    }

    return {
        SafetyCondition::NORMAL,
        "All readings are within normal limits"
    };
}

const char* SafetyEngine::conditionToString(
    SafetyCondition condition)
{
    switch (condition) {

        case SafetyCondition::NORMAL:
            return "NORMAL";

        case SafetyCondition::WARNING:
            return "WARNING";

        case SafetyCondition::CRITICAL:
            return "CRITICAL";
    }

    return "UNKNOWN";
}

}
