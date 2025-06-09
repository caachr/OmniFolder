//
// Created by Christian Caamano on 5/11/25.
//

#include "../include/server.h"

OmniServer* OmniServer::instance = nullptr;


/// _____________________________________________________
/// ------------------ PUBLIC INTERFACE -----------------
/// _____________________________________________________


void OmniServer::initNewNetwork(std::string& nameArg, std::string& ipaddressArg, uint32_t portArg,
                                std::string& usernameArg, std::string& passwordArg,
                                std::string& ghUsername, std::string& ghServerPAT, std::string& ghClientPAT)
{
    // Hash username & pass, write hash to auth file
    std::string hash = Hasher::hash({usernameArg, passwordArg});
    std::ofstream authFile(AUTH_FILE_RELATIVE_PATH);
    if (authFile.is_open()) {
        authFile << hash;
    } else {
        throw std::runtime_error("New network init error: could not open auth file.");
    }
    authFile.close();

    // Create UUIDs for server & network and create config info
    std::string serverUUID = "omni-server-" + uuids::to_string(uuids::uuid_system_generator{}());
    std::string networkUUID = "omni-net-" + uuids::to_string(uuids::uuid_system_generator{}());
    configformat_t configObj;
    configObj["network"] = {
            {"uuid", networkUUID},
            {"name", nameArg},
            {"server", {
                    {"app_uuid", serverUUID},
                    {"host", ipaddressArg},
                    {"port", portArg},
                    {"online", false}
            }},
            {"folders", configformat_t::array()}
    };
    if (!ConfigValidator::validate(configObj)) {
        throw std::logic_error("New network init error: config object failed schema validation.");
    }

    // Write config info to config file
    std::ofstream configFile(CONFIG_FILE_RELATIVE_PATH);
    if (configFile.is_open()) {
        configFile << configObj.dump();
    } else {
        throw std::runtime_error("New network init error: could not open config file.");
    }
    configFile.close();
}

void OmniServer::replaceDestroyedServerAndStart(std::string& ipaddressArg, uint32_t portArg, std::string& usernameArg,
                                   std::string& passwordArg, std::string& ghUsername, std::string& ghServerPAT, std::string& ghClientPAT)
{
    // Hash username & pass, write hash to auth file
    std::string hash = Hasher::hash({usernameArg, passwordArg});
    std::ofstream authFile(AUTH_FILE_RELATIVE_PATH);
    if (authFile.is_open()) {
        authFile << hash;
    } else {
        throw std::runtime_error("Recover old network error: could not open auth file.");
    }
    authFile.close();

    // Generate new server uuid
    std::string newServerUUID = "omni-server-" + uuids::to_string(uuids::uuid_system_generator{}());

    // Fetch & validate old config file
    std::ifstream oldConfigFile(CONFIG_FILE_RELATIVE_PATH);
    configformat_t oldConfig;
    if (oldConfigFile.is_open()) {
        oldConfigFile >> oldConfig;
    } else {
        throw std::runtime_error("Recover old network error: failed to open input file stream to read user-supplied config file.");
    }
    if (!ConfigValidator::validate(oldConfig)) {
        throw std::runtime_error("Recover old network error: user-supplied config file does not conform to config schema.");
    }

    // Swap out old config info
    configformat_t newConfig = oldConfig;
    newConfig["network"]["server"]["uuid"] = newServerUUID;
    newConfig["network"]["server"]["host"] = ipaddressArg;
    newConfig["network"]["server"]["port"] = portArg;
    if (!ConfigValidator::validate(newConfig)) {
        throw std::logic_error("Recover old network error: new config object failed schema validation.");
    }

    // Write new config to file
    std::ofstream newConfigFile(CONFIG_FILE_RELATIVE_PATH);
    if (newConfigFile.is_open()) {
        newConfigFile << newConfig;
    } else {
        throw std::runtime_error("Recover old network error: failed to open output file stream to write new config.");
    }
    newConfigFile.close();

    // Create server instance
    std::string netName = newConfig["name"];
    createInstance(netName, hash);
    getInstance()->loadConfig();

    // TODO fetch loggedInClients list from private repo (separate from the one clients can see) and fill in this server's list
//    getInstance()->loggedInClients = GitHubPortal::fetchClientListBackup(ghUsername, ghServerPAT);
    // Will want to std::move each of the login creds inside the function itself and then immediately destroy them upon successful login

    // TODO start server
    // getInstance()->start();

    // TODO Update github repo with new server connection info AND NEW SERVER UUID! (clients will automatically fetch new info)
    // GithubPortal::updateServerUpdateRepo(newIP, newPort, std::move(ghubUsername), std::move(ghubPrivatePAT));
    // Will want to std::move each of the login creds inside the function itself and then immediately destroy them upon successful login

    // At this point, will receive standard heartbeat message from client; everything should be working as standard.

    // Standard heartbeat reply to clients will include updated config info, which clients will receive and store as per usual (no fancy new logic needed).
    // Client side logic will look something like:
    // "if (no heartbeat reply from server) {freeze everything, server disconnected = true}
    // start 2 min countdown
    // while (disconnected && 2 mins not passed yet) {send heartbeat every 10 secs, if (reply received) disconnected = false}
    // if (still disconnected) {while (disconnected) {every 30 secs: fetch new server info from github repo & send heartbeat, if (reply received) disconnected = false}}"
}

