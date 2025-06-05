//
// Created by Christian Caamano on 5/13/25.
//

#include "../include/wizards.h"

void Wizards::newNetworkWizard()
{
    std::string name;
    std::string ipaddress;
    uint32_t port;
    std::string username;
    std::string password;
    std::string ghubUsername;
    std::string ghubPrivatePAT;

    std::cout << "[OMNIFOLDER SERVER: NEW NETWORK WIZARD]\n";

    std::cout << "To begin, please follow the tutorial in the PDF manual 'OmniFolder User Guide.pdf' to configure port forwarding on your router,"
                 "then proceed with the setup wizard. Press enter to continue with setup, or press Ctrl+C to abort"
                 "if you're not ready to continue yet.\n";
    std::cin.get();

    std::cout << "Please choose a name for your new OmniFolder Network.\n"
                 "Example: 'John Doe's Personal OmniFolder Network'\n";
    std::cin >> name;
    std::cout << "\n";

    std::cout << "Please input the public IP address of your router (the address you found in the tutorial).\n"
                 "Example: 203.45.128.91\n";
    std::cin >> ipaddress;
    std::cout << "\n";

    std::cout << "Please input the port number you configured during port forwarding.\n"
                 "Example: 8080\n";
    std::cin >> port;
    std::cout << "\n";

    std::cout << "Please input your GitHub username.\n";
    std::cin >> ghubUsername;
    std::cout << "\n";

    std::cout << "Please input the GitHub personal access token you just created.\n";
    std::cin >> ghubPrivatePAT;
    std::cout << "\n";

    std::cout << "Please choose a username and password you will be required to present on the client app"
                 "when logging in to the server.\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "\n";
    std::cout << "Password: ";
    std::cin >> password;
    std::cout << "\n";

    std::cout << "All necessary information received. Creating network...\n";

    OmniServer::initNewNetwork(name, ipaddress, port, username, password, ghubUsername, ghubPrivatePAT);

    std::cout << "New network created successfully.\n"
                 "(Reminder: to start the server, input 'omniserver start' into the terminal)\n";
}

void Wizards::recoverNetworkWizard()
{
    std::string name;
    std::string ipaddress;
    uint32_t port;
    std::string username;
    std::string password;
    std::string ghubUsername;
    std::string ghubPrivatePAT;

    std::cout << "[OMNIFOLDER SERVER: RECOVER NETWORK WIZARD]\n";

    std::cout << "To begin recovering your OmniFolder Network, ensure you have configured port forwarding"
                 "on your router.\n";
    std::cout << "Follow the tutorial in the PDF manual 'OmniFolder User Guide.pdf' to configure port forwarding on your router,"
                 "then proceed with the setup wizard. Press enter to continue with setup, or press Ctrl+C to abort"
                 "if you're not ready to continue yet.\n";
    std::cin.get();

    std::cout << "Please input the public IP address of your router.\n"
                 "Example: 203.45.128.91\n";
    std::cin >> ipaddress;
    std::cout << "\n";

    std::cout << "Please input the port number you configured during port forwarding.\n"
                 "Example: 8080\n";
    std::cin >> port;
    std::cout << "\n";

    std::cout << "Please enter your old username and password. These will be used to configure login info for the new server.\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "\n";
    std::cout << "Password: ";
    std::cin >> password;
    std::cout << "\n";

    std::cout << "Please input your GitHub username and the personal access token (PAT) with access to your OmniFolder-Client-Recovery repository.\n";
    std::cout << "Github username: ";
    std::cin >> ghubUsername;
    std::cout << "\n";
    std::cout << "Personal access token: ";
    std::cin >> ghubPrivatePAT;
    std::cout << "\n";

    std::cout << "Please find the configuration file of your old OmniFolder Network."
                 "Copy and paste this file into the root directory of this server application."
                 "The path should look like the following: 'OmniFolder Server/config.json'.\n"
                 "Once you have done this, press enter and the application will automatically recover the network.\n";
    std::cout << "Press enter when ready...";
    std::cin.get();

    OmniServer::replaceDestroyedServerAndStart(ipaddress, port, username, password, ghubUsername, ghubPrivatePAT);

    std::cout << "Network recovery complete."
                 "Please verify the information about your network below to ensure recovery was successful.\n";
    OmniServer::getInstance()->printInfoVerbose();
}