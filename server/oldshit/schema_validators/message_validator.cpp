//
// Created by Christian Caamano on 5/28/25.
//

#include "../include/schema_validators/message_validator.h"

std::unique_ptr<json_validator> MessageValidator::validator = nullptr;

bool MessageValidator::validate(const json& configObj)
{
    if (!validator) {
        try {
            // First use; load schema file into validator
            std::ifstream schemaFile(MESSAGE_SCHEMA_RELATIVE_PATH);
            if (!schemaFile.is_open()) {
                throw std::runtime_error("Error: Could not open message schema file at CONFIG_SCHEMA_RELATIVE_PATH");
            }

            json schema;
            schemaFile >> schema;
            schemaFile.close();

            // Init validator
            validator = std::make_unique<json_validator>();
            validator->set_root_schema(schema);
        } catch (const std::exception& e) {
            std::cerr << "Error loading message schema. Details: " << e.what() << "\n";
            std::terminate();
        }
    }

    // Validate
    try {
        validator->validate(configObj);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Validating message json object failed. Details: " << e.what() << "\n";
        return false;
    }
}