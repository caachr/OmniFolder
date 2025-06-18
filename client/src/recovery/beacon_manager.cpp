#include "beacon_manager.h"

BeaconManager::BeaconManager(const std::string& usernameArg, const std::string& passwordArg, const std::string& networkUUIDArg,
                             const std::string& gistURLArg, const std::string& ghPATArg)
{
    username = usernameArg;
    password = passwordArg;
    networkUUID = networkUUIDArg;
    gistURL = gistURLArg;
    ghPAT = ghPATArg;
}

