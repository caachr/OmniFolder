#ifndef OMNICLIENT_MESSAGE_BUILDER_H
#define OMNICLIENT_MESSAGE_BUILDER_H

#include "message.h"

class MessageBuilder {
public:
    static Message* makeMessage(const MessageType type, const nlohmann::json ingredients);

private:
    static Message* makeAuthRequest(const nlohmann::json ingredients);
    static Message* makeCreateFolderRequest(const nlohmann::json ingredients);
    static Message* makeEditFolderRequest(const nlohmann::json ingredients);
    static Message* makePushChangesRequest(const nlohmann::json ingredients);
    static Message* makeRemoveFolderRequest(const nlohmann::json ingredients);
};

#endif //OMNICLIENT_MESSAGE_BUILDER_H
