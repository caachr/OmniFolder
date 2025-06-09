#include "setup_wizard.h"
#include <QtWidgets/qboxlayout.h>


// -------------- WIZARD --------------
// -------- (Master Component) --------

SetupWizard::SetupWizard(QWidget *parent)
    : QWizard(parent)
{
    setMinimumSize(750, 550);
    resize(750, 550);

    setPage(Page_Intro, new IntroPage);

    setPage(Page_Name, new NamePage);
    setPage(Page_Creds, new CredsPage);
    setPage(Page_Beacon, new BeaconPage);
    setPage(Page_Port, new PortPage);
    setPage(Page_Confirm, new ConfirmPage);
    setPage(Page_Progress, new ProgressPage);
    setPage(Page_Done, new DonePage);

    // setPage(Page_Creds_R, new CredsPage_R);
    setPage(Page_Config_R, new ConfigPage_R);
    setPage(Page_Config_Confirm_R, new ConfigConfirmPage_R);
    setPage(Page_Port_R, new PortPage_R);
    setPage(Page_Confirm_R, new ConfirmPage_R);
    setPage(Page_Progress_R, new ProgressPage_R);
    setPage(Page_Done_R, new DonePage_R);

    setStartId(Page_Intro);
}


// ----------- INTRO PAGE -----------
// ------- (Branch Selection) -------

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));

    topLabel = new QLabel(tr("This wizard will help you set up your OmniFolder Server and OmniFolder Network."
                             "\n\nSelect 'Create new network' to set up this server with a new OmniFolder Network."
                             "\nThis will create a brand new network hosted by this server."
                             "\n\nSelect 'Restore existing network' to set up this server with an existing OmniFolder Network."
                             " Choose this option if you need to recover your network after losing your original server."));
    topLabel->setWordWrap(true);

    newRadioButton = new QRadioButton(tr("&Create new network"));
    restoreRadioButton = new QRadioButton(tr("&Restore existing network"));

    newRadioButton->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(newRadioButton);
    layout->addWidget(restoreRadioButton);
    setLayout(layout);
}

int IntroPage::nextId() const
{
    if (newRadioButton->isChecked()) {
        return SetupWizard::Page_Name;
    } else {
        return SetupWizard::Page_Config_R;
    }
}


// ------------------------------------------------------------------------
// -------------------------- "NEW" BRANCH PAGES --------------------------
// ------------------------------------------------------------------------


// ______________________________________________________________NAME PAGE

NamePage::NamePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("New OmniFolder Network - Name Selection (1/5)"));

    topLabel = new QLabel(tr("Please choose a name for your OmniFolder Network."));
    topLabel->setWordWrap(true);

    nameLine = new QLineEdit;
    nameLine->setPlaceholderText("e.g., Terry D's OmniFolder Network");

    registerField("networkName*", nameLine);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(nameLine);
    setLayout(layout);
}

int NamePage::nextId() const
{
    return SetupWizard::Page_Creds;
}


// _____________________________________________________________CREDS PAGE

