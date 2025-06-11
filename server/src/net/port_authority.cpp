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

#include "port_authority.h"

PortAuthority::PortAuthority(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
{

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
