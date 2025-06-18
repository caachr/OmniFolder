#ifndef OMNICLIENT_BEACON_MANAGER_H
#define OMNICLIENT_BEACON_MANAGER_H

#include <string>

class BeaconManager {
public:
    BeaconManager() = delete;
    explicit BeaconManager(const std::string& usernameArg, const std::string& passwordArg, const std::string& networkUUIDArg,
                           const std::string& gistURLArg, const std::string& ghPATArg);

private:
    std::string username;    // Server username
    std::string password;    // Server password
    std::string networkUUID; // Network UUID

    std::string gistURL;
    std::string ghPAT;
};

#endif //OMNICLIENT_BEACON_MANAGER_H
