//
// Created by Christian Caamano on 5/13/25.
//

#ifndef DEFINES_H
#define DEFINES_H


/* --------- VERSION --------- */

/**
 * The version of this OmniFolder Server app.
 */
#define APP_VERSION "0.1.0"


/* --------- AUTHENTICATION CREDENTIALS --------- */

/**
 * Path to the stored encrypted authentication hash relative to the server's root directory (server).
 */
#define AUTH_FILE_RELATIVE_PATH "auth.json"


/* --------- CONFIGURATION DATA --------- */

/**
 * Defines the serialization format for networks, folders, and drives, as well as config files.
 */
#define CONFIG_FORMAT "JSON"
#include <nlohmann/json.hpp>

/**
 * Configuration serialization format object type.
 * TIP: use this when serializing/deserializing networks, folders, and drives.
 */
using configformat_t = nlohmann::json;

/**
 * Path to the config file relative to the server's root directory (server).
 */
#define CONFIG_FILE_RELATIVE_PATH "config.json"

#define CONFIG_SCHEMA_RELATIVE_PATH "config_schema.json"

/* --------- NETWORK MESSAGE SERIALIZATION FORMAT --------- */
/**
 * Defines the serialization format for messages sent between server-client / client-client.
 */
#define MESSAGE_FORMAT "JSON"

/**
 * Message serialization format object type.
 * Example of where it fits: byte buffer -> nlohmann::json (message format object) -> Message
 *
 * REQUIREMENTS (for the type this alias refers to):
 * 1. Static parse method for char[1024]
 */
using messageformat_t = nlohmann::json;

#define MESSAGE_SCHEMA_RELATIVE_PATH "message_schema.json"

#endif //DEFINES_H
