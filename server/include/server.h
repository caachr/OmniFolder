//
// Created by Christian Caamano on 5/11/25.
//

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <nlohmann/json.hpp>
#include "omninetwork.h"

using json = nlohmann::json;

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
     * Clears the server instance from the system (destroys it).
     * TODO determine if this is actually needed or not
     */
    static void deleteInstance();

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
//    void loadNetworkFromFile(std::string_view& pathToFile);

    /**
     * Saves the state of the OmniNetwork associated with this server to the local config file and those of all the
     * drives in the network.
     */
    void saveNetworkConfig();

    /**
     * Validate that the credentials entered match the ones stored.
     * @param username The username to check.
     * @param password The password to check.
     * @return True if the credentials are correct, false if either the username or password is incorrect.
     */
    bool validateCredentials(std::string& username, std::string& password) const;


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
