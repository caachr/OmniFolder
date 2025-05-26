//
// Created by Christian Caamano on 5/25/25.
//

#ifndef FEDEX_H
#define FEDEX_H

#include <memory>

class Message;

class FedEx {
public:
    static void shipMessage(std::unique_ptr<Message> message);
};

#endif //FEDEX_H
