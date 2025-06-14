//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_OMNI_DRIVE_H
#define OMNISERVER_OMNI_DRIVE_H

#include <QObject>

class OmniDrive : public QObject {
    Q_OBJECT

public:
    explicit OmniDrive(QObject *parent = nullptr);

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
    QVector<QString> mirrorDirs;
};


#endif //OMNISERVER_OMNI_DRIVE_H
