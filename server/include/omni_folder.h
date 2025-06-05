//
// Created by Christian Caamano on 5/16/25.
//

#ifndef OMNIFOLDER_H
#define OMNIFOLDER_H

#include "serializable.h"
#include "omni_drive.h"

/**
 * OmniFolder: Holds state and data related to a specific OmniFolder and manages folder-level logic,
 * including management of the exclusive edit lock.
 * Serialization: An OmniFolder is serializable to a config format object.
 */
class OmniFolder : Serializable<configformat_t> {
public:

    /*** --- SUPER CLASS OVERRIDES --- ***/

    /**
     * Serialize this OmniFolder to an equivalent configuration format object.
     * @return The resulting configuration format object.
     */
    configformat_t serialize() const override;

    /**
     * Initialize this OmniFolder from a configuration format object.
     * @param configFormatObj The configuration format object to use.
     */
    void deserialize(const configformat_t& configFormatObj) override;


    /*** --- CLASS METHODS --- ***/

    OmniFolder();
    ~OmniFolder() = default;

    /**
     * Get the ID of the drive currently holding the exclusive edit lock.
     * If no drive currently holds a lock, returns null.
     * @return The ID of the of the drive holding the edit lock, or null if the lock is free.
     */
    std::string getLockHolder() const;

    /**
     * Assign the edit lock to the specified drive.
     * @param driveID The ID of the specified drive.
     */
    void setLockHolder(std::string driveID);

    /**
     * Release the edit lock (setting the holder to null).
     */
    void releaseLock();

//    /**
//     *
//     * @return
//     */
//    const std::vector<OmniDrive>* getDrivesReadOnly() const;

    /**
     * Add a drive to this OmniFolder.
     * @param drive The OmniDrive to add.
     */
    void addDrive(std::unique_ptr<OmniDrive> drive);

    /**
     * Get the ID of this OmniFolder.
     * @return
     */
    std::string getID() const;

    /**
     * Gets the list of drives (read only).
     * @return The list of OmniDrive objects in this folder (read only).
     */
    std::vector<OmniDrive*> getDrivesConst() const;

private:

    /**
     * Unique identifier for this OmniFolder.
     */
    std::string id;

    /**
     * Name of this OmniFolder.
     */
    std::string name;

    /**
     * Stores the driveID of the drive that currently holds the exclusive edit lock.
     * If the lock has been released, this just stores "0".
     */
    std::string lockHolderId;

    /**
     * Stores references to the OmniDrives that are a part of this OmniFolder.
     */
    std::vector<std::unique_ptr<OmniDrive>> drives;

};

#endif //OMNIFOLDER_H
