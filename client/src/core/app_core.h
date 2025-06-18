#ifndef OMNICLIENT_APP_CORE_H
#define OMNICLIENT_APP_CORE_H

#include <QObject>

#include "state_machine.h"
#include "client_core.h"
#include "net/port_authority.h"
#include "net/fedex.h"
#include "mailbox.h"
#include "db/omni_network.h"
#include "gui/main_window.h"
#include "gui/login_screen.h"

class AppCore : public QObject {
    Q_OBJECT

public:
    explicit AppCore(QObject* parent = nullptr);

    // Called from main.cpp to start up the app
    void boot();

private slots:
    void onStateTransition(AppState updatedState);

    void onLoginScreenPassed();
    void onQuitRequested();

private:
    StateMachine stateMachine;

    std::unique_ptr<ClientCore> clientCore = nullptr;
    std::unique_ptr<PortAuthority> portAuthority = nullptr;
    std::unique_ptr<FedEx> fedEx = nullptr;
    std::unique_ptr<Mailbox> mailbox = nullptr;
    std::unique_ptr<OmniNetwork> omniNetwork = nullptr;

    std::unique_ptr<LoginScreen> loginScreen = nullptr;
    std::unique_ptr<MainWindow> mainWindow = nullptr;
};

#endif //OMNICLIENT_APP_CORE_H
