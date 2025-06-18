#include "port_authority.h"
#include "message_schema.pb.h"
#include "net/fedex.h"
#include "core/client_core.h"

#include <QSslCertificate>
#include <QSslConfiguration>
#include <QTimer>
#include <QSettings>
#include <QMessageBox>

PortAuthority::PortAuthority(QObject *parent)
    : QObject(parent)
{
    socket = new QSslSocket(this);


    // Connect signals

    connect(socket, &QSslSocket::connected, this, [this]() {
        qDebug() << "TCP connection established, waiting for SSL handshake...";
    });

    connect(socket, &QSslSocket::encrypted, this, [this]() {
        qDebug() << "SSL handshake completed successfully!";
        // SSL connection is ready
    });

    connect(socket, &QSslSocket::disconnected, this, &PortAuthority::onSocketConnectionLost);

    connect(socket, &QSslSocket::readyRead, this, &PortAuthority::onDataReceived);

    // SSL errors -> should call first use / subsequent use handler
    connect(socket, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
            this, [this](const QList<QSslError>& errors) {
        qDebug() << "\n=== SSL ERRORS DETECTED ===";
        qDebug() << "Number of SSL errors:" << errors.size();

        for (int i = 0; i < errors.size(); ++i) {
            const QSslError& error = errors[i];
            qDebug() << "Error" << i+1 << ":" << error.errorString();
            qDebug() << "  Error code:" << error.error();
        }

        // Get certificate info regardless of errors
        QSslCertificate cert = socket->peerCertificate();
        if (!cert.isNull()) {
            qDebug() << "Certificate info:";
            qDebug() << "  Subject:" << cert.subjectDisplayName();
            qDebug() << "  Issuer:" << cert.issuerDisplayName();
            qDebug() << "  Self-signed:" << (cert.subjectDisplayName() == cert.issuerDisplayName());
        }

        qDebug() << "Calling onSslErrors handler...";
        this->onSslErrors(errors);
    });
}

void PortAuthority::setClientCore(ClientCore* clientCoreArg)
{
    clientCore = clientCoreArg;
}

void PortAuthority::setFedEx(FedEx* fedExArg)
{
    fedEx = fedExArg;
}

void PortAuthority::connectToServer(const QString& host, quint16 port)
{
    serverHost = host;
    serverPort = port;

    QSettings settings("OmniFolder Client", "Certificates");
    QString settingsKey = QString("%1_%2").arg(host).arg(port);

    storedFingerprint = settings.value(settingsKey).toString();

    if (storedFingerprint.isEmpty()) {
        qDebug() << "First time connecting to server";
        qDebug() << "Will perform trust-on-first-use verification";
        isFirstConnection = true;
    } else {
        qDebug() << "Verifying against stored certificate fingerprint";
        isFirstConnection = false;
    }

    // Configure SSL for self-signed certificates (verified manually)
    QSslConfiguration sslConfig = socket->sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    socket->setSslConfiguration(sslConfig);

    // Connect to server
    socket->connectToHostEncrypted(host, port);

    // Set a timeout
    QTimer::singleShot(15000, this, [this]() {
        if (socket->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Connection timeout";
            socket->abort();
            emit loginFailure("Connection timeout");
        }
    });
}

void PortAuthority::write(const QByteArray& dataFrame)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        throw std::runtime_error("Socket not connected, cannot send data frame");
    }

    qint64 bytesWritten = socket->write(dataFrame);

    if (bytesWritten == -1) {
        qDebug() << "Error: failed to write data frame to socket.";
    } else if (bytesWritten != dataFrame.size()) {
        qDebug() << "Warning: partial write - sent" << bytesWritten << "of" << dataFrame.size() << "bytes";
    } else {
        qDebug() << "Data frame sent successfully.";
    }
}

void PortAuthority::onSslErrors(const QList<QSslError>& errors)
{
    // Get the server's certificate
    QSslCertificate peerCert = socket->peerCertificate();
    if (peerCert.isNull()) {
        qDebug() << "❌ SECURITY FAILURE: No certificate received from server!";
        socket->abort();
        return;
    }

    // Calculate certificate fingerprint
    QByteArray fingerprint = peerCert.digest(QCryptographicHash::Sha256);
    QString currentFingerprint = fingerprint.toHex().toUpper();

    if (isFirstConnection) {
        handleFirstConnection(peerCert, currentFingerprint);
    } else {
        handleSubsequentConnection(peerCert, currentFingerprint);
    }
}

