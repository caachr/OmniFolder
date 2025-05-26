//
// Created by Christian Caamano on 5/25/25.
//

#ifndef MESSAGE_FACTORY_H
#define MESSAGE_FACTORY_H

#include "message.h"

class MessageFactory {
public:
    /**
     * Construct a new Message of the specified type using the provided ingredients.
     * @param type The type of Message to construct (e.g. login_denied).
     * @param ingredients The ingredients necessary to be able to construct the Message. Contents vary for each message type.
     * @return The Message.
     */
    static std::unique_ptr<Message> makeMessage(std::string type, messageformat_t ingredients);

private:
    static std::unique_ptr<Message> makeLoginDenied(messageformat_t ingredients);
    static std::unique_ptr<Message> makeLoginGranted(messageformat_t ingredients);
    static std::unique_ptr<Message> makeUnauthorized(messageformat_t ingredients);
    static std::unique_ptr<Message> make
    static std::unique_ptr<Message> make
    static std::unique_ptr<Message> make
    static std::unique_ptr<Message> make
    static std::unique_ptr<Message> make
    static std::unique_ptr<Message> make
    static std::unique_ptr<Message> make
    static std::unique_ptr<Message> make
    static std::unique_ptr<Message> make

};

#endif //MESSAGE_FACTORY_H
