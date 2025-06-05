//
// Created by Christian Caamano on 5/17/25.
//

#include "../include/message.h"

messageformat_t Message::serialize() const
{
    messageformat_t messageFormatObj;

    messageFormatObj["header"] = header;
    messageFormatObj["type"] = type;
    messageFormatObj["payload"] = payload;

    return messageFormatObj;
}

void Message::deserialize(const messageformat_t& messageFormatObj)
{
    header = messageFormatObj["header"];
    type = messageFormatObj["type"];
    payload = messageFormatObj["payload"];
}

Message::Message(nlohmann::json header, std::string type, nlohmann::json payload)
    : header(header)
    , type(type)
    , payload(payload)
{}

nlohmann::json Message::getHeader() const
{
    return header;
}

std::string Message::getType() const
{
    return type;
}

nlohmann::json Message::getPayload() const
{
    return payload;
}

//Message::Message(messageformat_t intermediateObj)
//{
//    header = intermediateObj["header"];
//    type = intermediateObj["type"];
//    contents = intermediateObj["contents"];
//}

//std::ostream& operator<<(std::ostream& os, const Message& message)
//{
//    messageformat_t iObj = message.serialize();
//    os << iObj;
//    return os;
//}

