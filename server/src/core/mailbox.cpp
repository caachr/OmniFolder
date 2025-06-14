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
    qDebug("server opening next message");
    QueuedMessageContext queuedMessage = messageQueue.top();
    messageQueue.pop();
    return queuedMessage;
}

void Mailbox::onMessageReady(const Message& message, const QString& identifier, const bool authenticated)
{
    qDebug("mailbox received message from fedex");
    messageQueue.push({message, identifier, authenticated});
    emit youveGotMail();
}