CredsPage::CredsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("New OmniFolder Network - Credentials Selection (2/5)"));


    // Set elements and properties

    topLabel = new QLabel(tr("Please choose a strong username and password for your OmniFolder Network."
                             "\n\nYou will be required to present these during login and to perform recovery actions."
                             "\n\nNote: We STRONGLY suggest writing these down somewhere safe, as login and recovery become impossible without them!"));
    topLabel->setWordWrap(true);

    disclaimerLabel = new QLabel(tr("We take no responsibility for hacks/attacks stemming from weak credential choice."
                                    "\nWhile OmniFolder was built with strong security mechanisms, these mean nothing if you choose a weak password."
                                    " It is YOUR responsibility to choose sufficiently strong credentials, remember them, and keep them somewhere private."));
    disclaimerLabel->setWordWrap(true);
    disclaimerLabel->setStyleSheet("QLabel { color: gray; font-style: italic; font-size: 10pt; }");

    usernameLabel = new QLabel("Username:");
    passwordLabel = new QLabel("Password:");
    usernameConfirmLabel = new QLabel("Confirm username:");
    passwordConfirmLabel = new QLabel("Confirm password:");

    usernameLine = new QLineEdit;
    passwordLine = new QLineEdit;
    usernameConfirmLine = new QLineEdit;
    passwordConfirmLine = new QLineEdit;

    // Set password fields to hide text by default
    passwordLine->setEchoMode(QLineEdit::Password);
    passwordConfirmLine->setEchoMode(QLineEdit::Password);

    // Create show/hide password buttons
    showPasswordButton = new QPushButton("Show");
    showPasswordButton->setCheckable(true);
    showPasswordButton->setMaximumWidth(60);
    showPasswordButton->setAutoDefault(false);

    showConfirmPasswordButton = new QPushButton("Show");
    showConfirmPasswordButton->setCheckable(true);
    showConfirmPasswordButton->setMaximumWidth(60);
    showConfirmPasswordButton->setAutoDefault(false);


    // Set connections and registrations

    // Connect buttons to toggle functions
    connect(showPasswordButton, &QPushButton::toggled, this, &CredsPage::togglePasswordVisibility);
    connect(showConfirmPasswordButton, &QPushButton::toggled, this, &CredsPage::toggleConfirmPasswordVisibility);

    connect(usernameLine, &QLineEdit::textChanged, this, &CredsPage::validateInputs);
    connect(passwordLine, &QLineEdit::textChanged, this, &CredsPage::validateInputs);
    connect(usernameConfirmLine, &QLineEdit::textChanged, this, &CredsPage::validateInputs);
    connect(passwordConfirmLine, &QLineEdit::textChanged, this, &CredsPage::validateInputs);

    registerField("username", usernameLine);
    registerField("password", passwordLine);


    // ISSUE: registering these lines above as fields causes the lines to get cleared when navigating to a previous page and returning to this one.
    // In other words, user enters username & password -> goes back to name page -> returns to creds page -> username & password cleared from the line edits.
    // Haven't found a solution to this yet; may just have to roll with it.


    // Set layouts

    QHBoxLayout *usernameLayout = new QHBoxLayout;
    QHBoxLayout *passwordLayout = new QHBoxLayout;
    QHBoxLayout *usernameConfirmLayout = new QHBoxLayout;
    QHBoxLayout *passwordConfirmLayout = new QHBoxLayout;

    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameLine);

    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordLine);
    passwordLayout->addWidget(showPasswordButton);

    usernameConfirmLayout->addWidget(usernameConfirmLabel);
    usernameConfirmLayout->addWidget(usernameConfirmLine);

    passwordConfirmLayout->addWidget(passwordConfirmLabel);
    passwordConfirmLayout->addWidget(passwordConfirmLine);
    passwordConfirmLayout->addWidget(showConfirmPasswordButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    mainLayout->addWidget(disclaimerLabel);
    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addLayout(usernameConfirmLayout);
    mainLayout->addLayout(passwordConfirmLayout);
    setLayout(mainLayout);
}

int CredsPage::nextId() const
{
    return SetupWizard::Page_Beacon;
}

bool CredsPage::isComplete() const
{
    QString username = usernameLine->text();
    QString password = passwordLine->text();
    QString confirmUsername = usernameConfirmLine->text();
    QString confirmPassword = passwordConfirmLine->text();

    return !username.isEmpty() &&
           !password.isEmpty() &&
           username == confirmUsername &&
           password == confirmPassword;
}

void CredsPage::togglePasswordVisibility(bool show)
{
    if (show) {
        passwordLine->setEchoMode(QLineEdit::Normal);
        showPasswordButton->setText("Hide");
    } else {
        passwordLine->setEchoMode(QLineEdit::Password);
        showPasswordButton->setText("Show");
    }
}

void CredsPage::toggleConfirmPasswordVisibility(bool show)
{
    if (show) {
        passwordConfirmLine->setEchoMode(QLineEdit::Normal);
        showConfirmPasswordButton->setText("Hide");
    } else {
        passwordConfirmLine->setEchoMode(QLineEdit::Password);
        showConfirmPasswordButton->setText("Show");
    }
}

void CredsPage::validateInputs()
{
    emit completeChanged();
}


// ____________________________________________________________BEACON PAGE

