//
// Created by Christian Caamano on 5/17/25.
//

#ifndef SOCKET_MESSAGE_H
#define SOCKET_MESSAGE_H

#include <string>
#include "defines.h"

class Message {
public:
    Message();
    ~Message();

    /**
     * Construct a new Message from an intermediate message format object.
     * @param intermediateObj The intermediate message format object to use for construction.
     */
    explicit Message(i_messageformat_t intermediateObj);

    i_messageformat_t toIntermediateFormat() const;

    friend std::ostream& operator<<(std::ostream& os, const Message& message);

    std::string getType() const;

    std::string getPayload() const;

private:
//    /**
//     * Message header: various networking information and sender identifier.
//     */
//    std::string header;

    std::string senderIP;
    std::string senderPort;

    /**
     * Message type: heartbeatPing, actionRequest, or loginAttempt.
     * Determines how the message contents will be interpreted.
     */
    std::string type;

    /**
     * Meaningful contents of the message.
     */
    std::string payload;
};

#endif //SOCKET_MESSAGE_H
