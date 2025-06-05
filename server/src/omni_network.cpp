//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/omni_network.h"

configformat_t OmniNetwork::serialize() const
{
    configformat_t configObj;

    configObj["uuid"] = uuid;
    configObj["name"] = name;

    configformat_t foldersConfigObj;
    for (const auto& folder : folders) {
        foldersConfigObj.push_back(folder->serialize());
    }
    configObj["folders"] = foldersConfigObj;

    return configObj;
}

void OmniNetwork::deserialize(const configformat_t& configObj)
{
    uuid = configObj["uuid"];
    name = configObj["name"];

    folders.clear();
    for (const auto& folderConfig : configObj["folders"]) {
        auto folder = std::make_unique<OmniFolder>();
        folder->deserialize(folderConfig);
        folders.push_back(std::move(folder));
    }
}

OmniNetwork::~OmniNetwork()
{
    instance = nullptr;
}

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

void OmniNetwork::addFolder(std::unique_ptr<OmniFolder> folder)
{
    folders.push_back(std::move(folder));
}

std::string OmniNetwork::getUUID() const
{
    return uuid;
}

OmniFolder* OmniNetwork::getFolderByID(const std::string& folderID)
{
    for (const auto& folder : folders) {
        if (folder->getID() == folderID) {
            return folder.get();
        }
    }
    return nullptr;
}

std::vector<OmniFolder *> OmniNetwork::getFoldersConst() const
{
    std::vector<OmniFolder*> returnObj;
    returnObj.reserve(folders.size());
    for (const auto& folder : folders) {
        returnObj.push_back(folder.get());
    }
    return returnObj;
}

OmniNetwork::OmniNetwork(std::string& networkName)
        : uuid()
        , name(networkName)
        , folders()
{}