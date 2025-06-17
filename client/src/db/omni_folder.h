//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_OMNI_FOLDER_H
#define OMNISERVER_OMNI_FOLDER_H

#include <QObject>

#include <nlohmann/json.hpp>

#include "omni_drive.h"

class OmniFolder : public QObject {
    Q_OBJECT

public:
    explicit OmniFolder(QObject *parent = nullptr);
    OmniFolder(nlohmann::json configInfo, QObject* parent = nullptr);

    QString getName() const;
    QString getId() const;
    std::vector<OmniDrive*> getDrives() const;

private:
    QString id;
    QString name;
    QString lockHolderId;
    std::vector<std::unique_ptr<OmniDrive>> drives;
};


#endif //OMNISERVER_OMNI_FOLDER_H
