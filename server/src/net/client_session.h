#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <QObject>
#include <QString>
#include <QSslSocket>

#include "fedex.h"
#include "client_info.h"

class ClientSession : public QObject {
    Q_OBJECT

public:
    explicit ClientSession(QSslSocket* socket, const ClientInfo& clientInfo, QObject* parent = nullptr);

    // Non-standard constructor called during recovery/restoration from config file info
    explicit ClientSession(const QString& clientUUID, const QString& clientHost, QObject* parent = nullptr);

    // Set pointers to unique object instances from app core
    void setFedEx(FedEx* fedExArg);

    QSslSocket* getSocket() const;
    ClientInfo getClientInfo() const;
    QString getClientUUID() const noexcept;
    QString getClientHost() const noexcept;

signals:
    // void dataReceived(const QByteArray& rawData, const QString& = QString());   // Empty 2nd param (not a temp socket)
    void disconnected(const QString& clientUUID);

private slots:
    void onSocketDataReceived();  // Will call fedex to process the raw data
    void onSocketDisconnected();

private:
    QSslSocket *socket = nullptr;  // The active TLS socket
    QString clientUUID;
    QString clientHost;

    FedEx* fedEx;   // Pointer to unique fedex instance from app core
};

#endif // CLIENT_SESSION_H