BeaconPage::BeaconPage(QWidget *parent)
    : QWizardPage(parent)
{
    // Objects from internal directories

    beaconManager = new BeaconManager(this);


    // Title

    setTitle(tr("New OmniFolder Network - Configure Recovery Beacon (3/5)"));


    // Set elements and properties

    topLabel = new QLabel(tr("In this step, you will help configure a cloud-based recovery 'beacon' that online clients will automatically poll"
                             " for updated connection information in the event of a server crash."
                             "\n\nTo get started, you will need a GitHub account."
                             " Please go to your browser, create an account if you don't already have one, log in, then return here."
                             "\n\nIn your GitHub account, navigate to"
                             " Settings -> Developer Settings -> Personal Access Tokens -> Tokens (Classic)."
                             " Click Generate New Token -> Generate New Token (Classic)."
                             "\n\nUnder 'Note', name this token 'OmniFolder Recovery Beacon'."
                             " Set the expiration date to 'no expiration'."
                             " Under 'Select scopes', check ONLY the box next to 'gist'."
                             " Finally, scroll down and click 'create token' at the bottom."
                             "\n\nYou should now see the token on your screen."
                             " Please input your username and this token into the fields below."));
    topLabel->setWordWrap(true);

    usernameLabel = new QLabel(tr("Username:"));
    tokenLabel = new QLabel(tr("Token:"));

    usernameLine = new QLineEdit();
    usernameLine->setPlaceholderText(tr("e.g., my_github_username"));

    tokenLine = new QLineEdit();
    tokenLine->setPlaceholderText(tr("e.g., ghp_1A2b3C4d5E6f7G8h9I0j1K2l3M4n5O6p7Q8r9S"));

    testButton = new QPushButton(tr("Test Login"));

    statusLabel = new QLabel;
    statusLabel->setStyleSheet("color: gray");
    statusLabel->setText("Waiting for credentials input...");


    // Set connections and registrations

    // updateStatus(Status::None);
    testButton->setDisabled(true);

    // Test button - trigger test when clicked
    connect(testButton, &QPushButton::clicked, this, [this]() {
        updateStatus(Status::Testing);

        // Trigger async login test
        emit startLoginTest(usernameLine->text(), tokenLine->text());
    });

    // Username line - check if both fields have content when changed
    connect(usernameLine, &QLineEdit::textChanged, this, [this]() {
        if (usernameLine->text().isEmpty() || tokenLine->text().isEmpty()) {
            updateStatus(Status::None);
        } else {
            updateStatus(Status::Waiting);
        }
    });

    // Password line - check if both fields have content when changed
    connect(tokenLine, &QLineEdit::textChanged, this, [this]() {
        if (usernameLine->text().isEmpty() || tokenLine->text().isEmpty()) {
            updateStatus(Status::None);
        } else {
            updateStatus(Status::Waiting);
        }
    });

    connect(this, &BeaconPage::startLoginTest, beaconManager, &BeaconManager::testLogin);
    connect(beaconManager, &BeaconManager::loginSuccess, this, &BeaconPage::onLoginSuccess);
    connect(beaconManager, &BeaconManager::loginFailure, this, &BeaconPage::onLoginFailure);

    registerField("ghUsername", usernameLine);
    registerField("ghToken", tokenLine);


    // Set layouts

    QHBoxLayout *usernameLayout = new QHBoxLayout;
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameLine);

    QHBoxLayout *tokenLayout = new QHBoxLayout;
    tokenLayout->addWidget(tokenLabel);
    tokenLayout->addWidget(tokenLine);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(tokenLayout);
    mainLayout->addWidget(testButton);
    mainLayout->addWidget(statusLabel);
    setLayout(mainLayout);
}

int BeaconPage::nextId() const
{
    return SetupWizard::Page_Port;
}

bool BeaconPage::isComplete() const
{
    return currentStatus == Status::Success;
}

void BeaconPage::onLoginSuccess()
{
    updateStatus(Status::Success);
}

void BeaconPage::onLoginFailure()
{
    updateStatus(Status::Failure);
}

