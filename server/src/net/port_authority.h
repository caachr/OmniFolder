//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_PORT_AUTHORITY_H
#define OMNISERVER_PORT_AUTHORITY_H

#include <QObject>
#include <QNetworkAccessManager>

class PortAuthority : public QObject {
    Q_OBJECT

public:
    explicit PortAuthority(QObject *parent = nullptr);

    const std::string getExternalIp();

signals:
    void portTestSuccess();
    void portTestFailure();

    void forwardTestSuccess();
    void forwardTestFailure();
    void forwardTestUnavailable();

public slots:
    void testPort(const qint32 port);
    void testForward(const qint32 port);

private:
    QNetworkAccessManager *networkManager;

    static const int LOGIN_TIMEOUT_MS = 10000; // 10 seconds
};


#endif //OMNISERVER_PORT_AUTHORITY_H
