//
// Created by Christian Caamano on 6/6/25.
//

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

    connect(reply, &QNetworkReply::finished, this, [=, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument json = QJsonDocument::fromJson(response);
            QString actualUsername = json["login"].toString();

            if (actualUsername.compare(ghUsername, Qt::CaseInsensitive) == 0) {
                emit loginSuccess();
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
