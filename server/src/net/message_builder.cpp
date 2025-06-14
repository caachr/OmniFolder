//
// Created by Christian Caamano on 6/6/25.
//

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
    MessageType type = MessageType::AuthAccepted;
    nlohmann::json payload;

    return new Message(header, type, payload);
}

Message* MessageBuilder::makeAuthRejected(const nlohmann::json ingredients)
{
    nlohmann::json header;
    MessageType type = MessageType::AuthRejected;
    nlohmann::json payload;

    return new Message(header, type, payload);
}
