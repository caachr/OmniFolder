#ifndef OMNICLIENT_PORT_AUTHORITY_H
#define OMNICLIENT_PORT_AUTHORITY_H

#include <QObject>
#include <QSslSocket>

#include "message.h"

class ClientCore;
class FedEx;

class PortAuthority : public QObject {
    Q_OBJECT

public:
    explicit PortAuthority(QObject *parent = nullptr);

    void setClientCore(ClientCore* clientCoreArg);
    void setFedEx(FedEx* fedExArg);

    void connectToServer(const QString& host, quint16 port);

    // Write a frame of raw data to the server.
    void write(const QByteArray& dataFrame);

    void onSslErrors(const QList<QSslError>& errors);

    void handleFirstConnection(const QSslCertificate& cert, const QString& fingerprint);
    void handleSubsequentConnection(const QSslCertificate& cert, const QString& currentFingerprint);

signals:
    void loginSuccess();
    void loginFailure(const std::string& failureReason);

    void serverConnectionLost();

public slots:
    void onLoginRequested(const QString &ip, int port, const QString &username, const QString &password);
    void onLoginReplyReady(const Message& reply);

private slots:
    void onSocketConnectionLost();
    void onDataReceived();

private:
    QSslSocket* socket = nullptr;
    QString serverHost;
    quint16 serverPort;
    QString storedFingerprint;
    bool isFirstConnection = false;

    ClientCore* clientCore = nullptr;
    FedEx* fedEx = nullptr;

    void sendLoginMessage();
};


#endif //OMNICLIENT_PORT_AUTHORITY_H
