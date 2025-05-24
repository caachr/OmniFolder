//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/omni_network.h"

configformat_t OmniFolder::serialize() const
{
    configformat_t configObj;

    configObj["id"] = id;
    configObj["name"] = name;
    configObj["lock_holder_id"] = lockHolderId;

    configformat_t drivesConfigObj;
    for (const auto& drive : drives) {
        drivesConfigObj.push_back(drive->serialize());
    }
    configObj["drives"] = drivesConfigObj;

    return configObj;
}

void OmniFolder::deserialize(const configformat_t& configObj)
{
    id = configObj["id"];
    name = configObj["name"];
    lockHolderId = configObj["lock_holder_id"];

    drives.clear();
    for (const auto& driveConfig : configObj["drives"]) {
        auto drive = std::make_unique<OmniDrive>();
        drive->deserialize(driveConfig);
        drives.push_back(std::move(drive));
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

OmniNetwork::OmniNetwork(std::string& networkName)
    : id()
    , name(networkName)
    , folders()
{}

void OmniNetwork::addFolder(std::unique_ptr<OmniFolder> folder)
{
    folders.push_back(std::move(folder));
}

//OmniFolder* OmniNetwork::getFolderByID(const std::string& folderID)
//{
//    return (omniFolders.find(folderID))->second.get();
//}