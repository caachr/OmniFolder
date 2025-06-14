//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_FEDEX_H
#define OMNISERVER_FEDEX_H

#include <QObject>
#include <QSslSocket>

#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>

#include "message_schema.pb.h"

#include "message.h"

class FedEx : public QObject {
    Q_OBJECT

public:
    explicit FedEx(QObject *parent = nullptr);

    /**
     * @brief processRawData - Process raw data received from a socket into a Message.
     * @param rawData - The raw data bytes received from the socket.
     * @param identifier - Either client UUID (from an authenticated client session) or temporary socket uuid
     * associated with an unauthenticated socket (one not associated with a client session).
     * Internally: identifier used to index buffer.
     */
    void processRawData(const QByteArray& rawData, const QString& identifier, const bool authenticated);

    void shipMessage(Message* message, QSslSocket* socket);

signals:
    /**
     * @brief messageReady - Signal emitted when a new Message has been received & validated and is now ready to be put into the MailBox.
     * Intended to plug into MailBox's onMessageReady slot.
     * @param message - The Message that has just been processed and is now ready to give to the MailBox.
     * @param tempSocketId - Optional param: temporary socket uuid
     * associated with an unauthenticated socket (one not associated with a client session).
     */
    void messageReady(const Message& message, const QString& identifier, const bool authenticated);

private:
    struct SourceBuffer {
        QByteArray recBuffer;
        quint32 expectedMessageSize = 0;
    };
    QHash<QString, SourceBuffer> sourceBuffers; // Key by source identifier (clientUUID or tempSocketId)

    bool processNextMessage(SourceBuffer& sourceBuffer, const QString& identifier, const bool authenticated);

    // Parse completed frame to proto, then convert to Message & return that Message
    Message parseProtoToMessage(const QByteArray& protoData);

    omniserver::MessageProto messageToProto(const Message& message);
};


#endif //OMNISERVER_FEDEX_H
