//
// Created by Christian Caamano on 6/2/25.
//

//#include "../include/mailbox.h"
//
//std::priority_queue<std::unique_ptr<Message>,
//        std::vector<std::unique_ptr<Message>>,
//        Mailbox::MessageComparator> Mailbox::messageQueue;
//
//std::unique_ptr<Message> Mailbox::openNextMessage()
//{
//    return nullptr;
//}
//
//bool Mailbox::MessageComparator::operator()(const std::unique_ptr<Message> &a, const std::unique_ptr<Message> &b) const
//{
//    return calculatePriority(*a) > calculatePriority(*b);
//}

//int Mailbox::calculatePriority(const Message &message)
//{
//    int basePriority = getTypePriority(message.getType());
//
//    // VIP treatment for logged-in clients
//    if (message.getHeader().isFromLoggedInClient) {
//        basePriority -= 5; // Lower number = higher priority
//    }
//
//    return basePriority;
//}

//int Mailbox::getTypePriority(int type)
//{
//    switch (type) {
//        case Message::LOCK_RELEASE: return 1;
//        case Message::LOCK_REQUEST: return 2;
//        case Message::DRIVE_DISCONNECTED: return 3;
//        case Message::PUSH_CHANGES: return 4;
//        case Message::DRIVE_UPDATED: return 5;
//        case Message::LOGIN_REQUEST: return 6;
//        case Message::HEARTBEAT: return 10;
//        case Message::STATUS_QUERY: return 11;
//        default: throw std::runtime_error("Mailbox MessageComparator getTypePriority error: unrecognized message type.");
//    }
//}