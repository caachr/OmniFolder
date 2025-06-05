//
// Created by Christian Caamano on 5/16/25.
//

#include "../include/port_authority.h"
#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

void PortAuthority::open()
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

    // Set received data buffer size limit & data structure
    const size_t MAX_BUFFER_SIZE = 8192;
    std::vector<char> byteBuffer(MAX_BUFFER_SIZE);

    // Start accepting incoming connections
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

        // Read data with proper bounds checking
        ssize_t bytes = recv(clientFd, byteBuffer.data(), MAX_BUFFER_SIZE - 1, 0);
        if (bytes > 0) {
            byteBuffer[bytes] = '\0';

            // Validate data before processing
            if (validateShipment(byteBuffer.data(), bytes)) {
                FedEx::processReceivedParcel(byteBuffer.data());
            }
        }
        close(clientFd);
    }
    close(serverFd);
}

bool PortAuthority::validateShipment(const char *data, size_t length)
{

}

//Message& SocketMaster::parseMessage(char msgBuffer[1024])
//{
//    // Convert buffer to intermediate object
//    messageformat_t iObj = messageformat_t::parse(msgBuffer);
//
//    // Convert intermediate object to Message
//    auto* message = new Message(iObj);
//
//    return *message;
//}