OmniServer* OmniServer::getInstance()
{
    if (instance == nullptr) {
        throw std::runtime_error("Server getInstance error: Attempted to call getInstance on a null OmniServer.");
    }
    return instance;
}

bool OmniServer::exists()
{
    return instance != nullptr;
}

bool OmniServer::networkExists()
{
    std::ifstream configFile(CONFIG_FILE_RELATIVE_PATH);
    std::ifstream authFile(AUTH_FILE_RELATIVE_PATH);
    return (configFile.is_open() || authFile.is_open());
}

void OmniServer::boot()
{
    configformat_t configObj;
    std::string networkName;
    std::string authHash;

    // Open config & auth file
    std::ifstream configFile(CONFIG_FILE_RELATIVE_PATH);
    if (!configFile.is_open()) {
        throw std::runtime_error("Error: failed to open config file during boot. No changes made.\n");
    }
    std::ifstream authFile(AUTH_FILE_RELATIVE_PATH);
    if (!authFile.is_open()) {
        throw std::runtime_error("Error: failed to open authentication hash during boot. No changes made.\n");
    }

    // Extract info from config file
    configFile >> configObj;

    // Validate config schema conformation
    if (!ConfigValidator::validate(configObj)) {
        throw std::logic_error("Server boot error: configuration file does not conform to schema.");
    }

    // Get network name
    networkName = configObj.find("networkName").key();

    // Extract hash from auth file
    authFile >> authHash;

    // Create server singleton & network
    createInstance(networkName, authHash);

    // Set server info
    getInstance()->uuid = configObj["network"]["server"]["uuid"];
    getInstance()->ipaddress = configObj["network"]["server"]["host"];
    getInstance()->port = configObj["network"]["server"]["port"];

    // Load config into network instance
    getInstance()->omniNetwork->deserialize(configObj);

    configFile.close();
    authFile.close();
}

OmniNetwork* OmniServer::getOmniNetwork()
{
    return omniNetwork;
}

void OmniServer::start() const
{
    std::cout << "Starting server...\n";

    PortAuthority::open();

    std::cout << "Server started. Press Ctrl+C to stop the server at any time.\n"
                 "NOTE: The OmniFolder server is meant to run continuously on an 'always-on' machine."
                 "The server is the backbone of the network; it is strongly recommended to stop it only in emergencies.\n";
}

void OmniServer::printInfo() const
{
    // TODO
}

void OmniServer::printInfoVerbose() const
{
    // TODO
}

