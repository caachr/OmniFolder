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
    static std::unique_ptr<Message> makeMessage(std::string type, nlohmann::json ingredients);

private:
    static std::unique_ptr<Message> makeLoginDenied(nlohmann::json ingredients);
    static std::unique_ptr<Message> makeLoginGranted(nlohmann::json ingredients);
    static std::unique_ptr<Message> makeUnauthorized(nlohmann::json ingredients);
    static std::unique_ptr<Message> makeConfigUpdate(nlohmann::json ingredients);
};

#endif //MESSAGE_FACTORY_H
