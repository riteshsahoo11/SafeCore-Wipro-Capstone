#include "state_manager.hpp"

namespace safecore {

StateManager::StateManager()
    : state_(MachineState::NORMAL),
      lastCondition_(SafetyCondition::NORMAL)
{
}

void StateManager::update(const SafetyResult& result)
{
    lastCondition_ = result.condition;

    // Once the system has entered SAFE_STATE,
    // it does not directly return to NORMAL.
    if (state_ == MachineState::SAFE_STATE) {

        if (result.condition == SafetyCondition::CRITICAL) {
            return;
        }

        state_ = MachineState::SAFE_READY;
        return;
    }

    // SAFE_READY requires an explicit reset.
    if (state_ == MachineState::SAFE_READY) {
        return;
    }

    switch (result.condition) {

        case SafetyCondition::NORMAL:
            state_ = MachineState::NORMAL;
            break;

        case SafetyCondition::WARNING:
            state_ = MachineState::WARNING;
            break;

        case SafetyCondition::CRITICAL:
            state_ = MachineState::SAFE_STATE;
            break;
    }
}

bool StateManager::reset()
{
    // Reset is allowed only after entering SAFE_READY
    // and only when the latest condition is NORMAL.
    if (state_ == MachineState::SAFE_READY &&
        lastCondition_ == SafetyCondition::NORMAL) {

        state_ = MachineState::NORMAL;
        return true;
    }

    return false;
}

MachineState StateManager::currentState() const
{
    return state_;
}

const char* StateManager::stateToString(MachineState state)
{
    switch (state) {

        case MachineState::NORMAL:
            return "NORMAL";

        case MachineState::WARNING:
            return "WARNING";

        case MachineState::SAFE_STATE:
            return "SAFE_STATE";

        case MachineState::SAFE_READY:
            return "SAFE_READY";
    }

    return "UNKNOWN";
}

}
