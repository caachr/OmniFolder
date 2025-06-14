//
// Created by Christian Caamano on 6/6/25.
//

#include "config_manager.h"
#include <QtCore/qdebug.h>

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
    QFile schemaFile(":/resources/schemas/config_schema.json");
    if (!schemaFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Error: Could not open config schema file from resources");
    }

    QByteArray schemaData = schemaFile.readAll();
    schemaFile.close();
    nlohmann::json schema = nlohmann::json::parse(schemaData.toStdString());

    // Init validator
    jsonValidator.set_root_schema(schema);
}

nlohmann::json ConfigManager::getConfigFromFile() const
{
    if (!validateLocalConfig()) {
        throw std::runtime_error("ConfigManager: must have validated config file before calling getConfigFromFile()");
    }

    // Get file, parse raw text to json, clean up, return json
    QFile file(QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath("config.json"));
    file.open(QIODevice::ReadOnly);
    return nlohmann::json::parse(QString::fromUtf8(file.readAll()).trimmed().toStdString());
}

void ConfigManager::setupFirstConfigFile(nlohmann::json ingredients)
{
    emit beginningConfigSetup();
    CryptoManager cryptoManager;
    nlohmann::json config;

    // Get the standard application data directory
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(appDataDir)) {
        dir.mkpath(appDataDir);  // Create the directory if it doesn't exist
    }

    QString authFilePath = QDir(appDataDir).filePath("auth.txt");
    QString configFilePath = QDir(appDataDir).filePath("config.json");
    QString birthFilePath = QDir(appDataDir).filePath("birth-certificate.txt");

    qDebug() << "Config will be saved to:" << configFilePath;
    qDebug() << "Auth will be saved to:" << authFilePath;
    qDebug() << "Birth certificate will be saved to:" << birthFilePath;

    emit creatingAuthFile();
    // Hash username & pass, write hash to auth file
    std::string hash = cryptoManager.hashCreds(ingredients["username"], ingredients["password"]);
    std::ofstream authFile(authFilePath.toStdString());
    if (authFile.is_open()) {
        authFile << hash;
    } else {
        throw std::runtime_error("New network init error: could not open auth file at " + authFilePath.toStdString());
    }
    authFile.close();

    emit creatingConfig();
    config["network"]["uuid"] = "omni-net-" + QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    config["network"]["name"] = ingredients["name"];
    config["network"]["server"]["uuid"] = "omni-server-" + QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    config["network"]["server"]["host"] = ingredients["ip"];
    config["network"]["server"]["port"] = ingredients["port"];
    config["network"]["server"]["active_sessions"] = nlohmann::json::object();
    config["network"]["beacon"]["url"] = ingredients["beacon_url"];
    config["network"]["beacon"]["pat"] = ingredients["beacon_pat"];
    config["network"]["folders"] = nlohmann::json::array();

    if (!conformsToSchema(config)) {
        throw std::runtime_error("Config file setup error: config object failed schema validation.");
    }

    emit openingFile();
    std::ofstream configFile(configFilePath.toStdString());
    if (!configFile.is_open()) {
        throw std::runtime_error("Config file setup error: failed to open config.json for writing at " + configFilePath.toStdString());
    }

    emit writingConfig();
    configFile << config.dump(4);
    configFile.close();

    emit writingBirthCert();
    std::ofstream birthFile(birthFilePath.toStdString());
    if (!birthFile.is_open()) {
        throw std::runtime_error("New network setup: could not open birth certificate file for writing");
    }
    birthFile << QString("%1 born %2")
                     .arg(config["network"]["name"].dump())
                     .arg(QDateTime::currentDateTime().toString("MM-dd-yy hh:mm"))
                     .toStdString();

    birthFile.close();

    emit configSetupComplete();
}

bool ConfigManager::validateLocalConfig() const
{
    QFile configFile(QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath("config.json"));

    if (!configFile.open(QIODevice::ReadWrite)) {
        qDebug("Error: could not open local config.json for read/write.");
        return false;
    }

    std::string rawText = QString::fromUtf8(configFile.readAll()).trimmed().toStdString();
    nlohmann::json jsonObj;

    // Check if valid json
    try {
        jsonObj = nlohmann::json::parse(rawText);
    } catch (const std::exception& e) {
        qDebug("Local config.json file is not valid json.");
        return false;
    }

    // Valid json, check schema
    return conformsToSchema(jsonObj);
}

void ConfigManager::validateEncryptedConfig(const QString &path, const QString &username, const QString &password)
{
    CryptoManager cryptoManager;


    // Read in file
    QFile configFile(path + "/config.enc");

    // Throw if config.enc not found (existence should have been confirmed by the wizard calling this function)
    if (!configFile.exists()) {
        throw std::runtime_error("ConfigManager error: did not find a config.enc file to validate."
                                 "This file must exist at the path specified before entering the validateConfig function!");
    }

    if (!configFile.open(QIODevice::ReadOnly))
        throw std::runtime_error("ConfigManager error: failed to open config.enc for reading.");

    // salt | nonce | cipher | tag
    QByteArray rawBytes = configFile.readAll();
    configFile.close();

    if (rawBytes.size() < 16 + 12 + 16) {
        qDebug("Config.enc file too small / corrupt.");
        emit validationFailure();
        return;
    }


    // Get salt
    const unsigned char *salt = reinterpret_cast<const unsigned char*>(rawBytes.data());

    // Get nonce
    const unsigned char *nonce = salt + 16;

    // Derive decryption key
    unsigned char key[32];
    cryptoManager.deriveConfigKey(username.toStdString(), password.toStdString(), salt, key);

    // Get ciphertext & message authentication code (MAC tag)
    const unsigned char *cipherAndTag = nonce + 12;
    const std::size_t   cipherAndTagLength = rawBytes.size() - (16 + 12);   // Length = what's left after taking out salt and nonce


    // Attempt decryption
    std::vector<unsigned char> plaintext;
    if (!cryptoManager.decrypt(key, nonce, std::span(cipherAndTag, cipherAndTagLength), plaintext)) {
        qDebug("Config file decryption failed.");
        emit validationFailure();
        return;
    }


    // Decryption successful, validate plaintext against JSON schema
    nlohmann::json jsonObj;
    try {
        jsonObj = nlohmann::json::from_cbor(plaintext);
    } catch (const std::exception& e) {
        qDebug("Decrypted config.enc file is not valid json.");
        emit validationFailure();
        return;
    }

    if (!conformsToSchema(jsonObj)) {
        qDebug("Config file does not conform to schema.");
        emit validationFailure();
        return;
    }


    // Success
    emit validationSuccess();
}

bool ConfigManager::conformsToSchema(nlohmann::json jsonObj) const
{
    try {
        jsonValidator.validate(jsonObj);
        return true;
    } catch (const std::exception& e) {
        qDebug("Validating config json object failed. Details: %s", e.what());
        return false;
    }
}
