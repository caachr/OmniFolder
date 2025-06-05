//
// Created by Christian Caamano on 6/3/25.
//

#include "github_portal.h"

GitHubPortal::repo = "OmniFolder Server Client List Backup";
GitHubPortal::branch = "master";
GitHubPortal::path = "client-list-recovery.json";

std::unordered_map<std::string, nlohmann::json>
GitHubPortal::fetchClientListBackup(std::string& user,
                              std::string& pat,
                              std::string& repo,
                              std::string& branch,
                              std::string& path) {
//    std::string fileContents;
//    nlohmann::json fileContentsJson;
//    std::unordered_map<std::string, nlohmann::json> result;
//
//    // TODO github shit - get file "client-list-recovery.json" from repo "OmniFolder - Client List Recovery"
//
//    // Get number of clients
//    unsigned int numClients = fileContentsJson.size() / 2;
//
//    for (int i = 0; i < numClients; ++i) {
//        result[fileContentsJson[std::to_string(i) + "_uuid"]] = fileContentsJson[std::to_string(i) + "_details"];
//    }
//
//    return result;

    // Placeholder
    std::unordered_map<std::string, nlohmann::json> placeholderMap;
    nlohmann::json placeholderJson;
    placeholderJson["shit_key"] = "shit_value";
    placeholderMap["placeholder_uuid"] = placeholderJson;
    return placeholderMap;
}

void GitHubPortal::writeClientListBackup(nlohmann::json& clientList,
                                     std::string& user,
                                     std::string& pat,
                                     std::string& repo,
                                     std::string& branch,
                                     std::string& path)
{
    // TODO
}