
#include "mailbox.h"

Mailbox::Mailbox(QObject *parent)
    : QObject(parent)
{

}

Mailbox::QueuedMessageContext Mailbox::openNextMessage()
{
    qDebug("client opening next message");
    QueuedMessageContext queuedMessage = messageQueue.top();
    messageQueue.pop();
    return queuedMessage;
}

void Mailbox::onMessageReady(const Message& message)
{
    qDebug("mailbox received message from fedex");
    messageQueue.push({message});
    emit youveGotMail();
}