void BeaconPage::updateStatus(Status newStatus)
{
    currentStatus = newStatus;

    switch (newStatus) {
    case Status::Testing:
        statusLabel->setText("Testing credentials...");
        statusLabel->setStyleSheet("color: gray");
        disableFields();
        break;
    case Status::Success:
        statusLabel->setText("Login success, okay to proceed.");
        statusLabel->setStyleSheet("color: green");
        disableFields();
        break;
    case Status::Failure:
        statusLabel->setText("Login failed. Please double-check you entered the correct"
                             " login information, then try again.");
        statusLabel->setStyleSheet("color: red");
        enableFields();
        break;
    case Status::Waiting:
        statusLabel->setText("Waiting to test...");
        statusLabel->setStyleSheet("color: gray");
        enableFields();
        break;
    case Status::None:
        statusLabel->setText("Waiting for credentials input...");
        statusLabel->setStyleSheet("color: gray");
        enableFields();
        testButton->setEnabled(false);
        break;
    }

    // Triggers wizard to re-check isComplete()
    emit completeChanged();
}

void BeaconPage::enableFields()
{
    usernameLine->setEnabled(true);
    tokenLine->setEnabled(true);
    testButton->setEnabled(true);
}

void BeaconPage::disableFields()
{
    usernameLine->setEnabled(false);
    tokenLine->setEnabled(false);
    testButton->setEnabled(false);
}


// ______________________________________________________________PORT PAGE

