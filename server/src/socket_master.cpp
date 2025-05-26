//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/socket_master.h"
#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

void SocketMaster::open()
{
    // Create socket
    int serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverFd < 0) {
        std::cout << "Socket creation failed.\n";
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;


    // Bind socket to address
    if (bind(serverFd, (sockaddr*)& serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Socket bind failure.\n";
        close(serverFd);
        return;
    }

    // Listen for connections
    if (listen(serverFd, 5) < 0) {
        std::cout << "Listen failed\n.";
        close(serverFd);
        return;
    }

    // Start accepting incoming connections
    char msgBuffer[1024];
    while (true) {
        // Store client's identifying information for later use
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        // Accept the next client connection request in the queue
        int clientFd = accept(serverFd, (struct sockaddr*) &clientAddr, &clientAddrLen);
        if (clientFd < 0) {
            std::cout << "Accept failed.\n";
            continue;
        }
        // Get the raw bytes received from the client
        ssize_t bytes = recv(clientFd, msgBuffer, sizeof (msgBuffer), 0);
        if (bytes > 0) {
            msgBuffer[bytes] = '\0';
            std::cout << "Received buffer: " << msgBuffer << "\n";

            // Parse the bytes into common Message object
            Message& message = parseMessage(msgBuffer);
            std::cout << "Received message: " << message << "\n";

            // Analyze the message and perform any appropriate actions
            OmniServer::getInstance()->handleMessage(message);
        }
        close(clientFd);
    }
    close(serverFd);
}

Message& SocketMaster::parseMessage(char msgBuffer[1024])
{
    // Convert buffer to intermediate object
    messageformat_t iObj = messageformat_t::parse(msgBuffer);

    // Convert intermediate object to Message
    auto* message = new Message(iObj);

    return *message;
}