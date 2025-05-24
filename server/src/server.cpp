//
// Created by Christian Caamano on 5/11/25.
//

#include "../include/server.h"
#include "../include/socket_master.h"

OmniServer* OmniServer::instance = nullptr;

void OmniServer::createInstance(std::string& omniNetworkName, std::string& username, std::string& password)
{
    if (!instance)
    {
        instance = new OmniServer(omniNetworkName, username, password);
    }
    else {
        std::cout << "Server already exists.\n";
    }
}

OmniServer* OmniServer::getInstance()
{
    if (instance == nullptr) throw std::runtime_error("Attempted to call getInstance on a null OmniServer.");
    return instance;
}

bool OmniServer::exists()
{
    return instance != nullptr;
}

void OmniServer::boot()
{
    std::ifstream c(CONFIG_FILE_RELATIVE_PATH);
    configformat_t configObj;
    if (c.is_open()) {
        std::ifstream a(AUTH_CREDS_RELATIVE_PATH);
        nlohmann::json authObj;
        if (a.is_open()){
            std::string networkName;
            std::string username;
            std::string password;

            // Extract network name from file
            c >> configObj;
            networkName = configObj.find("networkName").key();

            // Extract username & password from authentication file
            a >> authObj;
            username = authObj["username"];
            password = authObj["password"];

            // Create server singleton & network
            createInstance(networkName, username, password);

            // Load config info into network instance
            OmniNetwork::getInstance()->deserialize(configObj);
        } else {
            throw std::runtime_error("Error: failed to open authentication file during boot. No changes made.\n");
        }
    } else {
        throw std::runtime_error("Error: failed to open config file during boot. No changes made.\n");
    }
}

void OmniServer::wipeAndReload()
{
    std::ifstream i(CONFIG_FILE_RELATIVE_PATH);
    configformat_t configObj;
    if (i.is_open()) {
        delete instance;

        std::string networkName;
        std::string username;
        std::string password;

        // Load config file into config object
        i >> configObj;

        // Extract the network name
        networkName = configObj.find("networkName").key();

        // Create server singleton & network
        createInstance(networkName, username, password);

        // TODO populate with config file ???
    }
    else {
        throw std::runtime_error("Error: failed to open config file during wipeAndReplace. No changes made.\n");
    }
}

void OmniServer::saveConfig()
{
    // Local config save
    configformat_t configObj = omniNetwork->serialize();
    std::ofstream o(CONFIG_FILE_RELATIVE_PATH);
    if (o.is_open()) {
        o << configObj.dump(4);
        o.close();
    } else {
        std::cout << "Error opening local config file when attempting save.\n";
    }

    // TODO Update config file on all the drives

}

OmniServer::OmniServer(std::string& omniNetworkName, std::string& username, std::string& password)
    : username(username)
    , password(password)
{
    // Create blank OmniNetwork object and get its pointer
    OmniNetwork::createInstance(omniNetworkName);
    omniNetwork = OmniNetwork::getInstance();
}

OmniServer::~OmniServer()
{
    delete omniNetwork;
    omniNetwork = nullptr;

    instance = nullptr;
}

OmniNetwork* OmniServer::getOmniNetwork()
{
    return omniNetwork;
}

void OmniServer::start() const
{
    std::cout << "Starting server...\n";

    SocketMaster::open();

    std::cout << "Server started. Type 'stop' to end session and quit the application.\n"
                 "Note: The OmniFolder server is meant to run continuously on an 'always-on' machine. "
                 "It is recommended to only stop the session if absolutely necessary, as an OmniFolder cannot be edited while the server is offline.\n";
}

void OmniServer::printInfo() const
{
    // TODO
}

void OmniServer::printInfoVerbose() const
{
    // TODO
}

void OmniServer::printStatus() const
{
    // TODO
}

void OmniServer::printStatusAdvanced() const
{
    // TODO
}

void OmniServer::handleMessage(const Message& message)
{
    if (message.getType() == "areyouthere") {
        // Send response to client: "I'm online!"
    }
    if (message.getType() == "acquireLockRequest") {
        std::string folderID;
        std::string driveID;

        OmniFolder* folder = OmniNetwork::getInstance()->getFolderByID(folderID);
        if (folder->getLockHolder() != "0") {
            // Lock isn't free; send response to client: "DENY, msg: Failed to request lock, as it is already held by an existing drive."
        } else {
            // Lock is free, grant it
            folder->setLockHolder(driveID);
            // Send response to client: "GRNT, msg: Lock request granted."
            saveConfig();
        }
    }
    if (message.getType() == "releaseLockRequest") {
        std::string folderID;

        OmniFolder* folder = OmniNetwork::getInstance()->getFolderByID(folderID);
        if (folder->getLockHolder() == "0") {
            // Send response to client: "DENY, msg: Release request failed, as no drive currently holds a lock."
        } else {
            // Lock is held, release it
            folder->releaseLock();
            // Send response to client: "GRNT, msg: Release request granted."
            saveConfig();
        }
    }
    if (message.getType() == "pushRequest") {
        std::string folderID;
        std::string callingDriveID;

        pushChanges(folderID, callingDriveID);
    }
    if (message.getType() == "addFolderRequest") {
        const configformat_t& folderConfig(message.getPayload());
        addFolder(folderConfig);
    }
    if (message.getType() == "addDriveRequest") {
        std::string folderID;
        const configformat_t& driveConfig(message.getPayload());
        addDrive(folderID, driveConfig);
    }
    if (message.getType() == "loginRequest") {
        std::string usernameArg;
        std::string passwordArg;

        if (!validateCredentials(usernameArg, passwordArg)) {
            // Send response to client: "DENY, msg: Incorrect username or password."
        } else {
            // Send response to client: "GRNT, msg: Login request granted."
        }
    }
}

bool OmniServer::validateCredentials(std::string& usernameArg, std::string& passwordArg) const
{
    if (usernameArg != username || passwordArg != password) return false;
    return true;
}

void OmniServer::addFolder(const configformat_t& folderConfig)
{
    std::unique_ptr<OmniFolder> folder;
    folder->deserialize(folderConfig);

    omniNetwork->addFolder(std::move(folder));
    saveConfig();
}

void OmniServer::addDrive(std::string& folderID, const configformat_t& driveConfig)
{
    auto* drive = new OmniDrive();
    drive->fromConfigObj(driveConfig);

    omniNetwork->getFolderByID(folderID)->addDrive(drive);
    saveConfig();
}

void OmniServer::pushChanges(std::string folderID, const std::string& callingDriveID)
{
    OmniFolder* folder = OmniNetwork::getInstance()->getFolderByID(folderID);
    auto* drives = folder->getDrivesReadOnly();
    for (const OmniDrive& drive : *drives) {
        if (drive.getID() != callingDriveID) {
            // Send message to client: "Here's everything you need to connect to this drive. Go ahead and connect and send him some data!"
            // Client will handle pushing data to the other drive automatically once a connection has been made
        }
    }
}