PortPage::PortPage(QWidget *parent)
    : QWizardPage(parent)
{
    // Objects from internal directories

    portAuthority = new PortAuthority(this);


    // Title

    setTitle(tr("New OmniFolder Network - Configure Port Forwarding for Server (4/5)"));


    // Set elements and properties

    topLabel = new QLabel(tr("In this step, we will enable your server to receive communications from clients."
                             "\n\nPlease choose a port on your computer to receive connections."
                             "\nWe have automatically detected an available port and placed it into the field below,"
                             " but you are welcome to choose a different port."));
    topLabel->setWordWrap(true);

    portLabel = new QLabel(tr("Port:"));
    portLine = new QLineEdit();

    testPortButton = new QPushButton("Test Port Availability");

    instructionLabel = new QLabel(tr("\n\nPlease forward this port on your router, then press the button below to test the connection."));
    instructionLabel->setWordWrap(true);

    testForwardButton = new QPushButton(tr("Test Forwarded Connection"));

    statusLabel = new QLabel(tr("Waiting for user input..."));
    statusLabel->setStyleSheet("color: gray");

    helpButton = new QPushButton(tr("I Need Help"));


    // Set connections and registrations

    testPortButton->setEnabled(false);
    testForwardButton->setEnabled(false);

    connect(portLine, &QLineEdit::textChanged, this, [this]() {
        if (portLine->text().isEmpty()) {
            updateStatus(Status::None);
        } else {
            updateStatus(Status::AwaitingPort);
        }
    });

    connect(testPortButton, &QPushButton::clicked, this, [this]() {
        updateStatus(Status::TestingPort);

        // Trigger async port test
        emit startPortTest(portLine->text().toInt());
    });

    connect(testForwardButton, &QPushButton::clicked, this, [this]() {
        updateStatus(Status::TestingForward);

        // Trigger async forward test
        emit startForwardTest(portLine->text().toInt());
    });

    connect(this, &PortPage::startPortTest, portAuthority, &PortAuthority::testPort);
    connect(this, &PortPage::startForwardTest, portAuthority, &PortAuthority::testForward);

    connect(portAuthority, &PortAuthority::portTestSuccess, this, &PortPage::onPortSuccess);
    connect(portAuthority, &PortAuthority::portTestFailure, this, &PortPage::onPortFailure);

    connect(portAuthority, &PortAuthority::forwardTestSuccess, this, &PortPage::onForwardSuccess);
    connect(portAuthority, &PortAuthority::forwardTestFailure, this, &PortPage::onForwardFailure);

    connect(helpButton, &QPushButton::clicked, [](){
        QDesktopServices::openUrl(QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
    });

    registerField("port", portLine);


    // Set layouts

    QHBoxLayout *portLayout = new QHBoxLayout;
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portLine);
    portLayout->addWidget(testPortButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    mainLayout->addLayout(portLayout);
    mainLayout->addWidget(instructionLabel);
    mainLayout->addWidget(testForwardButton);
    mainLayout->addWidget(statusLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(helpButton);
    setLayout(mainLayout);
}

int PortPage::nextId() const
{
    return SetupWizard::Page_Confirm;
}

bool PortPage::isComplete() const
{
    return currentStatus == Status::ForwardSuccess;
}

void PortPage::updateStatus(Status newStatus)
{
    currentStatus = newStatus;

    switch (newStatus) {
    case Status::None:
        statusLabel->setText("Waiting for user input...");
        statusLabel->setStyleSheet("color: gray");
        portLine->setEnabled(true);
        testPortButton->setEnabled(false);
        testForwardButton->setEnabled(false);
        break;
    case Status::AwaitingPort:
        statusLabel->setText("Waiting for available port selection...");
        statusLabel->setStyleSheet("color: gray");
        enableFields();
        testForwardButton->setEnabled(false);
        break;
    case Status::TestingPort:
        statusLabel->setText("Checking port availability...");
        statusLabel->setStyleSheet("color: gray");
        disableFields();
        helpButton->setEnabled(true);
        break;
    case Status::PortSuccess:
        statusLabel->setText("Port available. Waiting to test port forward connection...");
        statusLabel->setStyleSheet("color: gray");
        enableFields();
        break;
    case Status::PortFailure:
        statusLabel->setText("Port unavailable. Please choose a different port.");
        statusLabel->setStyleSheet("color: red");
        enableFields();
        break;
    case Status::AwaitingForward:
        statusLabel->setText("Waiting to test port forward connection...");
        statusLabel->setStyleSheet("color: gray");
        enableFields();
        break;
    case Status::TestingForward:
        statusLabel->setText("Testing forwarded port connection...");
        statusLabel->setStyleSheet("color: gray");
        disableFields();
        helpButton->setEnabled(true);
        break;
    case Status::ForwardSuccess:
        statusLabel->setText("Connection succeeded, okay to proceed.");
        statusLabel->setStyleSheet("color: green");
        disableFields();
        break;
    case Status::ForwardFailure:
        statusLabel->setText("Connection failed. Please ensure you are connected to the Internet,"
                             " double-check the port-forwarding configuration on your router, and try again."
                             " If the connection still fails, you may want to choose a different port above, forward it, and try again.");
        statusLabel->setStyleSheet("color: red");
        enableFields();
        break;
    }

    emit completeChanged();
}

void PortPage::onPortSuccess()
{
    updateStatus(Status::PortSuccess);
}

void PortPage::onPortFailure()
{
    updateStatus(Status::PortFailure);
}

void PortPage::onForwardSuccess()
{
    updateStatus(Status::ForwardSuccess);
}

void PortPage::onForwardFailure()
{
    updateStatus(Status::ForwardFailure);
}

void PortPage::enableFields()
{
    portLine->setEnabled(true);
    testPortButton->setEnabled(true);
    testForwardButton->setEnabled(true);
    helpButton->setEnabled(true);
}

void PortPage::disableFields()
{
    portLine->setEnabled(false);
    testPortButton->setEnabled(false);
    testForwardButton->setEnabled(false);
    helpButton->setEnabled(false);
}


// ___________________________________________________________CONFIRM PAGE

ConfirmPage::ConfirmPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("New OmniFolder Network - Confirm Details (5/5)"));


    // Set elements and properties

    topLabel = new QLabel(tr("Please confirm the network is configured to your liking."
                             " Then, press 'Create Network'. (Once you press this button, you can't go back)"));
    topLabel->setWordWrap(true);

    nameLabel = new QLabel;
    usernameLabel = new QLabel;
    passwordLabel = new QLabel;
    portLabel = new QLabel;


    // Set password stuff & hiding logic

    showPasswordButton = new QPushButton("Show");
    showPasswordButton->setCheckable(true);

    connect(showPasswordButton, &QPushButton::toggled, this, &ConfirmPage::togglePasswordVisibility);


    // Set layouts

    QHBoxLayout *nameLayout = new QHBoxLayout;
    QHBoxLayout *usernameLayout = new QHBoxLayout;
    QHBoxLayout *passwordLayout = new QHBoxLayout;
    QHBoxLayout *portLayout = new QHBoxLayout;

    nameLayout->addWidget(nameLabel);
    usernameLayout->addWidget(usernameLabel);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(showPasswordButton);
    passwordLayout->addStretch();

    portLayout->addWidget(portLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addLayout(portLayout);
    setLayout(mainLayout);

    setButtonText(QWizard::NextButton, tr("Create Network"));
}

int ConfirmPage::nextId() const
{
    return SetupWizard::Page_Progress;
}

void ConfirmPage::initializePage()
{
    QString name = wizard()->field("networkName").toString();
    QString user = wizard()->field("username").toString();
    QString port = wizard()->field("port").toString();
    realPassword = wizard()->field("password").toString();
    maskedPassword = QString(realPassword.length(), QChar('*'));

    nameLabel->setText("Network name: " + name);
    usernameLabel->setText("Username: " + user);
    portLabel->setText("Port: " + port);
    passwordLabel->setText("Password: " + maskedPassword);
}

void ConfirmPage::togglePasswordVisibility(bool show)
{
    if (show) {
        passwordLabel->setText("Password: " + realPassword);
        showPasswordButton->setText("Hide");
    } else {
        passwordLabel->setText("Password: " + maskedPassword);
        showPasswordButton->setText("Show");
    }
}


// __________________________________________________________PROGRESS PAGE

ProgressPage::ProgressPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("New OmniFolder Network - Setup in Progress..."));

    topLabel = new QLabel(tr("Doing important things..."));
    topLabel->setWordWrap(true);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    mainLayout->addWidget(progressBar);
    setLayout(mainLayout);
}

