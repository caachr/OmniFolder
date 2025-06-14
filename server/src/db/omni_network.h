//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_OMNI_NETWORK_H
#define OMNISERVER_OMNI_NETWORK_H

#include <QObject>

#include "omni_folder.h"

class OmniNetwork : public QObject {
    Q_OBJECT

public:
    explicit OmniNetwork(QObject *parent = nullptr);

    void loadFromConfig();

private:
    QString uuid;
    QString name;
    QVector<std::unique_ptr<OmniFolder>> folders;
};


#endif //OMNISERVER_OMNI_NETWORK_H
