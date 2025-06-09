//
// Created by Christian Caamano on 5/16/25.
//

#ifndef DRIVE_H
#define DRIVE_H

#include <cstdint>
#include <string>
#include <fstream>
#include "../include/defines.h"
#include "../include/serializable.h"

/**
 * OmniDrive: Holds state and data related to a specific OmniDrive and manages drive-level logic.
 * Serialization: An OmniDrive is serializable to a config format object.
 */
class OmniDrive : Serializable<configformat_t> {
public:

    /*** --- SUPER CLASS OVERRIDES --- ***/

    /**
     * Serialize this OmniDrive to an equivalent configuration format object.
     * @return The resulting configuration format object.
     */
    configformat_t serialize() const override;

    /**
     * Initialize this OmniDrive from a configuration format object.
     * @param configFormatObj The configuration format object to use.
     */
    void deserialize(const configformat_t& configFormatObj) override;


    /*** --- CLASS METHODS --- ***/

    OmniDrive();
    ~OmniDrive() = default;

    // Copy construct & assign serve no purpose; disabled
    OmniDrive(const OmniDrive&) = delete;
    OmniDrive& operator=(const OmniDrive&) = delete;

    /**
     * Get the ID of this OmniDrive.
     * @return The ID of this drive.
     */
    std::string getID() const;

    /**
     * Get the host IP of this OmniDrive.
     * @return The host IP address of this drive.
     */
    std::string getHost() const;

    /**
     * Get the port of this OmniDrive.
     * @return The port of this drive.
     */
    uint32_t getPort() const;

    /**
     * Set the status of this drive.
     * @param newStatus The new status of this drive.
     */
    void setStatus(const std::string& newStatus);

private:

    /**
     * Unique identifier for this drive.
     */
    std::string id;

    /**
     * User-defined nickname, as input in client-side app.
     */
    std::string name;

    /**
     * The type of drive this is: F = full mirror, P = partial mirror.
     * Full mirrors mirror the entire OmniFolder, while partial mirrors mirror only user-specified subdirectories.
     */
    unsigned char type;

    /**
     * The status of this drive: up-to-date or stale.
     * Drives are marked stale when they're offline during a push and do not receive the updated data.
     */
    std::string status;

    /**
     * The IP address of the host machine on which the drive is currently running.
     */
    std::string currHost;

    /**
     * The port number of the host machine on which the drive is currently running.
     */
    uint32_t port;

    /**
     * Reflects whether the drive is on/offline.
     */
    bool online;

    /**
     * Reflects the physical capacity of the drive, in bytes.
     */
    uint64_t capacity;

    /**
     * Reflects the amount of storage left in the drive, in bytes.
     */
    uint64_t used;

    /**
     * The path on the physical drive to the OmniFolder.
     */
    std::string path;

    /**
     * For partial drives only: the subdirectories to mirror.
     */
    std::vector<std::string> mirrorDirs;
};

#endif //DRIVE_H
