#include <iostream>
#include "include/server.h"

int main() {
    std::string input;

    std::cout
    << "[OMNIFOLDER SERVER: VERSION 1.0.0]\n"
    << "Welcome to the OmniFolder Server application!\n"
    << "Please select an option, or press q to quit the application.\n"
    << "[1] Local server login\n"
    << "[2] New server setup\n";

    std::cin >> input;
    while (!(input == "1" || input == "2" || input == "q")) std::cin >> input;
    if (input == "q") return 0;

    if (input == "1")
    {
        if (!OServer::validateExists())
        {
            std::cout << "No server has been configured. Press any key then enter to quit the application.\n";
            std::cin >> input;
            return 0;
        }

        std::cout << "You are now logging in to the following OmniFolder Network: " << OServer::getInstance().getNetworkName();
        std::cout << "Please enter your credentials.\n";

        std::string username;
        std::cout << "Username: ";
        std::cin >> username;

        std::string password;
        std::cout << "Password: ";
        std::cin >> password;

        while (!OServer::getInstance().validateCredentials(username, password))
        {
            std::cout << "Invalid username or password. Please try again.";
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
        }

        std::cout << "Logging you in now...\n";

        // Enter into server land

        OServer::getInstance().start();
    }
    else if (input == "2")
    {
        std::cout
        << "Please select your desired setup for this server, or press q to quit the application.\n"
        << "[1] I am creating a new OmniFolder network and would like to configure this as its server.\n"
        << "[2] I am using this server to replace a missing/damaged server from an existing OmniFolder network.\n";

        std::cin >> input;
        while (!(input == "1" || input == "2" || input == "q")) std::cin >> input;
        if (input == "q") return 0;

        if (input == "1")
        {
            std::cout
            << "OMNIFOLDER SERVER: NEW NETWORK CREATION WIZARD\n";

            std::cout << "Please choose a name for this new OmniFolder network (no spaces).\n";

            std::string networkName;
            std::cin >> networkName;

            std::cout << "Please confirm this network name by pressing Y, or press any other key to redo: \n" << networkName;
            std::cin >> input;

            while (input != "Y")
            {
                std::cout << "Please choose a name for this new OmniFolder network (no spaces).\n";
                std::cin >> networkName;
                std::cout << "Please confirm this network name by pressing Y, or press any other key to redo: \n" << networkName;
                std::cin >> input;
            }

            std::cout << "Please set a secure username and password you will use to log in to this network.\n";

            std::string username;
            std::string password;

            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "\n";

            std::cout << "Password: ";
            std::cin >> password;
            std::cout << "\n";

            std::cout << "Please confirm these are your desired credentials by typing Y, or type any other key to redo.\n";
            std::cout << "Username: " << username << "\n";
            std::cout << "Password: " << password << "\n";
            std::cin >> input;

            while (input != "Y")
            {
                std::cout << "Please set a secure username and password for your OmniFolder network.\n";
                std::cout << "Username: ";
                std::cin >> username;
                std::cout << "\n";
                std::cout << "Password: ";
                std::cin >> password;
                std::cout << "\n";
                std::cout << "Please confirm these are your desired credentials by typing Y, or type any other key to redo.\n";
                std::cout << "Username: " << username << "\n";
                std::cout << "Password: " << password << "\n";
                std::cin >> input;
            }
            std::cout << "Credentials confirmed.\n";

            // Remaining server setup shit

            OServer::createInstance(networkName, username, password);

            std::cout << "Press q to quit the application (your server will be saved) or type 'start' to start the server.\n";

            std::cin >> input;
            while (!(input == "q" || input == "start")) std::cin >> input;
            if (input == "q") return 0;

            OServer::getInstance().start();
        }
        else if (input == "2")
        {
            std::cout
            << "OMNIFOLDER SERVER: SERVER REPLACEMENT WIZARD\n";

            std::cout << "Please input the name of your OmniFolder network with the missing/broken server."
                         "Tip: If you do not remember the name, it will be listed in the server's metadata information,"
                         "which can be found on any of the drives in the network in a file named 'networkinfo.txt'.\n";

            std::string networkName;
            std::cin >> networkName;

            // server replacement shit

        }
    }


    return 0;
}
