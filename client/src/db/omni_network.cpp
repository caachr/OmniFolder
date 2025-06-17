//
// Created by Christian Caamano on 6/6/25.
//

#include "omni_network.h"
#include "core/config_manager.h"

OmniNetwork::OmniNetwork(QObject *parent)
    : QObject(parent)
{

}

void OmniNetwork::loadFromConfig()
{
    ConfigManager configManager;
    nlohmann::json config = configManager.getConfigFromFile();

    // Load uuid & name
    uuid = QString::fromStdString(config["network"]["uuid"].get<std::string>());
    name = QString::fromStdString(config["network"]["name"].get<std::string>());

    // Load folders
    nlohmann::json foldersJson = config["folders"];
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