int ProgressPage::nextId() const
{
    return SetupWizard::Page_Done;
}

void ProgressPage::initializePage()
{
    // Remove navigation buttons
    QList<QWizard::WizardButton> button_layout;
    button_layout << QWizard::Stretch << QWizard::NextButton;
    wizard()->setButtonLayout(button_layout);
}


// ______________________________________________________________DONE PAGE

DonePage::DonePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("New OmniFolder Network - Setup Complete"));

    topLabel = new QLabel(tr("Setup complete! Press 'Launch Server' to boot up the server and begin using your OmniFolder Network."));
    topLabel->setWordWrap(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    setLayout(mainLayout);
}

int DonePage::nextId() const
{
    return -1;
}

void DonePage::initializePage()
{
    // Don't show back button
    QList<QWizard::WizardButton> button_layout;
    button_layout << QWizard::Stretch << QWizard::FinishButton;
    wizard()->setButtonLayout(button_layout);

    wizard()->setButtonText(QWizard::FinishButton, tr("Launch Server"));
}



// --------------------------------------------------------------------------
// ------------------------- "RESTORE" BRANCH PAGES -------------------------
// --------------------------------------------------------------------------


// ___________________________________________________________CONFIG PAGE (R)

ConfigPage_R::ConfigPage_R(QWidget *parent)
    : QWizardPage(parent)
{
    // Objects from internal directories

    configManager = new ConfigManager(this);


    // Title

    setTitle(tr("Network Restoration - Configuration File (1/4)"));


    // Set elements and properties

    topLabel = new QLabel(tr("Please input the username and password you have been using to log in to your network."));
    topLabel->setWordWrap(true);

    usernameLabel = new QLabel(tr("Username:"));
    passwordLabel = new QLabel(tr("Password:"));

    usernameLine = new QLineEdit;
    passwordLine = new QLineEdit;

    passwordLine->setEchoMode(QLineEdit::Password);

    showPasswordButton = new QPushButton("Show");
    showPasswordButton->setCheckable(true);
    showPasswordButton->setMaximumWidth(60);
    showPasswordButton->setAutoDefault(false);

    middleLabel = new QLabel(tr("Please plug into this computer one of the drives from your existing"
                             " OmniFolder Network. Then, indicate below the path to your network's"
                             " directory on that drive."));
    middleLabel->setWordWrap(true);

    pathLine = new QLineEdit;
    pathLine->setPlaceholderText(tr("e.g., /Volumes/D/Terry's OmniFolder Network"));

    browseButton = new QPushButton("Browse");

    statusLabel = new QLabel;
    statusLabel->setStyleSheet("color: gray");
    statusLabel->setText("Waiting for user input...");


    // Set connections

    // Show/hide password
    connect(showPasswordButton, &QPushButton::toggled, this, &ConfigPage_R::togglePasswordVisibility);

    // Browse button - update text field and trigger config validation
    connect(browseButton, &QPushButton::clicked, this, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Select OmniFolder Directory"), QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (!dir.isEmpty()) {
            pathLine->setText(dir);
        }
    });

    // Path line - trigger config validation on path changed
    connect(pathLine, &QLineEdit::textChanged, this, [this](const QString &path) {
        if (path.isEmpty()) {
            updateStatus(Status::None);
            return;
        }

        QFileInfo configFile(path + "/config.enc");
        if (!configFile.exists()) {
            updateStatus(Status::NotFound);
        } else {
            updateStatus(Status::Validating);

            // Trigger async validation
            emit startValidation(path, usernameLine->text(), passwordLine->text());
        }
    });

    // Username line - retrigger config validation on username changed
    connect(usernameLine, &QLineEdit::textChanged, this, [this]() {
        // Re-trigger validation if path is already set
        if (!pathLine->text().isEmpty()) {
            QString path = pathLine->text();
            QFileInfo configFile(path + "/config.enc");
            if (configFile.exists()) {
                updateStatus(Status::Validating);
                emit startValidation(path, usernameLine->text(), passwordLine->text());
            }
        }
    });

    // Password line - retrigger config validation on password changed
    connect(passwordLine, &QLineEdit::textChanged, this, [this]() {
        // Re-trigger validation if path is already set
        if (!pathLine->text().isEmpty()) {
            QString path = pathLine->text();
            QFileInfo configFile(path + "/config.enc");
            if (configFile.exists()) {
                updateStatus(Status::Validating);
                emit startValidation(path, usernameLine->text(), passwordLine->text());
            }
        }
    });

    // Config validation signal/slot mechanism
    connect(this, &ConfigPage_R::startValidation, configManager, &ConfigManager::validateConfig);
    connect(configManager, &ConfigManager::validationSuccess, this, &ConfigPage_R::onValidationSuccess);
    connect(configManager, &ConfigManager::validationFailure, this, &ConfigPage_R::onValidationFailure);


    // Set layouts

    QHBoxLayout *usernameLayout = new QHBoxLayout;
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameLine);

    QHBoxLayout *passwordLayout = new QHBoxLayout;
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordLine);
    passwordLayout->addWidget(showPasswordButton);

    QHBoxLayout *pathLayout = new QHBoxLayout;
    pathLayout->addWidget(pathLine);
    pathLayout->addWidget(browseButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addWidget(middleLabel);
    mainLayout->addLayout(pathLayout);
    mainLayout->addWidget(statusLabel);
    setLayout(mainLayout);
}

