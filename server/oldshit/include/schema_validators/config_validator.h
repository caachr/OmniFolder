//
// Created by Christian Caamano on 5/28/25.
//

#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <fstream>
#include <iostream>
#include "defines.h"

using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;

class ConfigValidator {
public:
    static bool validate(const json& configObj);

private:
    static std::unique_ptr<json_validator> validator;
};

#endif //CONFIG_VALIDATOR_H
