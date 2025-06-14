//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_MESSAGE_H
#define OMNISERVER_MESSAGE_H

#include <nlohmann/json.hpp>

enum class MessageType {
    // Client message types (only these will be in the queue)
    AuthRequest,
    AddFolderRequest,

    // Server reply types (none of these will be in the queue)
    AuthAccepted,
    AuthRejected
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


#endif //OMNISERVER_MESSAGE_H