int ConfigPage_R::nextId() const
{
    return SetupWizard::Page_Config_Confirm_R;
}

bool ConfigPage_R::isComplete() const
{
    return currentStatus == Status::Passed;
}

void ConfigPage_R::onValidationSuccess()
{
    updateStatus(Status::Passed);
}

void ConfigPage_R::onValidationFailure()
{
    updateStatus(Status::Failed);
}

void ConfigPage_R::togglePasswordVisibility(bool show)
{
    if (show) {
        passwordLine->setEchoMode(QLineEdit::Normal);
        showPasswordButton->setText("Hide");
    } else {
        passwordLine->setEchoMode(QLineEdit::Password);
        showPasswordButton->setText("Show");
    }
}

void ConfigPage_R::updateStatus(Status newStatus)
{
    currentStatus = newStatus;

    switch (newStatus) {
    case Status::NotFound:
        statusLabel->setText("config.enc not found at the selected directory.");
        statusLabel->setStyleSheet("color: red");
        enableFields();
        break;
    case Status::Validating:
        statusLabel->setText("config.enc found. Validating...");
        statusLabel->setStyleSheet("color: gray");
        disableFields();
        break;
    case Status::Failed:
        statusLabel->setText("config.enc found, but failed validation; either your username and/or password was incorrect,"
                             " you entered a path that didn't correspond to your OmniFolder Network, or the file is corrupted."
                             "\nPlease double-check you entered the correct username, password, and path."
                             "\nIf you are sure they are correct, please choose a different drive from the network and try again.");
        statusLabel->setStyleSheet("color: red");
        enableFields();
        break;
    case Status::Passed:
        statusLabel->setText("config.enc found and validated, okay to proceed.");
        statusLabel->setStyleSheet("color: green");
        enableFields();
        break;
    case Status::None:
        statusLabel->setText("Waiting for user input...");
        statusLabel->setStyleSheet("color: gray");
        enableFields();
        break;
    }

    // Triggers wizard to re-check isComplete()
    emit completeChanged();
}

