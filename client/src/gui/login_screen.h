#ifndef OMNICLIENT_LOGINSCREEN_H
#define OMNICLIENT_LOGINSCREEN_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

class LoginScreen : public QDialog
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);

signals:
    void loginRequested(const QString &ip, int port, const QString &username, const QString &password);

public slots:
    void onLoginSuccess();
    void onLoginFailure(const std::string& failureReason);

private slots:
    void onLoginClicked();
    void onTogglePasswordVisibility();

private:
    void setupUI();

    QLabel *welcomeLabel;
    QLineEdit *ipEdit;
    QSpinBox *portSpinBox;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *togglePasswordButton;
    QLabel *statusLabel;
    QPushButton *loginButton;
};

#endif //OMNICLIENT_LOGINSCREEN_H
