#include "state_machine.h"

StateMachine::StateMachine(QObject *parent)
    : QObject(parent)
{

}

AppState StateMachine::getCurrentState() const noexcept
{
    return currentState;
}

void StateMachine::transition(AppState newState)
{
    switch (currentState) {
    case AppState::Startup:
        if (newState == AppState::NormalRun
            || newState == AppState::RecoveryMode) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::NormalRun:
        if (newState == AppState::RecoveryMode) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::RecoveryMode:
        if (newState == AppState::NormalRun) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    }

    throw std::runtime_error("Illegal state transition");
}
