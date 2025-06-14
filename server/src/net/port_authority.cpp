//
// Created by Christian Caamano on 6/6/25.
//

#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QHostAddress>
#include <QTimer>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFile>
#include <QSslKey>
#include <QSsl>
#include <QProcess>

#include "port_authority.h"
#include "core/server_core.h"

PortAuthority::PortAuthority(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
{
    // Create the SSL server
    serverSocket = new QSslServer(this);

    // Set new connection signal/slot
    connect(serverSocket, &QSslServer::newConnection, this, &PortAuthority::onNewConnection);
}

void PortAuthority::configureSsl(const QString& certPath, const QString& keyPath, const QString& password)
{
    // Check if certificates exist, generate if they don't
    if (!QFile::exists(certPath) || !QFile::exists(keyPath)) {
        qDebug() << "SSL certificates not found, generating new ones...";
        if (!generateSelfSignedCertificates(certPath, keyPath)) {
            throw std::runtime_error("Failed to generate SSL certificates");
        }
    }

    // Load certificate from file
    QFile certFile(certPath);
    if (!certFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Cannot open certificate file: " + certPath.toStdString());
    }

    QSslCertificate certificate(&certFile, QSsl::Pem);
    if (certificate.isNull()) {
        throw std::runtime_error("Invalid certificate file: " + certPath.toStdString());
    }

    // Load private key from file
    QFile keyFile(keyPath);
    if (!keyFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Cannot open key file: " + keyPath.toStdString());
    }

    QSslKey privateKey(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, password.toUtf8());
    if (privateKey.isNull()) {
        throw std::runtime_error("Invalid private key file: " + keyPath.toStdString());
    }

    sslCertificate = certificate;
    sslPrivateKey = privateKey;
    sslPassword = password;

    qDebug() << "SSL server configured with certificate:" << certPath << "and key:" << keyPath;
}

void PortAuthority::startListening(const qint32 port)
{
    if (!serverSocket->listen(QHostAddress::Any, port)) {
        throw std::runtime_error("PortAuthority startListening: failed to start listening on any address");
    }
    qDebug() << "Server listening on all interfaces, port" << port;
}

void PortAuthority::stopListening()
{
    if (serverSocket->isListening()) {
        serverSocket->close();
    }

    for (auto& socket : socketsPendingAuth) {
        socket->close();
        socket->deleteLater();
    }
    socketsPendingAuth.clear();
}

void PortAuthority::setFedEx(FedEx* fedExArg)
{
    fedEx = fedExArg;
}

void PortAuthority::setServerCore(ServerCore* serverCoreArg)
{
    serverCore = serverCoreArg;
}

const std::string PortAuthority::getExternalIp()
{
    QUrl ipUrl("https://api.ipify.org");
    QNetworkRequest ipRequest(ipUrl);
    QNetworkReply *ipReply = networkManager->get(ipRequest);

    QEventLoop loop;
    connect(ipReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec(); // Block until request completes

    std::string result = ipReply->readAll().trimmed().toStdString();
    ipReply->deleteLater();
    return result;
}

void PortAuthority::testPort(const qint32 port)
{
    QTcpServer server;
    bool success = server.listen(QHostAddress::Any, port);
    server.close();

    if (success)
        emit portTestSuccess();
    else
        emit portTestFailure();
}

void PortAuthority::testForward(const qint32 port)
{
    // TODO might want to clean this shit up, looks a little ugly

    qDebug() << "Starting port forward test for port:" << port;

    // Start a temporary server to listen during the test
    QTcpServer *testServer = new QTcpServer(this);
    if (!testServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Cannot bind to port for testing:" << port;
        testServer->deleteLater();
        emit forwardTestFailure();
        return;
    }

    qDebug() << "Test server listening on port:" << port;

    // First get external IP
    QUrl ipUrl("https://api.ipify.org");
    QNetworkRequest ipRequest(ipUrl);
    QNetworkReply *ipReply = networkManager->get(ipRequest);

    qDebug() << "Requesting external IP from:" << ipUrl.toString();

    connect(ipReply, &QNetworkReply::finished, this, [this, ipReply, port, testServer]() {
        qDebug() << "External IP request finished";

        ipReply->deleteLater();
        if (ipReply->error() != QNetworkReply::NoError) {
            qDebug() << "External IP request failed with error:" << ipReply->errorString();
            qDebug() << "HTTP status code:" << ipReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            emit forwardTestUnavailable();
            return;
        }

        QString externalIP = ipReply->readAll().trimmed();
        qDebug() << "Retrieved external IP:" << externalIP;

        if (externalIP.isEmpty()) {
            qDebug() << "External IP is empty!";
            emit forwardTestUnavailable();
            return;
        }

        // Now test the port with the external IP
        QString urlString = QString("https://portchecker.io/api/%1/%2")
                                .arg(externalIP).arg(port);
        qDebug() << "Testing port with URL:" << urlString;

        QUrl url(urlString);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::UserAgentHeader, "PortAuthority/1.0");

        QNetworkReply *reply = networkManager->get(request);

        QTimer::singleShot(LOGIN_TIMEOUT_MS, reply, [reply, this, port]() {
            if (reply->isRunning()) {
                qDebug() << "Port test timed out for port:" << port;
                reply->abort();
                emit forwardTestUnavailable();
            }
        });

        connect(reply, &QNetworkReply::finished, this, [this, reply, port, testServer]() {
            qDebug() << "Port test request finished for port:" << port;

            reply->deleteLater();
            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << "Port test failed with error:" << reply->errorString();
                qDebug() << "HTTP status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                emit forwardTestUnavailable();
                return;
            }

            QByteArray data = reply->readAll();
            QString responseText = QString::fromUtf8(data).trimmed();
            qDebug() << "Port test response data:" << responseText;

            // Parse the response - it could be JSON or plain text
            bool isOpen = false;

            // First try to parse as JSON
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                qDebug() << "JSON response object:" << obj;
                isOpen = obj.value("open").toBool();
            } else {
                // Handle plain text responses
                qDebug() << "Response is plain text, not JSON";
                QString lowerResponse = responseText.toLower();
                isOpen = (lowerResponse == "true" || lowerResponse == "open" || lowerResponse == "1");
                qDebug() << "Parsed plain text response - port open:" << isOpen;
            }

            qDebug() << "Port" << port << "is open:" << isOpen;

            // Clean up test server BEFORE emitting signals
            testServer->close();
            testServer->deleteLater();

            if (isOpen) {
                qDebug() << "Emitting forwardTestSuccess for port:" << port;
                emit forwardTestSuccess();
            } else {
                qDebug() << "Emitting forwardTestFailure for port:" << port;
                emit forwardTestFailure();
            }
        });
    });
}

