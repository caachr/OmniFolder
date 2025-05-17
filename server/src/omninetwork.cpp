//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/omninetwork.h"

void OmniNetwork::createInstance(std::string& omniNetworkName)
{
    if (!instance)
    {
        instance = new OmniNetwork(omniNetworkName);
    }
    else {
        std::cout << "Cannot create instance; network already exists.\n";
    }
}

OmniNetwork* OmniNetwork::getInstance()
{
    return instance;
}

OmniNetwork::OmniNetwork(std::string& networkName)
    : omniNetworkName()
    , omniNetworkID(0)
    , omniFolders(nullptr)
{}

OmniNetwork::~OmniNetwork()
{
    delete omniFolders;
}

void OmniNetwork::fromJSON(const json& jsonObj)
{
    // TODO
}

json OmniNetwork::toJSON() const
{
    // TODO
}