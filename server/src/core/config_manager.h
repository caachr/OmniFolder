//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_CONFIG_MANAGER_H
#define OMNISERVER_CONFIG_MANAGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QUuid>
#include <QStandardPaths>
#include <QDir>

#include <fstream>

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

#include "crypto_manager.h"


class ConfigManager : public QObject {
    Q_OBJECT

public:
    explicit ConfigManager(QObject *parent = nullptr);

    /**
     * @brief validateLocalConfig - Validate the plaintext local config file stored in the server's app data directory.
     * @return True if the local config file is valid (valid json, conforms to schema), false if not.
     */
    bool validateLocalConfig() const;

    nlohmann::json getConfigFromFile() const;

signals:
    void beginningConfigSetup();
    void creatingAuthFile();
    void creatingConfig();
    void openingFile();
    void writingConfig();
    void writingBirthCert();
    void configSetupComplete();

    void validationSuccess();
    void validationFailure();

public slots:
    /**
     * @brief setupFirstConfigFile - Create the config.json file from the raw ingredients passed by the setup wizard.
     * @param ingredients - The raw ingredients passed by the setup wizard (network name, username, password, port)
     */
    void setupFirstConfigFile(nlohmann::json ingredients);

    // Username & pass used for decryption of config.enc
    void validateEncryptedConfig(const QString &path, const QString &username, const QString &password);

private:
    nlohmann::json_schema::json_validator jsonValidator;

    /**
     * @brief conformsToSchema - Validate that a given nlohmann::json object conforms to the schema defined in
     * resources/schemas/config_schema.json. Return true if conforms, false if not.
     * @param jsonObj - The nlohmann::json object to evaluate.
     * @return True if the given json object conforms to the config schema, false if it does not.
     */
    bool conformsToSchema(nlohmann::json jsonObj) const;
};


#endif //OMNISERVER_CONFIG_MANAGER_H
