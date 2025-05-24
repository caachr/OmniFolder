//
// Created by Christian Caamano on 5/11/25.
//

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include "omni_network.h"
#include "defines.h"
#include "message.h"

/**
 * OmniServer: Singleton; manages the state of the one and only OmniNetwork, manages access to it,
 * and 'serves' information and fulfills commands requested by the user.
 */
class OmniServer {
public:
    // No copy constructor or assignment operator in singletons
    OmniServer(const OmniServer&) = delete;
    OmniServer& operator=(const OmniServer&) = delete;

    /**
     * Creates the server singleton. Creates the OmniNetwork singleton and links it to this server
     * (creates just a blank network with a name at this point, no OmniFolders).
     * @param omniNetworkName The name of the OmniNetwork being created.
     * @param username Username used to log in to the server and gain access to do things.
     * @param password Password used to log in to the server and gain access to do things.
     */
    static void createInstance(std::string& omniNetworkName, std::string& username, std::string& password);

    /**
     * Returns a const pointer to the server singleton.
     * NOTE: There should never be a situation in which this method is called prior to creating the instance.
     * @return A const pointer to the server singleton.
     */
    static OmniServer* getInstance();

    /**
     * Checks whether or not the unique server instance has been created.
     * @return True if the server singleton instance exists, false if not.
     */
    static bool exists();

    /**
     * Create a server and network from the config file upon program startup.
     */
    static void boot();

//    /**
//     * Initializes the OmniNetwork associated with this server using a config file.
//     * @warning WARNING: If there is an existing OmniNetwork in the system, this operation will irreversibly overwrite it!
//     * This method should only be called in three instances:
//     * \n 1. By the "reload" command that clears the existing network and creates a new one in its place
//     * (leaving the server object itself unchanged), or
//     * \n 2. By the "recover network" wizard upon total server destruction, or
//     * \n 3. Upon program startup.
//     * @param pathToFile The path to the config file used to initialize the OmniNetwork.
//     */
    // Static because it loads shit for the server too, not just the network; needs to create everything from scratch upon boot
    /**
     * Completely destroys the server and network and constructs brand new ones using the config file.
     */
    static void wipeAndReload();

    /**
     * Saves the state of the OmniNetwork associated with this server to the local config file and those of all the
     * drives in the network.
     */
    void saveConfig();

    /**
     * Obtains a pointer to the OmniNetwork singleton.
     * @return A pointer to the OmniNetwork singleton.
     */
    OmniNetwork* getOmniNetwork();

    /**
     * Bring the server online.
     */
    void start() const;

    /**
     * TBD
     */
    void printInfo() const;

    /**
     * TBD
     */
    void printInfoVerbose() const;

    /**
     * TBD
     */
    void printStatus() const;

    /**
     * TBD
     */
    void printStatusAdvanced() const;

    /**
     *
     * @param message
     */
    void handleMessage(const Message& message);

private:
    /**
     * Private constructor: creates the OmniServer object and the blank OmniNetwork object
     * and assigns the network to the omniNetwork pointer in this class.
     * @param omniNetworkName The name of the OmniNetwork being created.
     * @param username Username used to log in to the server and gain access to do things.
     * @param password Password used to log in to the server and gain access to do things.
     */
    OmniServer(std::string& omniNetworkName, std::string& username, std::string& password);

    // Private destructor; just contains a debug message for now
    ~OmniServer();

    /**
     * Validate that the credentials entered match the ones stored.
     * @param username The username to check.
     * @param password The password to check.
     * @return True if the credentials are correct, false if either the username or password is incorrect.
     */
    bool validateCredentials(std::string& username, std::string& password) const;

    /**
     * Add an OmniFolder to this network according to a json description.
     * Intended to be used upon receiving an "addFolder" request from a client along with the folder description.
     * @param jsonObj The json representation of the folder to add, as received from a client.
     */
    void addFolder(const configformat_t& folderConfig);

    /**
     * Add a drive to the specified OmniFolder in this network according to a json description.
     * Intended to be used upon receiving an "addDrive" request from a client along with the folder ID and drive description.
     * @param folderID The omniFolderID of the OmniFolder this drive will be added to.
     * @param driveConfig Configuration information for the drive to add.
     */
    void addDrive(std::string& folderID, const configformat_t& driveConfig);

    /**
     * Push changes made to a particular drive to the rest of the drives in the network.
     * @param callerNetInfo The network information (IP address, port number) of the calling drive's host machine.
     * @param callingDriveID The ID of the drive pushing its changes.
     */
    void pushChanges(std::string folderID, const std::string& callingDriveID);

    /**
     * Pointer to server singleton instance
     */
    static OmniServer* instance;

    /**
     * Pointer to the one and only OmniNetwork associated with this server.
     */
    OmniNetwork* omniNetwork;

    // Username for server login (no current use yet)
    std::string username;

    // Password for server login (no current use yet)
    std::string password;
};

#endif //SERVER_H
