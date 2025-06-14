//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_STATE_MACHINE_H
#define OMNISERVER_STATE_MACHINE_H

#include <QObject>
#include <QString>

enum class AppState {
    Startup,
    SetupWizard,
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


#endif //OMNISERVER_STATE_MACHINE_H
