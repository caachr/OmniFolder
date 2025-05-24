//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/omni_folder.h"

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

OmniFolder::OmniFolder()
    : id(0)
    , name()
    , drives()
    , lockHolderId()
{}

std::string OmniFolder::getLockHolder() const
{
    return lockHolderId;
}

void OmniFolder::setLockHolder(std::string driveId)
{
    lockHolderId = driveId;
}

void OmniFolder::releaseLock()
{
    lockHolderId = "";
}

//const std::vector<OmniDrive>* OmniFolder::getDrivesReadOnly() const
//{
//    return drives;
//}

void OmniFolder::addDrive(std::unique_ptr<OmniDrive> drive)
{
    drives.push_back(std::move(drive));
}

std::string OmniFolder::getId() const
{
    return id;
}