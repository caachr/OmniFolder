#ifndef OMNICLIENT_STATE_MACHINE_H
#define OMNICLIENT_STATE_MACHINE_H

#include <QObject>
#include <QString>

enum class AppState {
    Startup,
    NormalRun,
    RecoveryMode
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
