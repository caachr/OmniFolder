//
// Created by Christian Caamano on 5/25/25.
//

#ifndef FEDEX_H
#define FEDEX_H

#include <memory>
#include "defines.h"

class Message;

class FedEx {
public:
    static void shipMessage(std::unique_ptr<Message> message);

    static void processReceivedParcel(char byteBuffer[1024]);
};

#endif //FEDEX_H
