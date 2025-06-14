//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_APP_CORE_H
#define OMNISERVER_APP_CORE_H

#include <QObject>
#include <QRegularExpression>
#include <QApplication>

#include "state_machine.h"
#include "config_manager.h"
#include "server_core.h"
#include "db/omni_network.h"
#include "gui/main_window.h"
#include "gui/setup_wizard.h"
#include "gui/recovery_wizard.h"
#include "gui/client_table_model.h"

class AppCore : public QObject {
    Q_OBJECT

public:
    explicit AppCore(QObject* parent = nullptr);

    // Called from main.cpp to start up the app
    void boot();

private slots:
    void onStateTransition(AppState updatedState);

    void onSetupWizardFinished();
    void onRecoveryWizardFinished();
    void onQuitRequested();

private:
    StateMachine stateMachine;

    std::unique_ptr<ServerCore> serverCore = nullptr;
    std::unique_ptr<PortAuthority> portAuthority = nullptr;
    std::unique_ptr<FedEx> fedEx = nullptr;
    std::unique_ptr<Mailbox> mailbox = nullptr;
    std::unique_ptr<OmniNetwork> omniNetwork = nullptr;

    ConfigManager configManager;

    MainWindow *mainWindow = nullptr;
    SetupWizard *setupWizard = nullptr;
    RecoveryWizard *recoveryWizard = nullptr;
};


#endif //OMNISERVER_APP_CORE_H
