#include "client_session.h"


ClientSession::ClientSession(QSslSocket* socketArg, const ClientInfo& clientInfo, QObject* parent)
    : socket(socketArg)
    , clientUUID(clientInfo.clientUUID)
    , clientHost(clientInfo.clientHost)
    , QObject(parent)
{
    // Take ownership of socket
    socket->setParent(this);

    // Set connections for socket signals
    connect(socket, &QSslSocket::readyRead, this, &ClientSession::onSocketDataReceived);
    connect(socket, &QSslSocket::disconnected, this, &ClientSession::onSocketDisconnected);
}

ClientSession::ClientSession(const QString& clientUUID, const QString& clientHost, QObject* parent)
    : clientUUID(clientUUID)
    , clientHost(clientHost)
    , QObject(parent)
{

}

void ClientSession::setFedEx(FedEx* fedExArg)
{
    fedEx = fedExArg;
}

void ClientSession::onSocketDataReceived()
{
    QByteArray rawData = socket->readAll();
    fedEx->processRawData(rawData, QString());
}

void ClientSession::onSocketDisconnected()
{
    qDebug() << "Client disconnected:" << clientUUID;
    emit disconnected();
}

QSslSocket* ClientSession::getSocket() const
{
    return socket;
}

QString ClientSession::getClientUUID() const noexcept
{
    return clientUUID;
}

QString ClientSession::getClientHost() const noexcept
{
    return clientHost;
}
