//
// Created by Christian Caamano on 5/16/25.
//

#ifndef OMNINETWORK_H
#define OMNINETWORK_H

#include <iostream>
#include "serializable.h"
#include "omni_folder.h"
#include "defines.h"

/**
 * OmniNetwork: Singleton - the one and only OmniNetwork associated with the one and only OmniServer.
 * Holds state and data related to this OmniNetwork and manages network-level logic.
 * Serialization: An OmniNetwork is serializable to a configuration format object.
 */
class OmniNetwork : Serializable<configformat_t> {
public:

    /*** --- SUPER CLASS OVERRIDES --- ***/

    /**
     * Serialize this OmniNetwork to an equivalent configuration format entity.
     * @return The resulting configuration format entity.
     */
    configformat_t serialize() const override;

    /**
     * Initialize this OmniNetwork from a configuration format entity.
     * @param configFormatEntity The configuration format entity to use.
     */
    void deserialize(const configformat_t& configFormatEntity) override;


    /*** --- CLASS METHODS --- ***/

    // No copy constructor or assignment operator in singletons
    OmniNetwork(const OmniNetwork&) = delete;
    OmniNetwork& operator=(const OmniNetwork&) = delete;

    /**
     * Public destructor: clears the OmniFolder objects from the system, then destroys the network singleton instance.
     * Intended to be called by the server when it is destroyed.
     */
    ~OmniNetwork();

    /**
     * Creates the network singleton (creates just a blank network with a name at this point, no OmniFolders).
     * @param name The name of the OmniNetwork being created.
     */
    static void createInstance(std::string& name);

    /**
     * Returns a pointer to the network singleton.
     * NOTE: There should never be a situation in which this method is called prior to creating the instance.
     * @return A pointer to the network singleton.
     */
    static OmniNetwork* getInstance();

    /**
     * Add an OmniFolder to this network.
     * @param folder The folder to add.
     */
    void addFolder(std::unique_ptr<OmniFolder> folder);

//    /**
//     * Get the OmniFolder object in this network by its ID.
//     * @param folderID The ID of the OmniFolder to get.
//     * @return A pointer to the OmniFolder.
//     */
//    OmniFolder* getFolderByID(const std::string& folderID);

private:
    /**
     * Private constructor: creates the OmniNetwork singleton object, gives it a name, and initializes all other fields to blank (0/nullptr).
     * @param name The name of the OmniNetwork being created.
     */
    OmniNetwork(std::string& name);

    /**
     * Pointer to the network singleton instance
     */
    static OmniNetwork* instance;

    /**
     * Unique identifier for this OmniNetwork.
     */
    std::string uuid;

    /**
     * Name for this OmniNetwork.
     */
    std::string name;

//    std::string serverHost;
//
//    std::string serverPort;

    /**
     * Stores references to the OmniFolders that are a part of this OmniNetwork.
     * Key value pairs are id, pointer to corresponding OmniFolder object.
     */
    std::vector<std::unique_ptr<OmniFolder>> folders;
};

#endif //OMNINETWORK_H
