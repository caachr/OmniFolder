//
// Created by Christian Caamano on 5/16/25.
//

#ifndef OMNIFOLDER_H
#define OMNIFOLDER_H

#include "drive.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * OmniFolder: Holds state and data related to a specific OmniFolder and manages folder-level logic,
 * including management of the exclusive edit lock.
 */
class OmniFolder {
public:
    OmniFolder();
    ~OmniFolder();

    /**
     * Initialize this OmniFolder from a json object.
     */
    void fromJSON(const json& jsonObj);

    /**
     * Convert this OmniFolder to an equivalent json object.
     * @return The json object.
     */
    json toJSON() const;

private:
    /**
     * Nickname for this OmniFolder.
     */
    std::string omniFolderNickname;

    /**
     * Unique identifier for this OmniFolder.
     */
    uint8_t omniFolderID;

    /**
     * Stores references to the drives that are a part of this OmniFolder.
     */
    std::vector<OmniDrive>* drives;

    /**
     * Stores the driveID of the drive that currently holds the exclusive edit lock.
     * If the lock has been released, this just stores "0".
     */
    std::string lockHolder;
};

#endif //OMNIFOLDER_H