void OmniServer::handleMessage(const Message& message)
{
    // Validate correct server

    // Validate correct network

    if (message.getHeader()["network_uuid"] != omniNetwork->getUUID()) {
        // Gather necessary ingredients for the reply
        nlohmann::json ingredients;
        ingredients["original_header"] = message.getHeader();
        std::unique_ptr<Message> reply = std::move(MessageFactory::makeMessage("wrong_network", ingredients));
        FedEx::shipMessage(std::move(reply));
        return;
    }

    if (message.getType() == "login_request") {
        // Extract payload
        nlohmann::json payloadReceived = message.getPayload();
        std::string usernameArg = payloadReceived["username"];
        std::string passwordArg = payloadReceived["password"];

        std::unique_ptr<Message> reply;

        // Gather necessary ingredients for the reply
        nlohmann::json ingredients;
        ingredients["original_header"] = message.getHeader();

        // Select appropriate reply
        if (!validateCredentials(usernameArg, passwordArg)) {
            reply = std::move(MessageFactory::makeMessage("login_denied", ingredients));
        } else {
            // Add client to logged-in list
            std::string clientUUID = message.getHeader()["sender_app_uuid"];
            nlohmann::json clientMailAddress = message.getHeader()["sender_mailing_address"];
            loggedInClients[clientUUID] = clientMailAddress;

            // Construct reply
            reply = std::move(MessageFactory::makeMessage("login_granted", ingredients));
        }
        FedEx::shipMessage(std::move(reply));
        return;
    }

    // Don't go any further unless client is logged in
    if (std::find(loggedInClients.begin(), loggedInClients.end(), message.getHeader()["sender_application_uuid"]) == loggedInClients.end()) {
        std::unique_ptr<Message> reply;
        messageformat_t ingredients;
        ingredients["original_header"] = message.getHeader();
        reply = std::move(MessageFactory::makeMessage("unauthorized", ingredients));
        FedEx::shipMessage(std::move(reply));
        return;
    }

    if (message.getType() == "heartbeat") {
        // Send response to client: "I'm online!"
    }
    if (message.getType() == "acquireLockRequest") {
        std::string folderID;
        std::string driveID;

        OmniFolder* folder = OmniNetwork::getInstance()->getFolderByID(folderID);
        if (folder->getLockHolder() != "") {
            // Lock isn't free; send response to client: "DENY, msg: Failed to request lock, as it is already held by an existing drive."
        } else {
            // Lock is free, grant it
            folder->setLockHolder(driveID);
            // Send response to client: "GRNT, msg: Lock request granted."
            saveConfig();
        }
    }
//    if (message.getType() == "releaseLockRequest") {
//        std::string folderID;
//
//        OmniFolder* folder = OmniNetwork::getInstance()->getFolderByID(folderID);
//        if (folder->getLockHolder() == "0") {
//            // Send response to client: "DENY, msg: Release request failed, as no drive currently holds a lock."
//        } else {
//            // Lock is held, release it
//            folder->releaseLock();
//            // Send response to client: "GRNT, msg: Release request granted."
//            saveConfig();
//        }
//    }
    if (message.getType() == "pushAndReleaseRequest") {
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
    if (message.getType() == "drive_updated") {
        // TODO set drive up-to-date
    }
}


/// ______________________________________________________
/// ------------------- PRIVATE HELPERS ------------------
/// ______________________________________________________


void OmniServer::createInstance(std::string& omniNetworkName, std::string& authHash)
{
    if (!instance)
    {
        instance = new OmniServer(omniNetworkName, authHash);
    }
    else {
        throw std::logic_error("Error: attempted creating server instance when one already exists.");
    }
}

OmniServer::OmniServer(std::string& omniNetworkName, std::string& authHash)
        : authHash(authHash)
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

bool OmniServer::validateCredentials(std::string& usernameArg, std::string& passwordArg) const
{
    std::string authArg = usernameArg + "\x00" + passwordArg;
    return (crypto_pwhash_str_verify(authHash.c_str(), authArg.c_str(), authArg.size()) == 0);
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
    std::unique_ptr<OmniDrive> drive;
    drive->deserialize(driveConfig);

    omniNetwork->getFolderByID(folderID)->addDrive(std::move(drive));
    saveConfig();
}

void OmniServer::saveConfig()
{
    // Gather info
    configformat_t configObj = omniNetwork->serialize();
    configObj["network"]["server"]["uuid"] = uuid;
    configObj["network"]["server"]["host"] = ipaddress;
    configObj["network"]["server"]["port"] = port;
    if (!ConfigValidator::validate(configObj)) {
        throw std::runtime_error("Server save config error: config obj failed schema validation.");
    }

    // Write to local file
    std::ofstream configFile(CONFIG_FILE_RELATIVE_PATH);
    if (!configFile.is_open()) {
        throw std::runtime_error("Server save config error: failed to open local config file.");
    }
    configFile << configObj.dump(2);
    configFile.close();

    // Update config file on all drives
    for (auto folder : omniNetwork->getFoldersConst()) {
        for (auto drive : folder->getDrivesConst()) {
            // Mark drive as stale; will later be marked up-to-date if server receives message back "drive_updated".
            drive->setStatus("stale");

            std::string driveHost = drive->getHost();
            uint32_t drivePort = drive->getPort();

            nlohmann::json ingredients;

            ingredients["network_uuid"] = omniNetwork->getUUID();
            ingredients["server_uuid"] = uuid;
            ingredients["server_ip"] = ipaddress;
            ingredients["server_port"] = port;

            ingredients["drive_ip"] = driveHost;
            ingredients["drive_port"] = drivePort;

            ingredients["config"] = configObj;

            // Make and ship message
            FedEx::shipMessage(std::move(MessageFactory::makeMessage("config_update", ingredients)));
        }
    }
}

void OmniServer::loadConfig()
{
    // Load config file
    configformat_t configObj;
    std::ifstream configFile(CONFIG_FILE_RELATIVE_PATH);
    if (!configFile.is_open()) {
        throw std::runtime_error("Server load config error: failed to open config file.");
    }
    configFile >> configObj;
    if (!ConfigValidator::validate(configObj)) {
        throw std::runtime_error("Server load config error: config file failed schema validation.");
    }

    // Set server info
    uuid = configObj["network"]["server"]["uuid"];
    ipaddress = configObj["network"]["server"]["host"];
    port = configObj["network"]["server"]["port"];

    // Set network info
    omniNetwork->deserialize(configObj);
}

void OmniServer::pushChanges(std::string folderID, const std::string& callingDriveID)
{
    // TODO this method needs review and is incomplete

    OmniFolder* folder = OmniNetwork::getInstance()->getFolderByID(folderID);
    auto drives = folder->getDrivesConst();
    for (auto drive : drives) {
        if (drive->getID() != callingDriveID) {
            // Send message to client: "Here's everything you need to connect to this drive. Go ahead and connect and send him some data!"
            // Client will handle pushing data to the other drive automatically once a connection has been made
        }
    }
}