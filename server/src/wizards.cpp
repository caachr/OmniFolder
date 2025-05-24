//
// Created by Christian Caamano on 5/13/25.
//

#include "../include/wizards.h"

void Wizards::newNetworkWizard()
{
    std::string networkName;
    std::string username;
    std::string password;

    std::cout << "Please choose a name for your new OmniFolder Network.\n";
    std::cin >> networkName;
    std::cout << "\n";

    std::cout << "Please choose a username and password you will be required to present in two scenarios:"
                 "\n 1. on the server prior to making changes and"
                 "\n 2. on the client app when requesting to connect to the server.\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "\n";
    std::cout << "Password: ";
    std::cin >> password;
    std::cout << "\n";

    OmniServer::createInstance(networkName, username, password);
    OmniServer::getInstance()->saveConfig();
}

void Wizards::recoverNetworkWizard()
{
    std::string networkName;
    std::string username;
    std::string password;

    std::cout << "Please enter your old OmniFolder Network name.\n";
    std::cin >> networkName;
    std::cout << "\n";

    std::cout << "Please enter your old username and password. These will be used to configure login info for the new server.\n";

    std::cout << "As a reminder, you will be required to present these in two scenarios:"
                 "\n 1. on the server prior to making changes and"
                 "\n 2. on the client app when requesting to connect to the server.\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "\n";
    std::cout << "Password: ";
    std::cin >> password;
    std::cout << "\n";

    std::cout << "Please find the configuration file of your OmniFolder Network you would like to recover."
                 "Copy and paste this file into the root directory of this server application."
                 "The path should look like the following: 'OmniServer/config.json'.\n"
                 "Once you have done this, press enter and the application will automatically recover the network.\n";
    std::cout << "Press enter when ready...";
    std::cin.get();

    OmniServer::createInstance(networkName, username, password);

    std::ifstream i(CONFIG_FILE_RELATIVE_PATH);
    configformat_t configObj;
    i >> configObj;

    OmniServer::getInstance()->getOmniNetwork()->fromConfigObj(configObj);
    OmniServer::getInstance()->saveConfig();

    std::cout << "[Automatic recovery complete]."
                 "Please verify the following information below about your network to ensure recovery was successful."
                 "The application will now exit.\n";
    OmniServer::getInstance()->printInfoVerbose();
}