void PortAuthority::onNewConnection()
{
    while (serverSocket->hasPendingConnections()) {
        QSslSocket* clientSocket = qobject_cast<QSslSocket*>(serverSocket->nextPendingConnection());

        if (!clientSocket) {
            qWarning() << "PortAuth onnewconnection: failed to cast pending tcp connection to a QSslSocket;"
                          "moving on to next pending connection";
            continue;
        }

        // Configure SSL for this specific socket
        clientSocket->setLocalCertificate(sslCertificate);
        clientSocket->setPrivateKey(sslPrivateKey);

        // Generate temp ID for tracking, store in pending auth list
        QString tempSocketId = QUuid::createUuid().toString();
        socketsPendingAuth[tempSocketId] = clientSocket;

        // Set up SSL error handling
        connect(clientSocket, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
                this, [this, tempSocketId](const QList<QSslError>& errors) {
            onSslErrors(tempSocketId, errors);
        });

        // Connect read data / disconnect signals for this socket
        connect(clientSocket, &QSslSocket::readyRead, this, [this, tempSocketId]() {
            onRawDataReceived(tempSocketId);
        });
        connect(clientSocket, &QSslSocket::disconnected, this, [this, tempSocketId]() {
            onSocketDisconnected(tempSocketId);
        });

        // Start SSL handshake
        clientSocket->startServerEncryption();

        qDebug() << "New SSL connection from " << clientSocket->peerAddress() << " assigned tempId: " << tempSocketId;
    }
}

void PortAuthority::onRawDataReceived(const QString& tempSocketId)
{
    QSslSocket* socket = socketsPendingAuth[tempSocketId];
    if (!socket) {
        throw std::runtime_error("PortAuth onrawdatarec: attempted to get nonexisting socket from pending auth list.");
    }
    QByteArray rawData = socket->readAll();

    if (!fedEx) {
        throw std::runtime_error("PortAuth onrawdatarec: fedEx not initialized, must be initialized at this point to read raw socket data");
    }
    fedEx->processRawData(rawData, tempSocketId);
}

