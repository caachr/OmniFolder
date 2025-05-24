//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/omni_drive.h"

configformat_t OmniDrive::serialize() const
{
    configformat_t configObj;

    configObj["id"] = id;
    configObj["name"] = name;
    configObj["type"] = type;
    configObj["curr_host"] = currHost;
    configObj["port"] = port;
    configObj["capacity_bytes"] = capacity;
    configObj["used_bytes"] = used;
    configObj["path"] = path;

    return configObj;
}

void OmniDrive::deserialize(const configformat_t& configObj)
{
    id = configObj["id"];
    name = configObj["name"];
    type = configObj["type"];
    currHost = configObj["curr_host"];
    port = configObj["port"];
    capacity = configObj["capacity_bytes"];
    used = configObj["used_bytes"];
    path = configObj["path"];

    if (type == 'P') {
        mirrorDirs = configObj.at("mirror_dirs").get<std::vector<std::string>>();
    }
}

OmniDrive::OmniDrive()
    : id()
    , name()
    , type()
    , currHost()
    , port(0)
    , online(false)
    , capacity(0)
    , used(0)
    , path()
    , mirrorDirs()
{}

std::string OmniDrive::getId() const
{
    return id;
}