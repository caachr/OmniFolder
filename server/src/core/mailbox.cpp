//
// Created by Christian Caamano on 6/6/25.
//

#include "mailbox.h"

Mailbox::Mailbox(QObject *parent)
    : QObject(parent)
{

}

Mailbox::QueuedMessageContext Mailbox::openNextMessage()
{
    QueuedMessageContext queuedMessage = messageQueue.top();
    messageQueue.pop();
    return queuedMessage;
}

void Mailbox::onMessageReady(const Message& message, const QString& tempSocketId)
{
    messageQueue.push({message, tempSocketId});
    emit youveGotMail();
}
