//
// Created by Christian Caamano on 6/6/25.
//

#include <QJsonObject>
#include <QTimer>

#include "beacon_manager.h"

BeaconManager::BeaconManager(QObject *parent)
    : QObject(parent)
{

}

void BeaconManager::testLogin(const QString &ghUsername, const QString &ghToken)
{
    QNetworkRequest request(QUrl("https://api.github.com/user"));
    QByteArray tokenHeader = "token " + ghToken.toUtf8();
    request.setRawHeader("Authorization", tokenHeader);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    QTimer::singleShot(LOGIN_TIMEOUT_MS, reply, [reply, this]() {
        if (reply->isRunning()) {
            reply->abort();
            emit loginTimeout();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [=, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response);
            QString actualUsername = json["login"].toString();

            if (actualUsername.compare(ghUsername, Qt::CaseInsensitive) == 0) {
                testLogin_CreateGist(ghToken);
            } else {
                emit loginFailure();
            }
        } else {
            qWarning() << "GitHub login test failed:" << reply->errorString();
            emit loginFailure();
        }

        reply->deleteLater();
    });
}

void BeaconManager::testLogin_CreateGist(const QString &ghToken)
{
    QNetworkRequest request(QUrl("https://api.github.com/gists"));
    QByteArray tokenHeader = "token " + ghToken.toUtf8();
    request.setRawHeader("Authorization", tokenHeader);
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject fileContent;
    fileContent["content"] = "test content for login verification";

    QJsonObject files;
    files["test.enc"] = fileContent;

    QJsonObject gistData;
    gistData["description"] = "Beacon login test gist";
    gistData["public"] = false;
    gistData["files"] = files;

    QJsonDocument doc(gistData);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, data);

    QTimer::singleShot(LOGIN_TIMEOUT_MS, reply, [reply, this]() {
        if (reply->isRunning()) {
            reply->abort();
            emit loginTimeout();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [=, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response);
            QString createdGistId = json["id"].toString();

            if (!createdGistId.isEmpty()) {
                testLogin_EditGist(ghToken, createdGistId);
            } else {
                emit loginFailure();
            }
        } else {
            qWarning() << "GitHub gist creation failed:" << reply->errorString();
            emit loginFailure();
        }

        reply->deleteLater();
    });
}

void BeaconManager::testLogin_EditGist(const QString &ghToken, const QString &gistId)
{
    QNetworkRequest request(QUrl(QString("https://api.github.com/gists/%1").arg(gistId)));
    QByteArray tokenHeader = "token " + ghToken.toUtf8();
    request.setRawHeader("Authorization", tokenHeader);
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject fileContent;
    fileContent["content"] = "edited content for login verification";

    QJsonObject files;
    files["test.enc"] = fileContent;

    QJsonObject gistData;
    gistData["description"] = "Beacon login test gist - edited";
    gistData["files"] = files;

    QJsonDocument doc(gistData);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->sendCustomRequest(request, "PATCH", data);

    QTimer::singleShot(LOGIN_TIMEOUT_MS, reply, [reply, this]() {
        if (reply->isRunning()) {
            reply->abort();
            emit loginTimeout();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [=, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response);
            QString editedGistId = json["id"].toString();

            if (editedGistId == gistId) {
                testLogin_DeleteGist(ghToken, gistId);
            } else {
                emit loginFailure();
            }
        } else {
            qWarning() << "GitHub gist edit failed:" << reply->errorString();
            emit loginFailure();
        }

        reply->deleteLater();
    });
}

void BeaconManager::testLogin_DeleteGist(const QString &ghToken, const QString &gistId)
{
    QNetworkRequest request(QUrl(QString("https://api.github.com/gists/%1").arg(gistId)));
    QByteArray tokenHeader = "token " + ghToken.toUtf8();
    request.setRawHeader("Authorization", tokenHeader);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->deleteResource(request);

    QTimer::singleShot(LOGIN_TIMEOUT_MS, reply, [reply, this]() {
        if (reply->isRunning()) {
            reply->abort();
            emit loginTimeout();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [=, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Successful deletion returns 204 No Content
            emit loginSuccess();
        } else {
            qWarning() << "GitHub gist deletion failed:" << reply->errorString();
            emit loginFailure();
        }

        reply->deleteLater();
    });
}

std::string BeaconManager::createBeacon(const QString &ghToken)
{
    QNetworkRequest request(QUrl("https://api.github.com/gists"));
    QByteArray tokenHeader = "Bearer " + ghToken.toUtf8();  // Use Bearer instead of token
    request.setRawHeader("Authorization", tokenHeader);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("User-Agent", "OmniServer/0.1");  // GitHub requires User-Agent

    QJsonObject fileContent;
    fileContent["content"] = "# OmniFolder Recovery Beacon\n# This file serves as a recovery beacon for OmniFolder networks.\n";  // Add some content

    QJsonObject files;
    files["beacon.enc"] = fileContent;

    QJsonObject gistData;
    gistData["description"] = "OmniFolder Recovery Beacon";
    gistData["public"] = false;  // Secret gist
    gistData["files"] = files;

    QJsonDocument doc(gistData);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, data);

    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    timeoutTimer.start(10000);

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(&timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);

    loop.exec();

    std::string result;

    if (timeoutTimer.isActive() && reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response);
        QString gistUrl = json["html_url"].toString();
        result = gistUrl.toStdString();
    } else {
        qDebug() << "Gist creation failed:" << reply->errorString();
        QByteArray errorResponse = reply->readAll();
        qDebug() << "Error response:" << errorResponse;
    }

    timeoutTimer.stop();
    reply->deleteLater();

    return result;
}
