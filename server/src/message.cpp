//
// Created by Christian Caamano on 5/17/25.
//

#include "../include/message.h"

Message::Message()
    : header()
    , type()
    , contents()
{}

Message::~Message()
{}

Message::Message(i_messageformat_t intermediateObj)
{
    header = intermediateObj["header"];
    type = intermediateObj["type"];
    contents = intermediateObj["contents"];
}

i_messageformat_t Message::toIntermediateFormat() const
{
    i_messageformat_t iObj = {
            {"header", header},
            {"type", type},
            {"contents", payload}
    };
    return iObj;
}

std::ostream& operator<<(std::ostream& os, const Message& message)
{
    i_messageformat_t iObj = message.toIntermediateFormat();
    os << iObj;
    return os;
}

