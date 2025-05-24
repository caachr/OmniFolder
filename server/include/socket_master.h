//
// Created by Christian Caamano on 5/17/25.
//

#ifndef SOCKET_MASTER_H
#define SOCKET_MASTER_H

#include <string>
#include "defines.h"
#include "message.h"

#define PORT 8080

class SocketMaster {
public:
    /**
     * Opens a socket and begins receiving data.
     */
    static void open();

    /**
     * Parse the byte buffer received by the socket into a new Message object.
     * @param msgBuffer The buffer of raw bytes received by the socket.
     * @return Reference to a Message object containing organized information extracted from the byte buffer.
     */
    static Message& parseMessage(char msgBuffer[1024]);
};

#endif //SOCKET_MASTER_H
