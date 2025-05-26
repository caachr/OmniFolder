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
 * Path to the stored authentication credentials relative to the server's root directory (server).
 */
#define AUTH_CREDS_RELATIVE_PATH "auth.json"


/* --------- CONFIG FORMAT --------- */

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


/* --------- NETWORK MESSAGE FORMAT --------- */
/**
 * Defines the serialization format for messages sent between server-client / client-client.
 */
#define MESSAGE_FORMAT "JSON"

/**
 * Message serialization format object type.
 * Example of where it fits: byte buffer -> nlohmann::json (message format object) -> Message
 */
using messageformat_t = nlohmann::json;



#endif //DEFINES_H
