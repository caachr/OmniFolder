//
// Created by Christian Caamano on 6/6/25.
//

#include <QTimer>

#include "port_authority.h"

PortAuthority::PortAuthority(QObject *parent)
    : QObject(parent)
{

}

void PortAuthority::testPort(const qint32 port)
{
    QTimer::singleShot(2000, this, [this]() {
        emit portTestSuccess();
    });
}

void PortAuthority::testForward(const qint32 port)
{
    QTimer::singleShot(2000, this, [this]() {
        emit forwardTestSuccess();
    });
}
