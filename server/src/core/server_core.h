//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_SERVER_CORE_H
#define OMNISERVER_SERVER_CORE_H

#include <QObject>
#include <QHash>
#include <QString>

#include <nlohmann/json.hpp>

#include "net/client_session.h"
#include "net/client_info.h"
#include "net/port_authority.h"
#include "net/message_builder.h"
#include "config_manager.h"
#include "crypto_manager.h"
#include "mailbox.h"

class ServerCore : public QObject {
    Q_OBJECT

public:
    explicit ServerCore(QObject *parent = nullptr);

    // Set pointers to unique object instances from app core
    void setMailbox(Mailbox* mailboxArg);
    void setFedEx(FedEx* fedExArg);
    void setPortAuthority(PortAuthority* portAuthorityArg);

    // Bring server online (open for listening)
    void boot();

    // Load server info from config file
    void loadFromConfig();

    QString getUUID() const;
    QString getHost() const;

signals:
    /**
     * @brief clientAuthenticated - Signal meant for PortAuthority's onClientAuthenticated slot.
     * PortAuthority will then construct a new client session using the temp socket id and the client info.
     * @param tempSocketId
     * @param clientInfo
     */
    void clientAuthenticated(const QString& tempSocketId, const ClientInfo& clientInfo);
    void clientAuthFailed(const QString& tempSocketId, const ClientInfo& clientInfo);

    // Signals for client table GUI
    void clientConnected(const ClientInfo& clientInfo);
    void clientDisconnected(const QString& clientUUID);

public slots:
    void onYouveGotMail();

    // Plugs into PortAuthority's newClientSession signal (emitted after client session has been made)
    void onNewClientSession(const QString& clientUUID, ClientSession* clientSession);

    // Plugs into ClientSession's disconnected signal
    // Inside: emits clientDisconnected signal for GUI table to remove client from display
    void onClientSessionDisconnected(const QString& clientUUID);

private:
    QString uuid;
    QString host;
    qint32 port;

    QString beaconURL;
    QString beaconPAT;

    CryptoManager cryptoManager;

    Mailbox* mailbox;              // Pointer to single mailbox instance from app core
    FedEx* fedEx;                  // Pointer to single fedex instance from app core
    PortAuthority* portAuthority;  // Pointer to single portauthority instance from app core

    /**
     * @brief activeSessions - Logged in client list, indexed by client app UUID
     */
    QHash<QString, ClientSession*> activeSessions;

    /**
     * @brief handleMessage - Handle a Message received from a client.
     * @param message - The Message to handle.
     * @param tempSocketId - Optional param: Temporary socket uuid for unauthenticated sockets.
     */
    void handleMessage(const Message& message, const QString& identifier, const bool authenticated);

    bool credentialsValid(const std::string& username, const std::string& password);
};


#endif //OMNISERVER_SERVER_CORE_H
