#include <iostream>

#include "safety_engine.hpp"
#include "state_manager.hpp"

int main()
{
    safecore::SafetyEngine engine;
    safecore::StateManager stateManager;

    int failures = 0;

    auto check = [&](bool condition,
                     const char* message)
    {
        if (condition) {
            std::cout
                << "[PASS] "
                << message
                << "\n";
        }
        else {
            std::cout
                << "[FAIL] "
                << message
                << "\n";

            ++failures;
        }
    };

    safecore_sensor_data normal{
        60, 1200, 2, 1
    };

    auto result =
        engine.evaluate(normal);

    check(
        result.condition ==
            safecore::SafetyCondition::NORMAL,
        "Normal values are classified as NORMAL"
    );

    stateManager.update(result);

    check(
        stateManager.currentState() ==
            safecore::MachineState::NORMAL,
        "Normal condition keeps machine in NORMAL"
    );

    safecore_sensor_data warning{
        75, 1200, 2, 1
    };

    result = engine.evaluate(warning);

    check(
        result.condition ==
            safecore::SafetyCondition::WARNING,
        "High temperature creates WARNING"
    );

    stateManager.update(result);

    check(
        stateManager.currentState() ==
            safecore::MachineState::WARNING,
        "Warning condition moves machine to WARNING"
    );

    safecore_sensor_data critical{
        95, 1200, 2, 1
    };

    result = engine.evaluate(critical);

    check(
        result.condition ==
            safecore::SafetyCondition::CRITICAL,
        "Critical temperature creates CRITICAL"
    );

    stateManager.update(result);

    check(
        stateManager.currentState() ==
            safecore::MachineState::SAFE_STATE,
        "Critical condition moves machine to SAFE_STATE"
    );

    stateManager.update(
        engine.evaluate(normal)
    );

    check(
        stateManager.currentState() ==
            safecore::MachineState::SAFE_READY,
        "Cleared fault moves machine to SAFE_READY"
    );

    check(
        stateManager.reset(),
        "Manual reset is accepted after safe recovery"
    );

    check(
        stateManager.currentState() ==
            safecore::MachineState::NORMAL,
        "Manual reset returns machine to NORMAL"
    );

    safecore_sensor_data faulty{
        60, 1200, 2, 0
    };

    result = engine.evaluate(faulty);

    check(
        result.condition ==
            safecore::SafetyCondition::CRITICAL,
        "Invalid sensor data is treated as a critical fault"
    );

    stateManager.update(result);

    check(
        stateManager.currentState() ==
            safecore::MachineState::SAFE_STATE,
        "Sensor failure moves machine to SAFE_STATE"
    );

    if (failures == 0) {
        std::cout
            << "\nAll Milestone 3 tests passed.\n";
        return 0;
    }

    std::cout
        << "\n"
        << failures
        << " test(s) failed.\n";

    return 1;
}
