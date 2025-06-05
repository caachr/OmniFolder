//
// Created by Christian Caamano on 5/13/25.
//

#include "../include/command_handler.h"
#include "../include/wizards.h"

void CommandHandler::execute_command(const std::string& command, const std::vector<std::string_view>& params) {

    // Check argument syntax and run command if exists
    if (command == "help") {
        if (!params.empty()) { std::cout << "Error: help command does not take any arguments."; }
        command_help();
    } else if (command == "version") {
        if (!params.empty()) { std::cout << "Error: version command does not take any arguments."; }
        command_version();
    } else if (command == "info") {
        command_info(params);
    } else if (command == "init") {
        if (params.size() != 1) { std::cout << "Error: init command takes 1 argument ('new' or 'recover')."; }
        command_init(params);
    } else if (command == "start") {
        if (!params.empty()) { std::cout << "Error: start command does not take any arguments."; }
        command_start();
    } else {
        std::cout << "Error: unrecognized command. Run 'omniserver help' for a list of available commands.\n";
    }
}

void CommandHandler::command_help() {
    std::cout << "Usage: omniserver <command> [options]\n"
              << "Commands:\n"
              << "   help      Show available commands, arguments, and flags\n"
              << "   version   Print application version\n"
              << "   info      Print server information (verbose mode available)\n"
              << "             Usage: omniserver info (--verbose)\n"
              << "   init      Create new OmniFolder network or recover an existing one\n"
              << "             Usage: omniserver init [new | recover]\n"
              << "   start     Bring server online (note: no commands available while online)\n";
}

void CommandHandler::command_version() {
    // Print version
    std::cout << "OmniFolder Server v" << APP_VERSION;
}

void CommandHandler::command_info(const std::vector<std::string_view>& params) {
    // Load config file
    OmniServer::boot();

    // Print info or exit if invalid flag
    if (params.empty()) {
        OmniServer::getInstance()->printInfo();
    } else if (params[0] == "--verbose") {
        OmniServer::getInstance()->printInfoVerbose();
    } else {
        std::cout << "Error: info command only supports the flag '--verbose'.";
    }
}

void CommandHandler::command_init(const std::vector<std::string_view>& params) {
    // Enter setup wizard or exit if invalid subcommand
    if (params[0] == "new") {
        if (OmniServer::networkExists()) {
            std::cout << "WARNING: A config and auth file have been detected in the system, indicating that"
                         "a network likely exists in the system. Continuing with the setup wizard will overwrite this"
                         "network! If you do not wish to overwrite the existing network, please press Ctrl+C to quit."
                         "Otherwise, press any key and then enter to continue with the wizard at your own risk"
                         "(if you continue, it means you know what you're doing).";
            std::cin.get();
        }
        Wizards::newNetworkWizard();
    } else if (params[0] == "recover") {
        if (OmniServer::networkExists()) {
            std::cout << "WARNING: A config and auth file have been detected in the system, indicating that"
                         "a network likely exists in the system. Continuing with the setup wizard will overwrite this"
                         "network! If you do not wish to overwrite the existing network, please press Ctrl+C to quit."
                         "Otherwise, press any key and then enter to continue with the wizard at your own risk"
                         "(if you continue, it means you know what you're doing).";
            std::cin.get();
        }
        Wizards::recoverNetworkWizard();
    } else {
        std::cout << "Error: unrecognized subcommand. init command only supports the subcommands 'new' and 'recover'.";
    }
}

void CommandHandler::command_start() {
    // Load everything into memory
    OmniServer::boot();

    // Start the server loop
    OmniServer::getInstance()->start();
}