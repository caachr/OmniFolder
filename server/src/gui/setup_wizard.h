#ifndef SETUP_WIZARD_H
#define SETUP_WIZARD_H

#include <QWizard>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QProgressBar>
#include <QDesktopServices>

#include "core/config_manager.h"
#include "recovery/beacon_manager.h"
#include "net/port_authority.h"


// -------------- WIZARD --------------
// -------- (Master Component) --------

class SetupWizard : public QWizard
{
    Q_OBJECT

public:
    enum {
        Page_Intro,

        Page_Name,
        Page_Creds,
        Page_Beacon,
        Page_Port,
        Page_Confirm,
        Page_Progress,
        Page_Done,

        Page_Config_R,
        Page_Config_Confirm_R,
        Page_Port_R,
        Page_Confirm_R,
        Page_Progress_R,
        Page_Done_R
    };

    SetupWizard(QWidget *parent = nullptr);
};


// ----------- INTRO PAGE -----------
// ------- (Branch Selection) -------

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *topLabel;
    QRadioButton *newRadioButton;
    QRadioButton *restoreRadioButton;
};



// --------------------------------------------------
// --------------- "NEW" BRANCH PAGES ---------------
// --------------------------------------------------


// __________________________________________NAME PAGE

class NamePage : public QWizardPage
{
    Q_OBJECT

public:
    NamePage(QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *topLabel;
    QLineEdit *nameLine;
};


// _________________________________________CREDS PAGE

class CredsPage : public QWizardPage
{
    Q_OBJECT

public:
    CredsPage(QWidget *parent = nullptr);

    int nextId() const override;

    bool isComplete() const override;

private:
    QLabel *topLabel;
    QLabel *disclaimerLabel;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *usernameConfirmLabel;
    QLabel *passwordConfirmLabel;
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
    QLineEdit *usernameConfirmLine;
    QLineEdit *passwordConfirmLine;
    QPushButton *showPasswordButton;
    QPushButton *showConfirmPasswordButton;

    void togglePasswordVisibility(bool show);
    void toggleConfirmPasswordVisibility(bool show);

    void validateInputs();
};


// ________________________________________BEACON PAGE

class BeaconPage : public QWizardPage
{
    Q_OBJECT

public:
    BeaconPage(QWidget *parent = nullptr);

    int nextId() const override;

    bool isComplete() const override;

    void onLoginSuccess();
    void onLoginFailure();

signals:
    void startLoginTest(const QString &ghUsername, const QString &ghToken);

private:
    QLabel *topLabel;
    QLabel *usernameLabel;
    QLabel *tokenLabel;
    QLabel *statusLabel;
    QLineEdit *usernameLine;
    QLineEdit *tokenLine;
    QPushButton *testButton;

    BeaconManager *beaconManager;

    // Status of beacon login test
    enum class Status { None, Waiting, Testing, Success, Failure };
    Status currentStatus = Status::None;

    void updateStatus(Status newStatus);

    void enableFields();
    void disableFields();
};


// __________________________________________PORT PAGE

class PortPage : public QWizardPage
{
    Q_OBJECT

public:
    PortPage(QWidget *parent = nullptr);

    int nextId() const override;

    bool isComplete() const override;

    void onPortSuccess();
    void onPortFailure();

    void onForwardSuccess();
    void onForwardFailure();

signals:
    void startPortTest(const qint32 &port);
    void startForwardTest(const qint32 &port);

private:
    QLabel *topLabel;
    QLabel *portLabel;
    QLabel *instructionLabel;
    QLabel *statusLabel;
    QLineEdit *portLine;
    QPushButton *testPortButton;
    QPushButton *testForwardButton;
    QPushButton *helpButton;

    PortAuthority *portAuthority;

    // Status of port test
    enum class Status { None,
                        AwaitingPort, TestingPort, PortSuccess, PortFailure,
                        AwaitingForward, TestingForward, ForwardSuccess, ForwardFailure };
    Status currentStatus = Status::AwaitingPort;

    void updateStatus(Status newStatus);

    void enableFields();
    void disableFields();
};


// _______________________________________CONFIRM PAGE

class ConfirmPage : public QWizardPage
{
    Q_OBJECT

public:
    ConfirmPage(QWidget *parent = nullptr);

    int nextId() const override;

    void initializePage() override;

private:
    QLabel *topLabel;
    QLabel *nameLabel;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *portLabel;
    QString realPassword;
    QString maskedPassword;
    QPushButton *showPasswordButton;

    void togglePasswordVisibility(bool show);
};


// ______________________________________PROGRESS PAGE

class ProgressPage : public QWizardPage
{
    Q_OBJECT

public:
    ProgressPage(QWidget *parent = nullptr);

    int nextId() const override;

    void initializePage() override;

private:
    QLabel *topLabel;
    QProgressBar *progressBar;
};


// __________________________________________DONE PAGE

class DonePage : public QWizardPage
{
    Q_OBJECT

public:
    DonePage(QWidget *parent = nullptr);

    int nextId() const override;

    void initializePage() override;

private:
    QLabel *topLabel;
};





// ------------------------------------------------------
// --------------- "RESTORE" BRANCH PAGES ---------------
// ------------------------------------------------------


// _______________________________________CONFIG PAGE (R)

class ConfigPage_R : public QWizardPage
{
    Q_OBJECT

public:
    ConfigPage_R(QWidget *parent = nullptr);

    int nextId() const override;

    bool isComplete() const override;

signals:
    void startValidation(const QString &path, const QString &username, const QString &password);

public slots:
    void onValidationSuccess();
    void onValidationFailure();

private:
    QLabel *topLabel;
    QLabel *middleLabel;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *statusLabel;
    QLineEdit *pathLine;
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
    QPushButton *browseButton;
    QPushButton *showPasswordButton;

    ConfigManager *configManager;

    enum class Status { None, NotFound, Validating, Failed, Passed };
    Status currentStatus = Status::None;

    void togglePasswordVisibility(bool show);

    void updateStatus(Status newStatus);

    void enableFields();
    void disableFields();
};


// _______________________________CONFIG CONFIRM PAGE (R)

class ConfigConfirmPage_R : public QWizardPage
{
    Q_OBJECT

public:
    ConfigConfirmPage_R(QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *topLabel;
    QLabel *networkNameLabel;
};


// _________________________________________PORT PAGE (R)

class PortPage_R : public QWizardPage
{
    Q_OBJECT

public:
    PortPage_R(QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *topLabel;
    QLabel *portLabel;
    QLabel *statusLabel;
    QLineEdit *portLine;
    QPushButton *testButton;
    QPushButton *helpButton;
};


// ______________________________________CONFIRM PAGE (R)

class ConfirmPage_R : public QWizardPage
{
    Q_OBJECT

public:
    ConfirmPage_R(QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *topLabel;
};


// _____________________________________PROGRESS PAGE (R)

class ProgressPage_R : public QWizardPage
{
    Q_OBJECT

public:
    ProgressPage_R(QWidget *parent = nullptr);

    int nextId() const override;

    void initializePage() override;

private:
    QLabel *topLabel;
    QProgressBar *progressBar;
};


// _________________________________________DONE PAGE (R)

class DonePage_R : public QWizardPage
{
    Q_OBJECT

public:
    DonePage_R(QWidget *parent = nullptr);

    int nextId() const override;

    void initializePage() override;

private:
    QLabel *topLabel;
};


#endif // SETUP_WIZARD_H
