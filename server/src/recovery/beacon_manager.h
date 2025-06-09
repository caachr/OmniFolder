//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_BEACON_MANAGER_H
#define OMNISERVER_BEACON_MANAGER_H

#include <QObject>

class BeaconManager : public QObject {
    Q_OBJECT

public:
    explicit BeaconManager(QObject *parent = nullptr);

signals:
    void loginSuccess();
    void loginFailure();

public slots:
    void testLogin(const QString &ghUsername, const QString &ghToken);
};

#endif //OMNISERVER_BEACON_MANAGER_H
