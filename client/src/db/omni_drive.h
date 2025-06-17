//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_OMNI_DRIVE_H
#define OMNISERVER_OMNI_DRIVE_H

#include <QObject>

#include <nlohmann/json.hpp>

class OmniDrive : public QObject {
    Q_OBJECT

public:
    explicit OmniDrive(QObject *parent = nullptr);
    OmniDrive(nlohmann::json configInfo, QObject* parent = nullptr);

    QString getName() const;
    QString getId() const;
    QString getType() const;

private:
    QString id;
    QString name;
    QString type;     // F/P (full/partial)
    QString status;   // Up-to-date/stale
    QString currentHost;
    bool online;
    qint64 capacityBytes;
    qint64 usedBytes;
    QString path;     // Path on this physical drive to the OmniFolder

    // For partial drives only: the subdirectories to mirror
    std::vector<QString> mirrorDirs;
};


#endif //OMNISERVER_OMNI_DRIVE_H
