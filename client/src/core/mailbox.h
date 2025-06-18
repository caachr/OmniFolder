#ifndef OMNICLIENT_MAILBOX_H
#define OMNICLIENT_MAILBOX_H

#include <QObject>

#include "net/message.h"

class Mailbox : public QObject {
    Q_OBJECT

public:
    explicit Mailbox(QObject *parent = nullptr);

    struct QueuedMessageContext {
        Message message;

        bool operator<(const QueuedMessageContext& other) const {
            // Priority queue is max heap, but we want ordering based on lower value message types (lower val = higher prio)
            return message.getType() > other.message.getType();
        }
    };

    QueuedMessageContext openNextMessage();

signals:
    void youveGotMail();

public slots:
    /**
     * @brief onMessageReady - Plugs into FedEx's onMessageReady signal
     * @param message - The Message that has just been sanitized & validated & constructed by FedEx
     * @param tempSocketId - Optional param: The temporary socket uuid for a socket pending authorization.
     */
    void onMessageReady(const Message& message);

private:
    std::priority_queue<QueuedMessageContext> messageQueue;
};


#endif //OMNICLIENT_MAILBOX_H
