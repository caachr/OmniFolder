//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_BEACON_MANAGER_H
#define OMNISERVER_BEACON_MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonValue>

class BeaconManager : public QObject {
    Q_OBJECT

public:
    explicit BeaconManager(QObject *parent = nullptr);

    std::string createBeacon(const QString &ghToken);

signals:
    void loginSuccess();
    void loginFailure();
    void loginTimeout();

public slots:
    void testLogin(const QString &ghUsername, const QString &ghToken);

private:
    QNetworkAccessManager *m_networkManager;

    static const int LOGIN_TIMEOUT_MS = 10000; // 10 seconds

    void testLogin_CreateGist(const QString &ghToken);
    void testLogin_EditGist(const QString &ghToken, const QString &gistId);
    void testLogin_DeleteGist(const QString &ghToken, const QString &gistId);
};

#endif //OMNISERVER_BEACON_MANAGER_H
