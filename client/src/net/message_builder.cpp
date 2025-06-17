#include <QDebug>

#include "message_builder.h"

Message* MessageBuilder::makeMessage(const MessageType type, const nlohmann::json ingredients)
{
    switch (type) {
    case MessageType::AuthRequest:
        return makeAuthRequest(ingredients);
        break;
    case MessageType::CreateFolderRequest:
        return makeCreateFolderRequest(ingredients);
        break;
    case MessageType::EditFolderRequest:
        return makeEditFolderRequest(ingredients);
        break;
    case MessageType::PushChangesRequest:
        return makePushChangesRequest(ingredients);
        break;
    case MessageType::RemoveFolderRequest:
        return makeRemoveFolderRequest(ingredients);
        break;
    default:
        throw std::runtime_error("MessageBuilder makeMessage: attempted to make unrecognized or unsupported message type.");
    }
}

Message* MessageBuilder::makeAuthRequest(const nlohmann::json ingredients)
{
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;

    header["sender"]["class"] = "client";
    header["sender"]["uuid"] = ingredients["client_uuid"].get<std::string>();
    header["sender"]["host"] = ingredients["client_host"].get<std::string>();

    header["receiver"]["class"] = "server";
    header["receiver"]["uuid"] = ingredients["server_uuid"].get<std::string>();
    header["receiver"]["host"] = ingredients["server_host"].get<std::string>();

    type = MessageType::AuthRequest;

    payload["details"] = "Requesting to log in to the network with the enclosed username and password.";
    payload["username"] = ingredients["username"].get<std::string>();
    payload["password"] = ingredients["password"].get<std::string>();

    return new Message(header, type, payload);
}

Message* MessageBuilder::makeCreateFolderRequest(const nlohmann::json ingredients)
{
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;

    header["sender"]["class"] = "client";
    header["sender"]["uuid"] = ingredients["client_uuid"].get<std::string>();
    header["sender"]["host"] = ingredients["client_host"].get<std::string>();

    header["receiver"]["class"] = "server";
    header["receiver"]["uuid"] = ingredients["server_uuid"].get<std::string>();
    header["receiver"]["host"] = ingredients["server_host"].get<std::string>();

    type = MessageType::CreateFolderRequest;

    payload["details"] = "Requesting to create a new folder.";

    // other payload shit


    return new Message(header, type, payload);
}

Message* MessageBuilder::makeEditFolderRequest(const nlohmann::json ingredients)
{
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;

    header["sender"]["class"] = "client";
    header["sender"]["uuid"] = ingredients["client_uuid"].get<std::string>();
    header["sender"]["host"] = ingredients["client_host"].get<std::string>();

    header["receiver"]["class"] = "server";
    header["receiver"]["uuid"] = ingredients["server_uuid"].get<std::string>();
    header["receiver"]["host"] = ingredients["server_host"].get<std::string>();

    type = MessageType::EditFolderRequest;

    payload["details"] = "Requesting to edit a folder.";

    // other payload shit


    return new Message(header, type, payload);
}

Message* MessageBuilder::makePushChangesRequest(const nlohmann::json ingredients)
{
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;

    header["sender"]["class"] = "client";
    header["sender"]["uuid"] = ingredients["client_uuid"].get<std::string>();
    header["sender"]["host"] = ingredients["client_host"].get<std::string>();

    header["receiver"]["class"] = "server";
    header["receiver"]["uuid"] = ingredients["server_uuid"].get<std::string>();
    header["receiver"]["host"] = ingredients["server_host"].get<std::string>();

    type = MessageType::PushChangesRequest;

    payload["details"] = "Requesting to push local changes to the network.";

    // other payload shit


    return new Message(header, type, payload);
}

Message* MessageBuilder::makeRemoveFolderRequest(const nlohmann::json ingredients)
{
    nlohmann::json header;
    MessageType type;
    nlohmann::json payload;

    header["sender"]["class"] = "client";
    header["sender"]["uuid"] = ingredients["client_uuid"].get<std::string>();
    header["sender"]["host"] = ingredients["client_host"].get<std::string>();

    header["receiver"]["class"] = "server";
    header["receiver"]["uuid"] = ingredients["server_uuid"].get<std::string>();
    header["receiver"]["host"] = ingredients["server_host"].get<std::string>();

    type = MessageType::RemoveFolderRequest;

    payload["details"] = "Requesting to remove a folder from the network.";

    // other payload shit
    // maybe i shouldn't have ability for clients to remove folders?
    // or could just prompt for username & password again...
    // risky operation either way, maybe just introduce a lot of strict mechanisms / friction to performing it


    return new Message(header, type, payload);
}