void PortAuthority::handleFirstConnection(const QSslCertificate& cert, const QString& fingerprint)
{
    // Extract certificate information
    QStringList commonNames = cert.subjectInfo(QSslCertificate::CommonName);
    QStringList organizations = cert.subjectInfo(QSslCertificate::Organization);
    QString certName = commonNames.isEmpty() ? "Unknown Server" : commonNames.first();
    QString orgName = organizations.isEmpty() ? "Not specified" : organizations.join(", ");

    // Build message
    QString message = QString("FIRST TIME CONNECTION TO %1\n"
                              "==========================================\n\n"
                              "Server Certificate Information:\n"
                              "  Common Name: %2\n"
                              "  Organization: %3\n"
                              "  Valid From: %4\n"
                              "  Expires: %5\n"
                              "  Fingerprint: %6\n\n"
                              "SECURITY VERIFICATION REQUIRED\n"
                              "Is this the correct server certificate?")
                          .arg(serverHost)
                          .arg(commonNames.isEmpty() ? "Not specified" : commonNames.join(", "))
                          .arg(orgName)
                          .arg(cert.effectiveDate().toString("yyyy-MM-dd hh:mm:ss"))
                          .arg(cert.expiryDate().toString("yyyy-MM-dd hh:mm:ss"))
                          .arg(fingerprint);

    // Use QTimer to defer dialog creation to main UI thread
    QTimer::singleShot(0, this, [this, message, fingerprint]() {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString("Certificate Verification - %1").arg(serverHost));
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Warning);

        QPushButton* trustButton = msgBox.addButton("Trust", QMessageBox::AcceptRole);
        QPushButton* rejectButton = msgBox.addButton("Reject", QMessageBox::RejectRole);
        msgBox.setDefaultButton(rejectButton);

        int result = msgBox.exec();

        if (result == QMessageBox::AcceptRole) {
            // Store fingerprint
            QSettings settings("OmniClient", "Certificates");
            QString settingsKey = QString("%1_%2").arg(serverHost).arg(serverPort);
            settings.setValue(settingsKey, fingerprint);
            settings.sync();

            // Accept SSL connection
            if (socket && socket->state() == QAbstractSocket::ConnectedState) {
                socket->ignoreSslErrors();
            }
        } else {
            // Reject connection
            if (socket) {
                socket->abort();
            }
        }
    });
}

