#include <QDebug>

#include "message.h"

Message::Message(nlohmann::json header, MessageType type, nlohmann::json payload)
    : header(header)
    , type(type)
    , payload(payload)
{
}

nlohmann::json Message::getHeader() const
{
    return header;
}

MessageType Message::getType() const
{
    return type;
}

nlohmann::json Message::getPayload() const
{
    return payload;
}
