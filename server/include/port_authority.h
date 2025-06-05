//
// Created by Christian Caamano on 5/17/25.
//

#ifndef PORT_AUTHORITY_H
#define PORT_AUTHORITY_H

#include <string>
#include "defines.h"
#include "message.h"
#include "fedex.h"

class PortAuthority {
public:
    /**
     * Opens a socket and begins receiving data.
     */
    static void open();

private:
    /**
     * Validate that the data (shipment) received by the socket is in the correct format and is not malicious.
     * @param data The array of data to validate.
     * @param length The length of the array of data.
     * @return True if the data is valid, false if not.
     */
    static bool validateShipment(const char* data, size_t length);

//    /**
//     * Parse the byte buffer received by the socket into a new Message object.
//     * @param msgBuffer The buffer of raw bytes received by the socket.
//     * @return Reference to a Message object containing organized information extracted from the byte buffer.
//     */
//    static Message& parseMessage(char msgBuffer[1024]);
};

#endif //PORT_AUTHORITY_H
