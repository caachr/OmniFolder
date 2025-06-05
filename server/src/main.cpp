#include <iostream>
#include "../include/command_handler.h"
#include <sodium.h>

int main(int argc, char* argv[]) {
    // Do nothing if user didn't input any args
    if (argc < 2) {
        std::cout << "Error: no command input. Run 'omniserver help' for a list of available commands.";
        return 0;
    }

    // Initialize crypto lib
    if (sodium_init() < 0) {
        std::cerr << "Error: Sodium library initialization failed.\n";
        return 0;
    }

    // Read in user args
    std::string command = argv[1];
    std::vector<std::string_view> params;
    for (int i = 2; i < argc; ++i) {
        params.emplace_back(argv[i]);
    }

    // Run user's command + params
    try {
        CommandHandler::execute_command(command, params);
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    return 0;
}