//
// Created by Christian Caamano on 6/6/25.
//

#include "app_core.h"

AppCore::AppCore(QObject *parent)
    : QObject(parent)
{
    // Set connections
    connect(&stateMachine, &StateMachine::stateChanged, this, &AppCore::onStateTransition);
}

void AppCore::boot()
{
    // Check if data directory exists

    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(appDataDir)) {
        qDebug("App boot: no app data dir; fresh build");
        stateMachine.transition(AppState::SetupWizard);
        return;
    }


    // Data directory exists; check if existence of auth, config, birth certificate files

    QString authFilePath = QDir(appDataDir).filePath("auth.txt");
    QString configFilePath = QDir(appDataDir).filePath("config.json");
    QString birthFilePath = QDir(appDataDir).filePath("birth-certificate.txt");

    bool authFileExists = QFile::exists(authFilePath);
    bool configFileExists = QFile::exists(configFilePath);
    bool birthFileExists = QFile::exists(birthFilePath);

    if (!authFileExists && !configFileExists && !birthFileExists) {
        qDebug("App boot: auth/config/birth cert not present in app data dir; fresh build");
        stateMachine.transition(AppState::SetupWizard);
        return;
    }

    if (!(authFileExists && configFileExists && birthFileExists)) {
        qDebug("App boot: exactly 1 or 2 of the 3 required files are missing; go to recovery mode");
        stateMachine.transition(AppState::RecoveryMode);
        return;
    }


    // All 3 files exist, verify they can be opened for use

    QFile authFile(authFilePath);
    QFile configFile(configFilePath);
    QFile birthFile(birthFilePath);

    authFile.open(QIODevice::ReadOnly);
    configFile.open(QIODevice::ReadWrite);  // Must be able to write to local config
    birthFile.open(QIODevice::ReadOnly);

    if (!(authFile.isOpen() && configFile.isOpen() && birthFile.isOpen())) {
        qDebug("App boot: one or more of auth/config/birth cert could not be opened for use; go to recovery mode");
        stateMachine.transition(AppState::RecoveryMode);

        authFile.close();
        configFile.close();
        birthFile.close();
        return;
    }


    // All 3 files exist, verify their contents

    static const QRegularExpression authRegex(R"(^\$argon2id\$v=\d+\$m=\d+,t=\d+,p=\d+\$[A-Za-z0-9+/]+\$[A-Za-z0-9+/]+={0,2}$)");
    static const QRegularExpression birthRegex(R"(^.+ born \d{2}-\d{2}-\d{2} \d{2}:\d{2}$)");

    bool authFileVerified = authRegex.match(QString::fromUtf8(authFile.readAll()).trimmed()).hasMatch();
    bool configFileVerified = configManager.validateLocalConfig();
    bool birthFileVerified = birthRegex.match(QString::fromUtf8(birthFile.readAll()).trimmed()).hasMatch();

    if (!authFileVerified || !configFileVerified || !birthFileVerified) {
        qDebug("App boot: one or more of auth/config/birth cert failed content validation; go to recovery mode");
        stateMachine.transition(AppState::RecoveryMode);

        authFile.close();
        configFile.close();
        birthFile.close();
        return;
    }


    // Everything looking good, clean up & transition to normal run

    authFile.close();
    configFile.close();
    birthFile.close();

    qDebug("App boot: auth/config/birth cert looking good, proceeding to normal run");
    stateMachine.transition(AppState::NormalRun);
}

void AppCore::onStateTransition(AppState updatedState)
{
    // Clean up previous objects
    delete setupWizard;
    delete recoveryWizard;
    delete mainWindow;

    serverCore.reset();
    portAuthority.reset();
    fedEx.reset();
    mailbox.reset();
    omniNetwork.reset();

    setupWizard = nullptr;
    recoveryWizard = nullptr;
    mainWindow = nullptr;

    serverCore = nullptr;
    portAuthority = nullptr;
    fedEx = nullptr;
    mailbox = nullptr;
    omniNetwork = nullptr;


    switch (updatedState) {
    case AppState::Startup:
        throw std::runtime_error("Should be impossible to transition to startup state from another, but it happened for some reason");
        break;

    case AppState::SetupWizard:
        setupWizard = new SetupWizard();
        connect(setupWizard, &QDialog::finished, this, &AppCore::onSetupWizardFinished);
        setupWizard->show();
        break;

    case AppState::RecoveryMode:
        recoveryWizard = new RecoveryWizard;
        connect(recoveryWizard, &QDialog::finished, this, &AppCore::onRecoveryWizardFinished);
        recoveryWizard->show();
        break;

    case AppState::NormalRun:
        // Server, db, and core single objects construction
        serverCore = std::make_unique<ServerCore>(this);
        portAuthority = std::make_unique<PortAuthority>(this);
        fedEx = std::make_unique<FedEx>(this);
        mailbox = std::make_unique<Mailbox>(this);
        omniNetwork = std::make_unique<OmniNetwork>(this);

        // Set local pointers to the objects above
        serverCore->setPortAuthority(portAuthority.get());
        serverCore->setFedEx(fedEx.get());
        serverCore->setMailbox(mailbox.get());
        portAuthority->setFedEx(fedEx.get());
        portAuthority->setServerCore(serverCore.get());

        // Load config info into server & db
        serverCore->loadFromConfig();
        omniNetwork->loadFromConfig();

        // Configure server SSL (cert & key will be generated by configure method if don't exist already)
        QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QString sslCertPath = QDir(appDataDir).filePath("server-cert.pem");
        QString sslKeyPath = QDir(appDataDir).filePath("server-key.pem");
        portAuthority->configureSsl(sslCertPath, sslKeyPath);

        // Server boot
        serverCore->boot();

        // GUI init
        mainWindow = new MainWindow;
        mainWindow->setNetwork(omniNetwork.get());
        connect(mainWindow, &QWidget::destroyed, this, &AppCore::onQuitRequested);

        // Wire server signals to GUI client table model
        ClientTableModel *clientTableModel = mainWindow->getClientTableModel();
        connect(serverCore.get(), &ServerCore::clientConnected, clientTableModel, &ClientTableModel::onClientConnected);
        connect(serverCore.get(), &ServerCore::clientDisconnected, clientTableModel, &ClientTableModel::onClientDisconnected);

        // Wire core systems
        connect(fedEx.get(), &FedEx::messageReady, mailbox.get(), &Mailbox::onMessageReady);
        connect(mailbox.get(), &Mailbox::youveGotMail, serverCore.get(), &ServerCore::onYouveGotMail);
        connect(serverCore.get(), &ServerCore::clientAuthenticated, portAuthority.get(), &PortAuthority::onClientAuthenticated);
        connect(serverCore.get(), &ServerCore::clientAuthFailed, portAuthority.get(), &PortAuthority::onClientAuthFailed);
        connect(portAuthority.get(), &PortAuthority::newClientSession, serverCore.get(), &ServerCore::onNewClientSession);

        // Load display
        mainWindow->show();
        break;
    }
}

void AppCore::onSetupWizardFinished()
{
    stateMachine.transition(AppState::NormalRun);
}

void AppCore::onRecoveryWizardFinished()
{
    stateMachine.transition(AppState::NormalRun);
}

void AppCore::onQuitRequested()
{
    QApplication::quit();
}
