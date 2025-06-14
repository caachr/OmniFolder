//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_PORT_AUTHORITY_H
#define OMNISERVER_PORT_AUTHORITY_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QSslServer>
#include <QSslKey>

#include "client_info.h"
#include "client_session.h"
#include "message_builder.h"

class ServerCore;

class PortAuthority : public QObject {
    Q_OBJECT

public:
    explicit PortAuthority(QObject *parent = nullptr);

    void configureSsl(const QString& certPath, const QString& keyPath, const QString& password = QString());

    // Set local pointer to unique fedex & servercore instances owned by app core
    void setFedEx(FedEx* fedExArg);
    void setServerCore(ServerCore* serverCore);

    // For testing purposes
    const std::string getExternalIp();

    // Start/stop the server
    void startListening(const qint32 port);
    void stopListening();

signals:
    // For testing purposes
    void portTestSuccess();
    void portTestFailure();
    void forwardTestSuccess();
    void forwardTestFailure();
    void forwardTestUnavailable();

    // Transfer ownership of newly constructed client session to server core
    void newClientSession(const QString& clientUUID, ClientSession* clientSession);

public slots:
    // For testing purposes
    void testPort(const qint32 port);
    void testForward(const qint32 port);

    // Manage unauthenticated sockets
    void onNewConnection();
    void onRawDataReceived(const QString& tempSocketId);
    void onSocketDisconnected(const QString& tempSocketId);

    void onSslErrors(const QString& tempSocketId, const QList<QSslError>& errors);

    // Receive signals from server core if client was/wasn't authenticated;
    // On authenticated, construct client session with approved socket; on failed, remove socket from pending list
    void onClientAuthenticated(const QString& tempSocketId, const ClientInfo& clientInfo);
    void onClientAuthFailed(const QString& tempSocketId);

private:
    QNetworkAccessManager *networkManager;

    QSslServer* serverSocket = nullptr;
    QSslCertificate sslCertificate;
    QSslKey sslPrivateKey;
    QString sslPassword;

    QHash<QString, QSslSocket*> socketsPendingAuth;

    FedEx* fedEx = nullptr;            // Local pointer to unique fedex instance owned by app core
    ServerCore* serverCore = nullptr;  // Local pointer to unique servercore instance owned by app core

    static const int LOGIN_TIMEOUT_MS = 10000; // 10 seconds

    bool generateSelfSignedCertificates(const QString& certPath, const QString& keyPath);
};


#endif //OMNISERVER_PORT_AUTHORITY_H