void PortAuthority::onSocketDisconnected(const QString& tempSocketId)
{
    QSslSocket* socket = socketsPendingAuth.take(tempSocketId);
    if (!socket) {
        throw std::runtime_error("PortAuth onsocketdisconnected: attempted to get nonexisting socket from pending auth list.");
    }
    qDebug() << "Pending auth socket disconnected: " << tempSocketId;
    socket->deleteLater();
}

void PortAuthority::onSslErrors(const QString& tempSocketId, const QList<QSslError>& errors)
{
    QSslSocket* socket = socketsPendingAuth[tempSocketId];
    if (!socket) {
        throw std::runtime_error("PortAuth onSslErrors: attempted to get nonexisting socket from pending auth list.");
    }

    qDebug() << "SSL errors for" << tempSocketId << ":";
    for (const auto& error : errors) {
        qDebug() << "  " << error.errorString();
    }

    // For development, you might want to ignore some errors:
    // socket->ignoreSslErrors();

    // For production, handle errors appropriately
    onSocketDisconnected(tempSocketId);
}

void PortAuthority::onClientAuthenticated(const QString& tempSocketId, const ClientInfo& clientInfo)
{
    // Retrieve socket
    QSslSocket* socket = socketsPendingAuth.take(tempSocketId);
    if (!socket) {
        throw std::runtime_error("PortAuth onClientAuthenticated: retrieved non-existent socket from socketsPendingAuth list");
    }

    // Disconnect PortAuthority's signals before transferring ownership
    socket->disconnect(this);

    // Create client session & hand off to server
    ClientSession* clientSession = new ClientSession(socket, clientInfo, this);
    clientSession->setFedEx(fedEx);
    emit newClientSession(clientInfo.clientUUID, clientSession);
}

void PortAuthority::onClientAuthFailed(const QString& tempSocketId)
{
    // Clean up socket that failed auth
    QSslSocket* socket = socketsPendingAuth.value(tempSocketId);
    if (!socket) {
        throw std::runtime_error("PortAuth onAuthFailed: attempted to remove non-existent socket from pending auth list");
    }

    // Send rejection message before cleanup
    nlohmann::json ingredients;
    Message* rejectReply = MessageBuilder::makeMessage(MessageType::AuthRejected, ingredients);
    fedEx->shipMessage(rejectReply, socket);

    // Wait for half a sec for the message to get shipped to the client
    QTimer::singleShot(500, this, [this, tempSocketId]() {
        QSslSocket* socket = socketsPendingAuth.take(tempSocketId);
        if (socket) {
            socket->close();
            socket->deleteLater();
        }
    });
}

bool PortAuthority::generateSelfSignedCertificates(const QString& certPath, const QString& keyPath) {
    qDebug() << "Generating self-signed SSL certificate and key...";

    // Get external IP first
    std::string externalIP = serverCore->getHost().toStdString();
    if (externalIP.empty()) {
        qWarning() << "Could not get external IP, using localhost";
        externalIP = "localhost";
    }

    QProcess opensslProcess;
    QStringList arguments;

    // Use external IP in the certificate
    QString subjectString = QString("/C=US/ST=State/L=City/O=OmniServer/CN=%1").arg(QString::fromStdString(externalIP));

    arguments << "req" << "-x509" << "-newkey" << "rsa:2048"
              << "-keyout" << keyPath
              << "-out" << certPath
              << "-days" << "365"
              << "-nodes"
              << "-subj" << subjectString;  // Use the external IP here

    opensslProcess.start("openssl", arguments);

    if (!opensslProcess.waitForFinished(30000)) {
        qWarning() << "OpenSSL process timed out";
        return false;
    }

    if (opensslProcess.exitCode() != 0) {
        qWarning() << "OpenSSL failed with exit code:" << opensslProcess.exitCode();
        qWarning() << "Error output:" << opensslProcess.readAllStandardError();
        return false;
    }

    if (!QFile::exists(certPath) || !QFile::exists(keyPath)) {
        qWarning() << "SSL files were not created successfully";
        return false;
    }

    qDebug() << "Successfully generated SSL certificate for IP:" << QString::fromStdString(externalIP);
    return true;
}
