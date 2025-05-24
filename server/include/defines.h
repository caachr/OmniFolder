//
// Created by Christian Caamano on 5/13/25.
//

#ifndef DEFINES_H
#define DEFINES_H


/* --------- VERSION --------- */

#define APP_VERSION "0.1.0"


/* --------- AUTHENTICATION CREDENTIALS --------- */

#define AUTH_CREDS_RELATIVE_PATH "auth.json"


/* --------- CONFIG FORMAT --------- */

// Format for configuration files and config information
#define CONFIG_FORMAT "JSON"

#include <nlohmann/json.hpp>

// Configuration object type (based on the config file format)
using configformat_t = nlohmann::json;

// Path to the config file relative to the root directory (server)
#define CONFIG_FILE_RELATIVE_PATH "config.json"


/* --------- NETWORK MESSAGE FORMAT --------- */

// Intermediate message format
#define I_MESSAGE_FORMAT "JSON"

// Intermediate message format object type (example of where it fits: byte buffer -> nlohmann::json (intermediate format object) -> Message)
using i_messageformat_t = nlohmann::json;



#endif //DEFINES_H
