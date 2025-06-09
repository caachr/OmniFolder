//
// Created by Christian Caamano on 6/6/25.
//

#include <QTimer>

#include "beacon_manager.h"

BeaconManager::BeaconManager(QObject *parent)
    : QObject(parent)
{

}

void BeaconManager::testLogin(const QString &ghUsername, const QString &ghToken)
{
    QTimer::singleShot(3000, this, [this]() {
        emit loginSuccess();
    });
}
