//
// Created by Christian Caamano on 5/17/25.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "defines.h"
#include "serializable.h"

class Message : Serializable<messageformat_t> {
public:

    /*** --- SUPER CLASS OVERRIDES --- ***/

    /**
     * Serialize this Message to an equivalent message format object.
     * @return The resulting message format object.
     */
    messageformat_t serialize() const override;

    /**
     * Initialize this Message from a message format object.
     * @param messageFormatObj The message format object to use.
     */
    void deserialize(const messageformat_t& messageFormatObj) override;


    /*** --- CLASS METHODS --- ***/

    ~Message() = default;

    /**
     * Get this Message's header.
     * @return The header of this Message (containing sender/receiver info + network uuid)
     */
    messageformat_t getHeader() const;

    /**
     * Get this Message's type.
     * @return The type of this Message (heartbeat, loginRequest, etc)
     */
    std::string getType() const;

    /**
     * Get this Message's payload.
     * @return The payload of this Message.
     */
    messageformat_t getPayload() const;

//    /**
//     * Construct a new Message from an intermediate message format object.
//     * @param intermediateObj The intermediate message format object to use for construction.
//     */
//    explicit Message(messageformat_t intermediateObj);

//    friend std::ostream& operator<<(std::ostream& os, const Message& message);

//    std::string getPayload() const;

    // Only MessageFactory should be allowed to construct Messages.
    friend class MessageFactory;

private:

    /**
     * Construct a new message with the specified header, type, and payload.
     * @param header Header information for this message.
     * @param type The type for this message.
     * @param payload The message payload.
     */
    Message(messageformat_t header, std::string type, messageformat_t payload);

    /**
     * Header containing the following message metadata:
     * sender type, receiver type (client or server)
     * sender ip & port, receiver ip & port
     * omninetwork uuid
     */
    messageformat_t header;

//    /**
//     * IP address of the host sending the message.
//     */
//    std::string senderIp;
//
//    /**
//     * Port of the host sending the message.
//     */
//    uint32_t senderPort;
//
//    /**
//     * IP address of the host receiving the message.
//     */
//    std::string receiverIp;
//
//    /**
//     * Port of the host receiving the message.
//     */
//    uint32_t receiverPort;

    /**
     * Message type: heartbeatPing, actionRequest, or loginAttempt.
     * Determines how the message contents will be interpreted.
     */
    std::string type;

    /**
     * Meaningful contents of the message.
     */
    messageformat_t payload;
};

#endif //MESSAGE_H
