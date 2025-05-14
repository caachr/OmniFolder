//
// Created by Christian Caamano on 5/11/25.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <iostream>

class OServer {
public:
    /* --- Singleton logic --- */

    // No copy constructor or assignment operator in singletons
    OServer(const OServer&) = delete;
    OServer& operator=(const OServer&) = delete;

    ~OServer();

    static void createInstance(std::string networkName, std::string username, std::string password);
    static OServer& getInstance();

    static bool exists();

    /* --- Business logic --- */

    std::string& getNetworkName();

    bool validateCredentials(std::string username, std::string password);

    void start();

private:
    /* --- Singleton shit --- */
    static OServer* instance;
    OServer(std::string networkName, std::string username, std::string password);

    /* --- Business shit --- */
    std::string networkName;
    std::string username;
    std::string password;
};

#endif //SERVER_SERVER_H
