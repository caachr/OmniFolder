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
        if (newState == AppState::Login) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::Login:
        if (newState == AppState::NormalRun || newState == AppState::Shutdown) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::NormalRun:
        if (newState == AppState::Frozen || newState == AppState::Away || newState == AppState::Shutdown) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::Frozen:
        if (newState == AppState::NormalRun || newState == AppState::Shutdown) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::Away:
        if (newState == AppState::AwayFrozen || newState == AppState::NormalRun || newState == AppState::Shutdown) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::AwayFrozen:
        if (newState == AppState::BeaconMode || newState == AppState::Away || newState == AppState::Shutdown) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::BeaconMode:
        if (newState == AppState::Away || newState == AppState::Shutdown) {
            currentState = newState;
            emit stateChanged(currentState);
            return;
        }
        break;
    case AppState::Shutdown:
        throw std::runtime_error("Attempted illegal transition from Shutdown state to something else");
        break;
    }
}
