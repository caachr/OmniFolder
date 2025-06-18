#include "app_core.h"
#include "gui/login_screen.h"

AppCore::AppCore(QObject* parent)
    : QObject(parent)
{
    // Set state transition connection
    connect(&stateMachine, &StateMachine::stateChanged, this, &AppCore::onStateTransition);
}

void AppCore::boot()
{
    // nothing to do here at the moment...
    stateMachine.transition(AppState::Login);
}

void AppCore::onStateTransition(AppState updatedState)
{
    switch (updatedState) {
    case AppState::Startup:
        throw std::runtime_error("Should be impossible to transition to startup state from another, but it happened for some reason");
        break;

    case AppState::Login:
        // Objects construction
        loginScreen = std::make_unique<LoginScreen>();
        clientCore = std::make_unique<ClientCore>(this);
        fedEx = std::make_unique<FedEx>(this);
        portAuthority = std::make_unique<PortAuthority>(this);

        // Set local pointers to unique objects
        clientCore->setFedEx(fedEx.get());
        clientCore->setPortAuthority(portAuthority.get());
        fedEx->setPortAuthority(portAuthority.get());
        portAuthority->setClientCore(clientCore.get());
        portAuthority->setFedEx(fedEx.get());

        // Set connections
        connect(loginScreen.get(), &QDialog::accepted, this, &AppCore::onLoginScreenPassed);
        connect(loginScreen.get(), &LoginScreen::loginRequested, portAuthority.get(), &PortAuthority::onLoginRequested);
        connect(fedEx.get(), &FedEx::messageReady, portAuthority.get(), &PortAuthority::onLoginReplyReady);
        connect(portAuthority.get(), &PortAuthority::loginSuccess, loginScreen.get(), &LoginScreen::onLoginSuccess);
        connect(portAuthority.get(), &PortAuthority::loginFailure, loginScreen.get(), &LoginScreen::onLoginFailure);

        // Display
        loginScreen->show();
        break;

    case AppState::NormalRun:
        break;

    case AppState::Frozen:
        break;

    case AppState::Away:
        break;

    case AppState::AwayFrozen:
        break;

    case AppState::BeaconMode:
        break;

    case AppState::Shutdown:
        // Clean up previous objects
        clientCore.reset();
        portAuthority.reset();
        fedEx.reset();
        mailbox.reset();
        omniNetwork.reset();
        loginScreen.reset();
        mainWindow.reset();

        clientCore = nullptr;
        portAuthority = nullptr;
        fedEx = nullptr;
        mailbox = nullptr;
        omniNetwork = nullptr;
        loginScreen = nullptr;
        mainWindow = nullptr;
        break;
    }
}

void AppCore::onLoginScreenPassed()
{
    // Clear existing connections & login objects
    disconnect(loginScreen.get(), &QDialog::accepted, this, &AppCore::onLoginScreenPassed);
    disconnect(loginScreen.get(), &LoginScreen::loginRequested, portAuthority.get(), &PortAuthority::onLoginRequested);
    disconnect(fedEx.get(), &FedEx::messageReady, portAuthority.get(), &PortAuthority::onLoginReplyReady);
    disconnect(portAuthority.get(), &PortAuthority::loginSuccess, loginScreen.get(), &LoginScreen::onLoginSuccess);
    disconnect(portAuthority.get(), &PortAuthority::loginFailure, loginScreen.get(), &LoginScreen::onLoginFailure);
    loginScreen->deleteLater();
    loginScreen = nullptr;

    // Remaining objects construction
    mailbox = std::make_unique<Mailbox>(this);
    omniNetwork = std::make_unique<OmniNetwork>(this);
    mainWindow = std::make_unique<MainWindow>();

    // Set local pointers to unique objects
    clientCore->setMailbox(mailbox.get());
    mainWindow->setNetwork(omniNetwork.get());

    // Client boot
    clientCore->boot();

    // GUI wiring
    connect(mainWindow.get(), &QWidget::destroyed, this, &AppCore::onQuitRequested);

    // Core systems wiring
    connect(fedEx.get(), &FedEx::messageReady, mailbox.get(), &Mailbox::onMessageReady);
    connect(mailbox.get(), &Mailbox::youveGotMail, clientCore.get(), &ClientCore::onYouveGotMail);

    // Load display
    mainWindow->show();

    // Transition
    stateMachine.transition(AppState::NormalRun);
}

void AppCore::onQuitRequested()
{
    stateMachine.transition(AppState::Shutdown);
}
