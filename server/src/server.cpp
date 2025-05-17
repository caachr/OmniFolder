//
// Created by Christian Caamano on 5/11/25.
//

#include "../include/server.h"

OmniServer* OmniServer::instance = nullptr;

void OmniServer::createInstance(std::string& omniNetworkName, std::string& username, std::string& password)
{
    if (!instance)
    {
        instance = new OmniServer(omniNetworkName, username, password);
    }
    else {
        std::cout << "Server already exists.\n";
    }
}

void OmniServer::deleteInstance()
{
    delete instance;
}

OmniServer* OmniServer::getInstance()
{
    if (instance == nullptr) throw std::runtime_error("Attempted to call getInstance on a null OmniServer.");
    return instance;
}

bool OmniServer::exists()
{
    return instance != nullptr;
}

void OmniServer::saveNetworkConfig()
{
    // Local config save
    json j = omniNetwork->toJSON();
    std::ofstream o("config.json");
    if (o.is_open()) {
        o << j.dump(4);
        o.close();
    } else {
        std::cout << "Error opening local config file.\n";
    }

    // Update config on all the drives (TODO)

}

OmniServer::OmniServer(std::string& omniNetworkName, std::string& username, std::string& password)
    : username(username)
    , password(password)
{
    // Create blank OmniNetwork object and get its pointer
    OmniNetwork::createInstance(omniNetworkName);
    omniNetwork = OmniNetwork::getInstance();
}

OmniServer::~OmniServer()
{
    std::cout << "OmniServer destroyed.\n";
}

bool OmniServer::validateCredentials(std::string& username, std::string& password) const
{
    if (username != this->username || password != this->password) return false;
    return true;
}

OmniNetwork* OmniServer::getOmniNetwork()
{
    return omniNetwork;
}

void OmniServer::start() const
{
    std::cout << "Starting server...\n";
    // start server shit
    std::cout << "Server started. Type 'stop' to end session and quit the application.\n"
                 "Note: The OmniFolder server is meant to run continuously on an 'always-on' machine. "
                 "It is recommended to only stop the session if absolutely necessary, as an OmniFolder cannot be edited while the server is offline.\n";
}

void OmniServer::printInfo() const
{
    std::cout << "nothing to see here yet...\n";
}

void OmniServer::printInfoVerbose() const
{
    std::cout << "nothing to see here yet...\n";
}

void OmniServer::printStatus() const
{
    std::cout << "nothing to see here yet...\n";
}

void OmniServer::printStatusAdvanced() const
{
    std::cout << "nothing to see here yet...\n";
}