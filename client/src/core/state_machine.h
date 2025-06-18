#ifndef OMNICLIENT_STATE_MACHINE_H
#define OMNICLIENT_STATE_MACHINE_H

#include <QObject>
#include <QString>

enum class AppState {
    Startup,    // Startup boot shit
    Login,      // Login page
    NormalRun,  // Normal mode: you're the one at the computer doing things, making edits, etc. all manually like usual
    Frozen,     // Server disconnection -> frozen (still interactable, but can't edit shit or request locks or anything, app stays as it is)
    Away,       // Situations like keeping a client running continuously at your house while you're away
    AwayFrozen, // When away client disconnects from server
    BeaconMode, // Activated when an Away client remains disconnected for more than 5 minutes (AwayFrozen for more than 5 mins)
    Shutdown    // Shutdown sequence
};

class StateMachine : public QObject {
    Q_OBJECT

public:
    explicit StateMachine(QObject *parent = nullptr);

    AppState getCurrentState() const noexcept;

    void transition(AppState newState);

signals:
    void stateChanged(AppState updatedState);

private:
    AppState currentState = AppState::Startup;
};


#endif //OMNICLIENT_STATE_MACHINE_H
