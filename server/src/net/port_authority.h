//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_PORT_AUTHORITY_H
#define OMNISERVER_PORT_AUTHORITY_H

#include <QObject>

class PortAuthority : public QObject {
    Q_OBJECT

public:
    explicit PortAuthority(QObject *parent = nullptr);

signals:
    void portTestSuccess();
    void portTestFailure();

    void forwardTestSuccess();
    void forwardTestFailure();

public slots:
    void testPort(const qint32 port);
    void testForward(const qint32 port);
};


#endif //OMNISERVER_PORT_AUTHORITY_H
