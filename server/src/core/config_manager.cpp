//
// Created by Christian Caamano on 6/6/25.
//

#include "config_manager.h"

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{

}

void ConfigManager::validateConfig(const QString &path, const QString &username, const QString &password)
{
    if (path.isEmpty()) {
        emit validationFailure();
    }

    emit validationSuccess();
}
