//
// Created by Christian Caamano on 5/28/25.
//

#ifndef MESSAGE_VALIDATOR_H
#define MESSAGE_VALIDATOR_H

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <fstream>
#include <iostream>
#include "defines.h"

using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;

class MessageValidator {
public:
    static bool validate(const json& configObj);

private:
    static std::unique_ptr<json_validator> validator;
};

#endif //MESSAGE_VALIDATOR_H
