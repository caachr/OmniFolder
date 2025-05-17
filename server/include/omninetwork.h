//
// Created by Christian Caamano on 5/16/25.
//

#ifndef OMNINETWORK_H
#define OMNINETWORK_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "omnifolder.h"

using json = nlohmann::json;

/**
 * OmniNetwork: Singleton - the one and only OmniNetwork associated with the one and only OmniServer.
 * Holds state and data related to this OmniNetwork and manages network-level logic.
 */
class OmniNetwork {
public:
    // No copy constructor or assignment operator in singletons
    OmniNetwork(const OmniNetwork&) = delete;
    OmniNetwork& operator=(const OmniNetwork&) = delete;

    /**
     * Creates the network singleton (creates just a blank network with a name at this point, no OmniFolders).
     * @param omniNetworkName The name of the OmniNetwork being created.
     */
    static void createInstance(std::string& omniNetworkName);

    /**
     * Returns a pointer to the network singleton.
     * NOTE: There should never be a situation in which this method is called prior to creating the instance.
     * @return A pointer to the network singleton.
     */
    static OmniNetwork* getInstance();

    /**
     * Initialize this OmniNetwork from a json object.
     */
    void fromJSON(const json& jsonObj);

    /**
     * Convert this OmniNetwork to an equivalent json object.
     * @return The json object.
     */
    json toJSON() const;

private:
    /**
     * Private constructor: creates the OmniNetwork singleton object, gives it a name, and initializes all other fields to blank (0/nullptr).
     * @param networkName The name of the OmniNetwork being created.
     */
    OmniNetwork(std::string& networkName);

    /**
     * Private destructor; clears the OmniFolder objects from the system before destroying itself.
     */
    ~OmniNetwork();

    /**
     * Pointer to the network singleton instance
     */
    static OmniNetwork* instance;

    /**
     * Name for this OmniNetwork.
     */
    std::string omniNetworkName;

    /**
     * Unique identifier for this OmniNetwork.
     */
    uint8_t omniNetworkID;

    /**
     * Stores references to the OmniFolders that are a part of this OmniNetwork.
     */
    std::vector<OmniFolder>* omniFolders;
};

#endif //OMNINETWORK_H
