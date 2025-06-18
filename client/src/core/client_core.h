#ifndef OMNICLIENT_CLIENT_CORE_H
#define OMNICLIENT_CLIENT_CORE_H

#include <QObject>

#include "mailbox.h"
#include "net/fedex.h"
#include "net/port_authority.h"

class ClientCore : public QObject {
    Q_OBJECT

public:
    explicit ClientCore(QObject *parent = nullptr);

    // Set pointers to unique object instances from app core
    void setMailbox(Mailbox* mailboxArg);
    void setFedEx(FedEx* fedExArg);
    void setPortAuthority(PortAuthority* portAuthorityArg);

    void boot();

    bool isLoggedIn();

public slots:
    void onYouveGotMail();

private:
    Mailbox* mailbox;
    FedEx* fedEx;
    PortAuthority* portAuthority;

    bool loggedIn = false;

    void handleMessage(const Message& message);
};

#endif //OMNICLIENT_CLIENT_CORE_H
