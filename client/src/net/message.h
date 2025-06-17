#ifndef OMNICLIENT_MESSAGE_H
#define OMNICLIENT_MESSAGE_H

#include <nlohmann/json.hpp>

enum class MessageType {
    // Client message types
    AuthRequest = 1,
    CreateFolderRequest = 2,
    EditFolderRequest = 3,
    PushChangesRequest = 4,
    RemoveFolderRequest = 5,

    // Server reply types
    AuthAccepted = 6,
    AuthRejected = 7
};

class Message {
public:
    explicit Message(nlohmann::json header, MessageType type, nlohmann::json payload);

    nlohmann::json getHeader() const;
    MessageType getType() const;
    nlohmann::json getPayload() const;

private:
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;
};

#endif //OMNICLIENT_MESSAGE_H
