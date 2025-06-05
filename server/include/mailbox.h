//
// Created by Christian Caamano on 6/2/25.
//

#ifndef MAILBOX_H
#define MAILBOX_H

#include "message.h"

class Mailbox {
public:
    static std::unique_ptr<Message> openNextMessage();

    static void addMessage(const std::unique_ptr<Message> message);
private:
    struct MessageComparator {
        bool operator()(const std::unique_ptr<Message> &a,
                        const std::unique_ptr<Message> &b) const;
    };

    static int calculatePriority(const Message &message);

    static int getTypePriority(Message::Type type);

    // Priority queue based on message type and if client is in logged in list or not (logged in = VIP treatment)
    static std::priority_queue<std::unique_ptr<Message>, std::vector<std::unique_ptr<Message>>, MessageComparator> messageQueue;
};

#endif //MAILBOX_H