void ConfigPage_R::enableFields()
{
    usernameLine->setEnabled(true);
    passwordLine->setEnabled(true);
    pathLine->setEnabled(true);
    browseButton->setEnabled(true);
}

void ConfigPage_R::disableFields()
{
    usernameLine->setEnabled(false);
    passwordLine->setEnabled(false);
    pathLine->setEnabled(false);
    browseButton->setEnabled(false);
}


// ___________________________________________________CONFIG CONFIRM PAGE (R)

ConfigConfirmPage_R::ConfigConfirmPage_R(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Network Restoration - Confirm Configuration (2/4)"));

    topLabel = new QLabel(tr("Please confirm that the details below match the network you intend to restore."
                             " If you do not recognize this information, it's possible you chose the wrong"
                             " network to restore in the previous page."));
    topLabel->setWordWrap(true);

    networkNameLabel = new QLabel(tr("Network name:"));

    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addWidget(networkNameLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    mainLayout->addLayout(configLayout);
    setLayout(mainLayout);
}

int ConfigConfirmPage_R::nextId() const
{
    return SetupWizard::Page_Port_R;
}


// _____________________________________________________________PORT PAGE (R)

PortPage_R::PortPage_R(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Network Restoration - Configure Port Forwarding for Server (3/4)"));

    topLabel = new QLabel(tr("Please choose a port on your computer to receive connections."
                             "\n\nIf you are using the same router as before,"
                             " simply enter the port you originally forwarded below."
                             " Otherwise, please choose a port and forward it on your router."
                             "\n\nOnce port forwarding has been configured, test the connection with the provided button to continue."));
    topLabel->setWordWrap(true);

    portLabel = new QLabel(tr("Port:"));
    portLine = new QLineEdit();

    testButton = new QPushButton(tr("Test Connection"));
    statusLabel = new QLabel(tr("Waiting to test..."));

    helpButton = new QPushButton(tr("I need help"));


    QHBoxLayout *portLayout = new QHBoxLayout;
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portLine);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    mainLayout->addLayout(portLayout);
    mainLayout->addWidget(testButton);
    mainLayout->addWidget(statusLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(helpButton);
    setLayout(mainLayout);
}

int PortPage_R::nextId() const
{
    return SetupWizard::Page_Confirm_R;
}


// __________________________________________________________CONFIRM PAGE (R)

ConfirmPage_R::ConfirmPage_R(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Network Restoration - Confirm Network Details (4/4)"));

    topLabel = new QLabel(tr("Please confirm the network is configured correctly and to your liking."
                             "\nThen, press 'Restore Network'. (Once you press this button, you can't go back)"));
    topLabel->setWordWrap(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    setLayout(mainLayout);

    setButtonText(QWizard::NextButton, tr("Restore Network"));
}

int ConfirmPage_R::nextId() const
{
    return SetupWizard::Page_Progress_R;
}


// _________________________________________________________PROGRESS PAGE (R)

ProgressPage_R::ProgressPage_R(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Network Restoration - Restoration in Progress..."));

    topLabel = new QLabel(tr("Doing important things..."));
    topLabel->setWordWrap(true);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    mainLayout->addWidget(progressBar);
    setLayout(mainLayout);
}

int ProgressPage_R::nextId() const
{
    return SetupWizard::Page_Done_R;
}

void ProgressPage_R::initializePage()
{
    // Remove navigation buttons
    QList<QWizard::WizardButton> button_layout;
    button_layout << QWizard::Stretch << QWizard::NextButton;
    wizard()->setButtonLayout(button_layout);
}


// _____________________________________________________________DONE PAGE (R)

DonePage_R::DonePage_R(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Network Restoration - Restoration Complete"));

    topLabel = new QLabel(tr("Restoration complete!"
                             "\n\nPress 'Launch Server' to boot up your new server"
                             " and begin using your restored OmniFolder Network."));
    topLabel->setWordWrap(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(topLabel);
    setLayout(mainLayout);
}

int DonePage_R::nextId() const
{
    return -1;
}

void DonePage_R::initializePage()
{
    // Don't show back button
    QList<QWizard::WizardButton> button_layout;
    button_layout << QWizard::Stretch << QWizard::FinishButton;
    wizard()->setButtonLayout(button_layout);

    wizard()->setButtonText(QWizard::FinishButton, tr("Launch Server"));
}
