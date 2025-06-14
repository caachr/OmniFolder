//
// Created by Christian Caamano on 6/6/25.
//

#include <QDebug>

#include "message_builder.h"

Message* MessageBuilder::makeMessage(const MessageType type, const nlohmann::json ingredients)
{
    switch (type) {
    case MessageType::AuthAccepted:
        return makeAuthAccepted(ingredients);
        break;
    case MessageType::AuthRejected:
        return makeAuthRejected(ingredients);
        break;
    default:
        throw std::runtime_error("MessageBuilder makeMessage: attempted to make unrecognized or unsupported message type.");
    }
}

Message* MessageBuilder::makeAuthAccepted(const nlohmann::json ingredients)
{
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;

    header["sender"]["class"] = "server";
    header["sender"]["uuid"] = ingredients["server_uuid"].get<std::string>();
    header["sender"]["host"] = ingredients["server_host"].get<std::string>();

    header["receiver"]["class"] = "client";
    header["receiver"]["uuid"] = ingredients["client_uuid"].get<std::string>();
    header["receiver"]["host"] = ingredients["client_host"].get<std::string>();

    type = MessageType::AuthAccepted;

    payload["details"] = "Login request accepted: correct username and password.";

    return new Message(header, type, payload);
}

Message* MessageBuilder::makeAuthRejected(const nlohmann::json ingredients)
{
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;

    qDebug("MessageBuilder: setting sender of auth rejected message");

    header["sender"]["class"] = "server";
    header["sender"]["uuid"] = ingredients["server_uuid"].get<std::string>();
    header["sender"]["host"] = ingredients["server_host"].get<std::string>();

    qDebug("MessageBuilder: setting receiver of auth rejected message");

    header["receiver"]["class"] = "client";
    header["receiver"]["uuid"] = ingredients["client_uuid"].get<std::string>();
    header["receiver"]["host"] = ingredients["client_host"].get<std::string>();

    qDebug("MessageBuilder: setting type of auth rejected message");

    type = MessageType::AuthRejected;

    qDebug("MessageBuilder: setting payload of auth rejected message");

    payload["details"] = "Login request rejected; incorrect username and/or password.";

    qDebug("MessageBuilder: creating auth rejected final message object");

    return new Message(header, type, payload);
}
