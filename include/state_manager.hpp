#ifndef SAFECORE_STATE_MANAGER_HPP
#define SAFECORE_STATE_MANAGER_HPP

#include "safety_engine.hpp"

namespace safecore {

enum class MachineState {
    NORMAL,
    WARNING,
    SAFE_STATE,
    SAFE_READY
};

class StateManager {
private:
    MachineState state_;
    SafetyCondition lastCondition_;

public:
    StateManager();

    void update(const SafetyResult& result);

    bool reset();

    MachineState currentState() const;

    static const char* stateToString(MachineState state);
};

}

#endif
