//
// Created by Christian Caamano on 6/6/25.
//

#include "omni_drive.h"

OmniDrive::OmniDrive(QObject *parent)
    : QObject(parent)
{

}

OmniDrive::OmniDrive(nlohmann::json configInfo, QObject* parent)
    : QObject(parent)
{
    id = QString::fromStdString(configInfo["id"].get<std::string>());
    name = QString::fromStdString(configInfo["name"].get<std::string>());
    type = QString::fromStdString(configInfo["type"].get<std::string>());
    status = QString::fromStdString(configInfo["status"].get<std::string>());
    currentHost = QString::fromStdString(configInfo["curr_host"].get<std::string>());
    online = configInfo["online"].get<bool>();
    capacityBytes = configInfo["capacity_bytes"];
    usedBytes = configInfo["used_bytes"];
    path = QString::fromStdString(configInfo["local_path"].get<std::string>());

    if (type == "P") {
        nlohmann::json mirrorDirsJson = configInfo["mirror_dirs"];
        for (const auto& mirrorDirJson : mirrorDirsJson) {
            QString mirrorDir = QString::fromStdString(mirrorDirJson.get<std::string>());
            mirrorDirs.push_back(mirrorDir);
        }
    }
}

QString OmniDrive::getName() const
{
    return name;
}

QString OmniDrive::getId() const
{
    return id;
}

QString OmniDrive::getType() const
{
    return type;
}
