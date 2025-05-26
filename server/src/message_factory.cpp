//
// Created by Christian Caamano on 5/25/25.
//

#include "../include/message_factory.h"

std::unique_ptr<Message> MessageFactory::makeMessage(std::string type, messageformat_t ingredients)
{
    if (type == "login_denied") {
        return makeLoginDenied(ingredients);
    } else if (type == "login_granted") {
        return makeLoginGranted(ingredients);
    } else if (type == "unauthorized") {
        return makeUnauthorized(ingredients);
    }
}

std::unique_ptr<Message> MessageFactory::makeLoginDenied(messageformat_t ingredients)
{
    messageformat_t header;
    std::string type;
    messageformat_t payload;

    header["network_uuid"] = ingredients["original_header"]["network_uuid"];

    // This is a reply; original sender becomes receiver, original receiver (this server) becomes sender.
    header["sender_class"] = "server";
    header["sender_ip"] = ingredients["original_header"]["receiver_ip"];
    header["sender_port"] = ingredients["original_header"]["receiver_port"];

    header["receiver_class"] = "client";
    header["receiver_ip"] = ingredients["original_header"]["sender_ip"];
    header["receiver_port"] = ingredients["original_header"]["sender_port"];

    type = "login_denied";

    payload["details"] = "Login request denied; invalid username or password.";

    return std::unique_ptr<Message>(new Message(header, type, payload));
}

std::unique_ptr<Message> MessageFactory::makeLoginGranted(messageformat_t ingredients)
{
    messageformat_t header;
    std::string type;
    messageformat_t payload;

    header["network_uuid"] = ingredients["original_header"]["network_uuid"];

    // This is a reply; original sender becomes receiver, original receiver (this server) becomes sender.
    header["sender_class"] = "server";
    header["sender_application_uuid"] = "";     // Only client apps have a UUID
    header["sender_ip"] = ingredients["original_header"]["receiver_ip"];
    header["sender_port"] = ingredients["original_header"]["receiver_port"];

    header["receiver_class"] = "client";
    header["receiver_ip"] = ingredients["original_header"]["sender_ip"];
    header["receiver_port"] = ingredients["original_header"]["sender_port"];

    type = "login_granted";

    payload["details"] = "Login request granted.";

    return std::unique_ptr<Message>(new Message(header, type, payload));
}

std::unique_ptr<Message> MessageFactory::makeUnauthorized(messageformat_t ingredients)
{
    messageformat_t header;
    std::string type;
    messageformat_t payload;

    header["network_uuid"] = ingredients["original_header"]["network_uuid"];

    // This is a reply; original sender becomes receiver, original receiver (this server) becomes sender.
    header["sender_class"] = "server";
    header["sender_application_uuid"] = "";     // Only client apps have a UUID
    header["sender_ip"] = ingredients["original_header"]["receiver_ip"];
    header["sender_port"] = ingredients["original_header"]["receiver_port"];

    header["receiver_class"] = "client";
    header["receiver_ip"] = ingredients["original_header"]["sender_ip"];
    header["receiver_port"] = ingredients["original_header"]["sender_port"];

    type = "unauthorized";

    payload["details"] = "Error: client is not logged in and is therefore unauthorized to interact with the network. Please log in using the correct username and password.";

    return std::unique_ptr<Message>(new Message(header, type, payload));
}