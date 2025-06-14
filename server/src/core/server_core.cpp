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

QString ServerCore::getHost() const
{
    return host;
}

void ServerCore::onYouveGotMail()
{
    auto queuedMessage = mailbox->openNextMessage();
    handleMessage(queuedMessage.message, queuedMessage.tempSocketId);
}

void ServerCore::onNewClientSession(const QString& clientUUID, ClientSession* clientSession)
{
    // Add client to active sessions list (logged in list)
    activeSessions[clientUUID] = clientSession;

    // Craft & send reply to new client letting them know they've just been authenticated
    nlohmann::json ingredients;
    Message* authAcceptReply = MessageBuilder::makeMessage(MessageType::AuthAccepted, ingredients);
    fedEx->shipMessage(authAcceptReply, clientSession->getSocket());
}

void ServerCore::handleMessage(const Message& message, const QString& tempSocketId)
{
    nlohmann::json header = message.getHeader();
    MessageType type = message.getType();
    nlohmann::json payload = message.getPayload();

    // Handle unauthenticated clients separately
    if (!tempSocketId.isEmpty()) {
        if (type == MessageType::AuthRequest) {
            if (credentialsValid(payload["username"].dump(), payload["password"].dump())) {
                QString clientUUID = QString::fromStdString(header["sender"]["uuid"].get<std::string>());
                QString clientHost = QString::fromStdString(header["sender"]["host"].get<std::string>());

                ClientInfo clientInfo = {clientUUID, clientHost};

                emit clientAuthenticated(tempSocketId, clientInfo);
            } else {
                emit clientAuthFailed(tempSocketId);
            }
        }
        return;
    }

    // Handle messages from verified clients

}

bool ServerCore::credentialsValid(const std::string& username, const std::string& password)
{
    return cryptoManager.credentialsMatchStoredAuth(username, password);
}
