//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_CONFIG_MANAGER_H
#define OMNISERVER_CONFIG_MANAGER_H

#include <QObject>
#include <QString>

class ConfigManager : public QObject {
    Q_OBJECT

public:
    explicit ConfigManager(QObject *parent = nullptr);

signals:
    void validationSuccess();
    void validationFailure();

public slots:
    // Username & pass used for decryption of config.enc
    void validateConfig(const QString &path, const QString &username, const QString &password);
};


#endif //OMNISERVER_CONFIG_MANAGER_H
