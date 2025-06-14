//
// Created by Christian Caamano on 6/6/25.
//

#include "server_core.h"

ServerCore::ServerCore(QObject *parent)
    : QObject(parent)
{

}

void ServerCore::setMailbox(Mailbox* mailboxArg)
{
    mailbox = mailboxArg;
}

void ServerCore::setFedEx(FedEx* fedExArg)
{
    fedEx = fedExArg;
}

void ServerCore::setPortAuthority(PortAuthority* portAuthorityArg)
{
    portAuthority = portAuthorityArg;
}

void ServerCore::boot()
{
    portAuthority->startListening(port);
}

void ServerCore::loadFromConfig()
{
    ConfigManager configManager;
    nlohmann::json config = configManager.getConfigFromFile();

    // Load active sessions
    activeSessions.clear();
    for (auto& [key, value] : config["network"]["server"]["active_sessions"].items()) {
        QString clientUUID = QString::fromStdString(value["client_uuid"].dump());
        QString clientHost = QString::fromStdString(value["client_host"].dump());

        activeSessions[clientUUID] = new ClientSession(clientUUID, clientHost, this);
    }

    // Load uuid, host, port
    uuid = QString::fromStdString(config["network"]["server"]["uuid"]);
    host = QString::fromStdString(config["network"]["server"]["host"]);
    port = config["network"]["server"]["port"];
}

QString ServerCore::getUUID() const
{
    return uuid;
}

QString ServerCore::getHost() const
{
    return host;
}

void ServerCore::onYouveGotMail()
{
    qDebug("server received mail notif from mailbox");
    auto queuedMessage = mailbox->openNextMessage();
    handleMessage(queuedMessage.message, queuedMessage.identifier, queuedMessage.authenticated);
}

void ServerCore::onNewClientSession(const QString& clientUUID, ClientSession* clientSession)
{
    // Set connections to server & new parent (this server)
    clientSession->setParent(this);
    connect(clientSession, &ClientSession::disconnected, this, &ServerCore::onClientSessionDisconnected);

    // Add client to active sessions list (logged in list)
    activeSessions[clientUUID] = clientSession;

    // Signal client connected for GUI
    emit clientConnected(clientSession->getClientInfo());

    // Craft & send reply to new client letting them know they've just been authenticated
    nlohmann::json ingredients;
    ingredients["server_uuid"] = uuid.toStdString();
    ingredients["server_host"] = host.toStdString();
    ingredients["client_uuid"] = clientUUID.toStdString();
    ingredients["client_host"] = clientSession->getClientHost().toStdString();

    Message* authAcceptReply = MessageBuilder::makeMessage(MessageType::AuthAccepted, ingredients);
    fedEx->shipMessage(authAcceptReply, clientSession->getSocket());
}

void ServerCore::onClientSessionDisconnected(const QString& clientUUID)
{
    emit clientDisconnected(clientUUID);
}

void ServerCore::handleMessage(const Message& message, const QString& identifier, const bool authenticated)
{
    qDebug("server handling message");

    // Get message contents
    nlohmann::json header = message.getHeader();
    MessageType type = message.getType();
    nlohmann::json payload = message.getPayload();


    qDebug() << "server received message with header:" << header.dump() << "Type" << static_cast<int>(type) << "payload:" << payload.dump();
    qDebug() << "Authenticated: " << authenticated;

    QString clientUUID = QString::fromStdString(header["sender"]["uuid"].get<std::string>());
    QString clientHost = QString::fromStdString(header["sender"]["host"].get<std::string>());
    ClientInfo clientInfo = {clientUUID, clientHost};

    // Handle unauthenticated clients separately
    if (authenticated == false) {

        qDebug("authenticated false");
        QString tempSocketId = identifier; // Identifier: temp socket id in PortAuthority

        if (type == MessageType::AuthRequest) {
            qDebug("message type auth request");
            if (credentialsValid(payload["username"].get<std::string>(), payload["password"].get<std::string>())) {
                emit clientAuthenticated(tempSocketId, clientInfo);
                qDebug("emit clientauthenticated");
            } else {
                emit clientAuthFailed(tempSocketId, clientInfo);
                qDebug("emit clientauthfailed");
            }
        }
        return;
    }


    // Handle messages from authenticated clients
    if (authenticated == true) {
        switch (type) {
        case MessageType::AddFolderRequest:
            break;
        default:
            break;
        }
    }
}

bool ServerCore::credentialsValid(const std::string& username, const std::string& password)
{
    return cryptoManager.credentialsMatchStoredAuth(username, password);
}
