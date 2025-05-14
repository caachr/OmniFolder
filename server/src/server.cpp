//
// Created by Christian Caamano on 5/11/25.
//

#include "../include/server.h"

/* --- Singleton logic --- */

OServer* OServer::instance = nullptr;

void OServer::createInstance(std::string networkName, std::string username, std::string password)
{
    if (!instance)
    {
        instance = new OServer(networkName, username, password);
    }
    else {
        std::cout << "Server already exists.\n";
    }
}

OServer& OServer::getInstance()
{
    return *instance;
}

bool OServer::exists()
{
    return !(instance == nullptr);
}

OServer::OServer(std::string networkName, std::string username, std::string password)
    : networkName(networkName)
    , username(username)
    , password(password)
{
    std::cout << "OServer created!\n";
}

OServer::~OServer()
{
    std::cout << "OServer destroyed.\n";
}

/* --- Business logic --- */

std::string& OServer::getNetworkName()
{
    return networkName;
}

bool OServer::validateCredentials(std::string username, std::string password)
{
    if (username != this->username || password != this->password) return false;
    return true;
}

void OServer::start()
{
    std::cout << "Starting server...\n";
    // start server shit
    std::cout << "Server started. Type 'stop' to end session and quit the application.\n"
                 "Note: The OmniFolder server is meant to run continuously on an 'always-on' machine. "
                 "It is recommended to only stop the session if absolutely necessary, as an OmniFolder cannot be edited while the server is offline.\n";
}