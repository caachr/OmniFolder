//
// Created by Christian Caamano on 5/11/25.
//

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include "omni_network.h"
#include "defines.h"
#include "message_factory.h"
#include "fedex.h"
#include "port_authority.h"
#include "schema_validators/config_validator.h"
#include "hasher.h"
#include "github_portal.h"

#define UUID_SYSTEM_GENERATOR
#include "uuid.h"

class Message;

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
     * Initialize a new network and server with the given name, username, and password.
     * Create config and auth files and save the info there.
     * Intended to be called by the setup wizard when bringing a new network into existence.
     * NOTE: This is a lightweight method that only deals with files; it does not create instances of anything
     * within the program.
     * @param name The name of the network to create.
     * @param ipaddress The public IP address of the new server.
     * @param port The port number used for port forwarding.
     * @param username The username that will be used to log in to the server.
     * @param password The password that will be used to log in to the server.
     * @param ghubUsername The user's GitHub username.
     * @param ghubPrivatePAT The user's GitHub personal access token scoped to the recovery repos.
     */
    static void initNewNetwork(std::string& name, std::string& ipaddress, uint32_t port,
                               std::string& username, std::string& password,
                               std::string& ghubUsername, std::string& ghubPrivatePAT);

    /**
     * Recover an old network after a server has gone down by configuring a new server with the given public ip & port,
     * and old username & password.
     * Starts the server once set up.
     * @param ipaddress The public IP address of the new server.
     * @param port The port number used for port forwarding.
     * @param username The old username.
     * @param password The old password.
     * @param ghUsername The user's GitHub username.
     * @param ghPrivatePAT The user's private personal access token scoped to the loggedInClients list recovery repo.
     */
    static void replaceDestroyedServerAndStart(std::string& ipaddress, uint32_t port,
                                               std::string& username, std::string& password,
                                               std::string& ghUsername, std::string& ghServerPAT, std::string& ghClientPAT);

    /**
     * Returns a const pointer to the server singleton.
     * NOTE: There should never be a situation in which this method is called prior to creating the instance.
     * @return A const pointer to the server singleton.
     */
    static OmniServer* getInstance();

    /**
     * Singleton method: Checks whether or not the unique server instance has been created.
     * @return True if the server singleton instance exists, false if not.
     */
    static bool exists();

    /**
     * Indicates whether or not a server/network exists in the system.
     * Specifically: checks for the existence of config/auth files at their relative paths designated in defines.h;
     * if either file is found, returns false.
     * @return True if a config file or auth file exists, false otherwise.
     */
    static bool networkExists();

    /**
     * Loads everything (server, server info, network & structure) into program memory from config and auth files.
     * Assumes no instances of anything exist yet.
     * Intended to be called just before starting the server loop.
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
//    /**
//     * Completely destroys the server and network and constructs brand new ones using the config file.
//     */
//    static void wipeAndReload();

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
     * Print a brief summary about the network.
     */
    void printInfo() const;

    /**
     * Print comprehensive information about the server and its network.
     */
    void printInfoVerbose() const;

    /**
     *
     * @param message
     */
    void handleMessage(const Message& message);

private:
    /**
     * (Singleton helper method) \n
     * Creates the server singleton. Creates the OmniNetwork singleton and links it to the server singleton.
     * NOTE: creates just an empty network with basic fields (name and auth hash) this point.
     * @param omniNetworkName The name of the OmniNetwork being created.
     * @param authHash Encrypted credentials hash used to compare login attempts against.
     */
    static void createInstance(std::string& omniNetworkName, std::string& authHash);

    /**
     * (Singleton helper method) \n
     * Creates the OmniServer object and the blank OmniNetwork object
     * and assigns the network to the omniNetwork pointer in this class.
     * @param omniNetworkName The name of the OmniNetwork being created.
     * @param authHash Username + password hash to compare login attempts against.
     */
    OmniServer(std::string& omniNetworkName, std::string& authHash);

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
     * Saves the state of the OmniNetwork associated with this server to the local config file and those of all the
     * drives in the network.
     */
    void saveConfig();

    /**
     * Populates the entire local server/network program structure with the information from the local config file.
     * Assumes server/network instances exist.
     */
    void loadConfig();

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
     * UUID of this server (used for server identity verification client-side).
     */
    std::string uuid;

    /**
     * Pointer to the one and only OmniNetwork associated with this server.
     */
    OmniNetwork* omniNetwork;

    /**
     * Encrypted credentials hash to compare login attempts against.
     */
    std::string authHash;

    /**
     * The IP address of the server's host machine.
     */
    std::string ipaddress;

    /**
     * The port number used for port forwarding.
     */
    uint32_t port;

    /**
     * Stores the UUIDs and mailing addresses of all the clients that are currently logged in to the server.
     */
    std::unordered_map<std::string, nlohmann::json> loggedInClients;
};

#endif //SERVER_H
