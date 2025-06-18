#ifndef OMNICLIENT_FEDEX_H
#define OMNICLIENT_FEDEX_H

#include <QObject>
#include <QSslSocket>

#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>

#include "message_schema.pb.h"

#include "message.h"
#include "port_authority.h"

class FedEx : public QObject {
    Q_OBJECT

public:
    explicit FedEx(QObject *parent = nullptr);

    void setPortAuthority(PortAuthority* portAuthorityArg);

    /**
     * @brief processRawData - Process raw data received from a socket into a Message.
     * Fits into the main data -> processing -> message -> mailbox chain.
     * @param rawData - The raw data bytes received from the socket.
     */
    void processRawData(const QByteArray& rawData);

    void shipMessage(Message* message);

signals:
    /**
     * @brief messageReady - Signal emitted when a new Message has been received & validated and is now ready to be put into the MailBox.
     * Intended to plug into MailBox's onMessageReady slot.
     * @param message - The Message that has just been processed and is now ready to give to the MailBox.
     */
    void messageReady(const Message& message);

private:
    QByteArray recBuffer;  // Single byte buffer from server socket
    quint32 expectedMessageSize = 0;

    PortAuthority* portAuthority;  // Pointer to the unique PortAuthority instance in app core

    bool processNextMessage();

    // Parse completed frame to proto, then convert to Message & return that Message
    Message parseProtoToMessage(const QByteArray& protoData);

    omniserver::MessageProto messageToProto(const Message& message);
};


#endif //OMNICLIENT_FEDEX_H
