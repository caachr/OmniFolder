#include "client_core.h"

ClientCore::ClientCore(QObject *parent)
    : QObject(parent)
{

}

void ClientCore::setMailbox(Mailbox* mailboxArg)
{
    mailbox = mailboxArg;
}

void ClientCore::setFedEx(FedEx* fedExArg)
{
    fedEx = fedExArg;
}

void ClientCore::setPortAuthority(PortAuthority* portAuthorityArg)
{
    portAuthority = portAuthorityArg;
}

void ClientCore::boot()
{
    //
}

bool ClientCore::isLoggedIn()
{
    return loggedIn;
}

void ClientCore::onYouveGotMail()
{
    qDebug("Client received mail notif from mailbox");
    auto queuedMessage = mailbox->openNextMessage();
    handleMessage(queuedMessage.message);
}

void ClientCore::handleMessage(const Message& message)
{
    //
}
