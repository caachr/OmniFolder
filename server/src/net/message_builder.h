//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_MESSAGE_BUILDER_H
#define OMNISERVER_MESSAGE_BUILDER_H

#include "message.h"

class MessageBuilder {
public:
    static Message* makeMessage(const MessageType type, const nlohmann::json ingredients);

private:
    static Message* makeAuthAccepted(const nlohmann::json ingredients);
    static Message* makeAuthRejected(const nlohmann::json ingredients);
};


#endif //OMNISERVER_MESSAGE_BUILDER_H
