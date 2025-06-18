#include "login_screen.h"
#include <QApplication>

LoginScreen::LoginScreen(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("Welcome - Login");
    resize(400, 300);

    // Connect signals
    connect(loginButton, &QPushButton::clicked, this, &LoginScreen::onLoginClicked);
    connect(togglePasswordButton, &QPushButton::clicked, this, &LoginScreen::onTogglePasswordVisibility);

    // Set default focus to IP field
    ipEdit->setFocus();
}

void LoginScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // Welcome message
    welcomeLabel = new QLabel("Welcome to the OmniFolder Client application!\n\n"
                              "Please enter your server's connection details and your credentials to get started.");
    welcomeLabel->setWordWrap(true);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("QLabel { font-size: 14px; margin: 10px; }");

    // Form layout
    auto *formLayout = new QGridLayout();

    // IP Address
    formLayout->addWidget(new QLabel("Server IP:"), 0, 0);
    ipEdit = new QLineEdit();
    ipEdit->setPlaceholderText("192.168.1.100");
    formLayout->addWidget(ipEdit, 0, 1, 1, 2);

    // Port
    formLayout->addWidget(new QLabel("Port:"), 1, 0);
    portSpinBox = new QSpinBox();
    portSpinBox->setRange(1, 65535);
    portSpinBox->setValue(8080);
    formLayout->addWidget(portSpinBox, 1, 1, 1, 2);

    // Username
    formLayout->addWidget(new QLabel("Username:"), 2, 0);
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Enter username");
    formLayout->addWidget(usernameEdit, 2, 1, 1, 2);

    // Password with show/hide toggle
    formLayout->addWidget(new QLabel("Password:"), 3, 0);
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Enter password");

    togglePasswordButton = new QPushButton("Show");
    togglePasswordButton->setMaximumWidth(60);
    togglePasswordButton->setCheckable(true);

    formLayout->addWidget(passwordEdit, 3, 1);
    formLayout->addWidget(togglePasswordButton, 3, 2);

    // Status label
    statusLabel = new QLabel();

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    loginButton = new QPushButton("Log In");
    loginButton->setDefault(true);

    buttonLayout->addWidget(loginButton);

    // Add everything to main layout
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(statusLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    // Set some spacing
    mainLayout->setSpacing(15);
    formLayout->setVerticalSpacing(10);
    formLayout->setHorizontalSpacing(10);
}

void LoginScreen::onLoginClicked()
{
    QString ip = ipEdit->text().trimmed();
    int port = portSpinBox->value();
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    // Basic validation
    if (ip.isEmpty()) {
        ipEdit->setFocus();
        return;
    }

    if (username.isEmpty()) {
        usernameEdit->setFocus();
        return;
    }

    if (password.isEmpty()) {
        passwordEdit->setFocus();
        return;
    }

    // Emit the signal with the entered data
    emit loginRequested(ip, port, username, password);
}

void LoginScreen::onTogglePasswordVisibility()
{
    if (togglePasswordButton->isChecked()) {
        passwordEdit->setEchoMode(QLineEdit::Normal);
        togglePasswordButton->setText("Hide");
    } else {
        passwordEdit->setEchoMode(QLineEdit::Password);
        togglePasswordButton->setText("Show");
    }
}

void LoginScreen::onLoginSuccess()
{
    statusLabel->setText("Login successful. Proceeding to client interface...");

    // Accept & finish
    accept();
}

void LoginScreen::onLoginFailure(const std::string& failureReason)
{
    QString newStatusText = "Login failed. Reason: " + QString::fromStdString(failureReason);
    statusLabel->setText(newStatusText);
}
