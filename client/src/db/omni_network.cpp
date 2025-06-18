//
// Created by Christian Caamano on 6/6/25.
//

#include "omni_network.h"

OmniNetwork::OmniNetwork(QObject *parent)
    : QObject(parent)
{

}

void OmniNetwork::loadFromConfigJson(const nlohmann::json& configInfo)
{
    // Load uuid & name
    uuid = QString::fromStdString(configInfo["network"]["uuid"].get<std::string>());
    name = QString::fromStdString(configInfo["network"]["name"].get<std::string>());

    // Load folders
    nlohmann::json foldersJson = configInfo["folders"];
    for (const auto& folderJson : foldersJson) {
        folders.emplace_back(std::make_unique<OmniFolder>(folderJson, this));
    }
}

QString OmniNetwork::getName() const
{
    return name;
}

std::vector<OmniFolder*> OmniNetwork::getFolders() const
{
    std::vector<OmniFolder*> result;
    result.reserve(folders.size());

    for (const auto& folder : folders) {
        result.push_back(folder.get());
    }

    return result;
}
