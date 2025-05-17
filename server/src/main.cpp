#include <iostream>
#include "../include/command_handler.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Error: no command input. Run 'omniserver help' for a list of available commands.";
        return 0;
    }

    std::string command = argv[1];
    std::vector<std::string_view> params;
    for (int i = 2; i < argc; ++i) {
        params.emplace_back(argv[i]);
    }

    CommandHandler::execute_command(command, params);

    return 0;
}