void PortAuthority::handleSubsequentConnection(const QSslCertificate& cert, const QString& currentFingerprint)
{
    // Normalize fingerprints for comparison
    QString normalizedCurrent = QString(currentFingerprint).remove(':').remove(' ').toUpper();
    QString normalizedStored = QString(storedFingerprint).remove(':').remove(' ').toUpper();

    if (normalizedCurrent == normalizedStored) {
        // Certificate matches - show brief success notification
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString("🔒 Secure Connection - %1").arg(serverHost));
        msgBox.setText(QString("✅ Certificate verified against stored fingerprint\n"
                               "🔒 Connection is secure and authenticated"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.addButton("OK", QMessageBox::AcceptRole);
        msgBox.setStandardButtons(QMessageBox::NoButton); // Remove default buttons

        // Show briefly and auto-close after 2 seconds
        QTimer::singleShot(2000, &msgBox, &QMessageBox::accept);
        msgBox.exec();

        socket->ignoreSslErrors();
    } else {
        // Certificate changed - show security alert
        QString message = QString("🚨 SECURITY ALERT: CERTIFICATE CHANGED! 🚨\n"
                                  "=====================================\n\n"
                                  "The server's certificate has changed since your last connection!\n\n"
                                  "Previous fingerprint: %1\n"
                                  "Current fingerprint:  %2\n\n"
                                  "This could mean:\n"
                                  "  1. The server administrator updated the certificate (normal)\n"
                                  "  2. Someone is trying to intercept your connection (DANGER!)\n"
                                  "  3. You're connecting to a different server by mistake\n\n"
                                  "⚠️  Contact your server administrator to verify this change!\n\n"
                                  "Do you want to accept the new certificate?")
                              .arg(storedFingerprint)
                              .arg(currentFingerprint);

        QMessageBox msgBox;
        msgBox.setWindowTitle(QString("🚨 Security Alert - %1").arg(serverHost));
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.addButton("Accept New Certificate", QMessageBox::AcceptRole);
        msgBox.addButton("Reject Connection", QMessageBox::RejectRole);
        msgBox.setDefaultButton(QMessageBox::Cancel);

        int result = msgBox.exec();

        if (result == QMessageBox::AcceptRole) {
            // Update stored fingerprint
            QSettings settings("OmniClient", "Certificates");
            QString settingsKey = QString("%1_%2").arg(serverHost).arg(serverPort);
            settings.setValue(settingsKey, currentFingerprint);
            settings.sync();

            socket->ignoreSslErrors();
        } else {
            socket->abort();
        }
    }
}

void PortAuthority::onLoginRequested(const QString &ip, int port, const QString &username, const QString &password)
{
    // First, establish connection to server
    connectToServer(ip, port);

    // Construct login message
    omniserver::MessageProto protoMessage;
    omniserver::HeaderProto* header = protoMessage.mutable_header();
    header->set_sender_class("client");
    header->set_sender_uuid("omni-client-1234abcd");
    header->set_sender_host("1.2.3.4");
    header->set_receiver_class("server");
    header->set_receiver_uuid("omni-server-1234abcd");
    header->set_receiver_host(ip.toStdString());
    protoMessage.set_type(1);
    omniserver::PayloadProto* payload = protoMessage.mutable_payload();
    std::stringstream payloadJsonString;
    payloadJsonString << "{" << "\"" << "username" << "\"" << ":" << "\"" << username.toStdString() << "\"" << ", ";
    payloadJsonString << "\"" << "password" << "\"" << ":" << "\"" << password.toStdString() << "\"" << "}";
    payload->set_json_data(payloadJsonString.str());

    // Verify the message is properly initialized
    if (!protoMessage.IsInitialized()) {
        throw std::runtime_error("ERROR: Protobuf message is not properly initialized!");
    }

    // // Serialize the message first to get exact data
    // std::string messageData;
    // if (!protoMessage.SerializeToString(&messageData)) {
    //     throw std::runtime_error("ERROR: Failed to serialize protobuf message");
    // }

    // // Create length-delimited message manually
    // std::string serializedData;

    // // Calculate varint32 size for the length prefix
    // uint32_t messageSize = messageData.size();
    // uint32_t originalMessageSize = messageSize;  // Store original for debugging

    // // Encode varint32 manually
    // while (messageSize >= 0x80) {
    //     serializedData.push_back((messageSize & 0xFF) | 0x80);
    //     messageSize >>= 7;
    // }
    // serializedData.push_back(messageSize & 0xFF);

    // // Append the actual message data
    // serializedData.append(messageData);

    // // Convert to QByteArray
    // QByteArray dataToSend = QByteArray::fromStdString(serializedData);


    // Serialize to bytes with automatic length delimiter
    std::string serializedData;
    google::protobuf::io::StringOutputStream output(&serializedData);
    google::protobuf::io::CodedOutputStream coded_output(&output);

    // Write delimited message (automatically includes length prefix)
    coded_output.WriteVarint32(protoMessage.ByteSizeLong());
    protoMessage.SerializeToCodedStream(&coded_output);

    // Send data
    QByteArray dataToSend = QByteArray::fromStdString(serializedData);


    // Send data
    qint64 bytesWritten = socket->write(dataToSend);

    if (bytesWritten == -1) {
        qDebug() << "Error: failed to write login message to socket";
        emit loginFailure("Failed to write login message to socket.");
    } else if (bytesWritten != dataToSend.size()) {
        qDebug() << "WARNING: Partial write - sent" << bytesWritten << "of" << dataToSend.size() << "bytes";
        emit loginFailure("Only part of the login request data was written to the socket.");
    } else {
        qDebug() << "✅ Protobuf message sent successfully:" << bytesWritten << "bytes";
    }
}

void PortAuthority::onDataReceived()
{
    qDebug() << "Data received from server.";
    QByteArray data = socket->readAll();
    qDebug() << "Length:" << data.size() << "bytes";

    if (!fedEx) {
        throw std::runtime_error("PortAuth onrawdatarec: fedEx not initialized, must be initialized at this point to read raw socket data");
    }

    fedEx->processRawData(data);
}

void PortAuthority::onLoginReplyReady(const Message& reply)
{
    if (reply.getType() == MessageType::AuthAccepted) {
        emit loginSuccess();
        return;
    } else if (reply.getType() == MessageType::AuthRejected) {
        emit loginFailure(reply.getPayload()["details"].get<std::string>());
        return;
    } else {
        throw std::runtime_error("Received non-auth message after sending login request to server. Something's wrong on the server end...");
    }
}

void PortAuthority::onSocketConnectionLost()
{
    qDebug("Lost connection to socket, emitting serverConnectionLost()");
    emit serverConnectionLost();
}
