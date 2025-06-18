//
// Created by Christian Caamano on 6/6/25.
//

#include "omni_folder.h"

OmniFolder::OmniFolder(QObject *parent)
    : QObject(parent)
{

}

OmniFolder::OmniFolder(const nlohmann::json& configInfo, QObject* parent)
    : QObject(parent)
{
    id = QString::fromStdString(configInfo["id"].get<std::string>());
    name = QString::fromStdString(configInfo["name"].get<std::string>());
    lockHolderId = QString::fromStdString(configInfo["lock_holder_id"].get<std::string>());

    nlohmann::json drivesJson = configInfo["drives"];
    for (const auto& driveJson : drivesJson) {
        drives.emplace_back(std::make_unique<OmniDrive>(driveJson, this));
    }
}

QString OmniFolder::getName() const
{
    return name;
}

QString OmniFolder::getId() const
{
    return id;
}

std::vector<OmniDrive*> OmniFolder::getDrives() const
{
    std::vector<OmniDrive*> result;
    result.reserve(drives.size());

    for (const auto& drive : drives) {
        result.push_back(drive.get());
    }

    return result;
}
