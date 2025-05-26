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

Message::Message(messageformat_t header, std::string type, messageformat_t payload)
    : header(header)
    , type(type)
    , payload(payload)
{}

messageformat_t Message::getHeader() const
{
    return header;
}

std::string Message::getType() const
{
    return type;
}

messageformat_t Message::getPayload() const
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

