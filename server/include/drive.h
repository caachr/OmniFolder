//
// Created by Christian Caamano on 5/16/25.
//

#ifndef DRIVE_H
#define DRIVE_H

#include <cstdint>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * OmniDrive: Holds state and data related to a specific OmniDrive and manages drive-level logic.
 */
class OmniDrive {
public:
    OmniDrive();
    ~OmniDrive();

    // Copy construct & assign serve no purpose; disabled
    OmniDrive(const OmniDrive&) = delete;
    OmniDrive& operator=(const OmniDrive&) = delete;

    /**
     * Initialize this OmniDrive from a json object.
     */
    void fromJSON(const json& jsonObj);

    /**
     * Convert this OmniDrive to an equivalent json object.
     * @return The json object.
     */
    json toJSON() const;

private:
    /**
     * User-defined nickname, as input in client-side app.
     */
    std::string driveNickname;

    /**
     * Unique identifier for this drive.
     */
    uint8_t driveID;

    /**
     * The path on the actual physical drive to this OmniFolder.
     */
    std::string pathToOmniFolder;

    /**
     * The type of drive this is: true = full mirror, false = partial mirror.
     * Full mirrors mirror the entire OmniFolder, while partial mirrors mirror only user-specified subdirectories.
     */
    bool driveType;
};

#endif //DRIVE_H
