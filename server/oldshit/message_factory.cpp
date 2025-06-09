//
// Created by Christian Caamano on 5/25/25.
//

#include "../include/message_factory.h"

std::unique_ptr<Message> MessageFactory::makeMessage(std::string type, nlohmann::json ingredients)
{
    if (type == "login_denied") {
        return makeLoginDenied(ingredients);
    } else if (type == "login_granted") {
        return makeLoginGranted(ingredients);
    } else if (type == "unauthorized") {
        return makeUnauthorized(ingredients);
    } else if (type == "config_update") {
        return makeConfigUpdate(ingredients);
    } else {
        return nullptr;
    }
}

std::unique_ptr<Message> MessageFactory::makeLoginDenied(nlohmann::json ingredients)
{
    nlohmann::json header;
    std::string type;
    nlohmann::json payload;

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

std::unique_ptr<Message> MessageFactory::makeLoginGranted(nlohmann::json ingredients)
{
    nlohmann::json header;
    std::string type;
    nlohmann::json payload;

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

std::unique_ptr<Message> MessageFactory::makeUnauthorized(nlohmann::json ingredients)
{
    nlohmann::json header;
    std::string type;
    nlohmann::json payload;

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

std::unique_ptr<Message> MessageFactory::makeConfigUpdate(nlohmann::json ingredients)
{
    nlohmann::json header;
    std::string type;
    nlohmann::json payload;

    header["network_uuid"] = ingredients["network_uuid"];

    header["sender_class"] = "server";
    header["sender_app_uuid"] = ingredients["server_uuid"];
    header["sender_ip"] = ingredients["server_ip"];
    header["sender_port"] = ingredients["server_port"];

    header["receiver_class"] = "client";
    header["receiver_ip"] = ingredients["drive_ip"];
    header["receiver_port"] = ingredients["drive_port"];

    type = "config_update";

    payload["config"] = ingredients["config"];

    return std::unique_ptr<Message>(new Message(header, type, payload));
}