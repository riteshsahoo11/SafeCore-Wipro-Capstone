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

    // Once the system enters SAFE_STATE,
    // it must not immediately return to normal.
    if (state_ == MachineState::SAFE_STATE) {

        if (result.condition == SafetyCondition::NORMAL) {
            state_ = MachineState::SAFE_READY;
        }
        else {
            state_ = MachineState::SAFE_STATE;
        }

        return;
    }

    // SAFE_READY means the dangerous condition has
    // been cleared, but manual reset is still required.
    if (state_ == MachineState::SAFE_READY) {

        if (result.condition == SafetyCondition::NORMAL) {
            state_ = MachineState::SAFE_READY;
        }
        else {
            state_ = MachineState::SAFE_STATE;
        }

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
