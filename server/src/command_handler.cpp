//
// Created by Christian Caamano on 5/13/25.
//

#include "../include/command_handler.h"
#include "../include/wizards.h"

int CommandHandler::execute_command(const std::string& command, const std::vector<std::string_view>& params) {

    // Check argument syntax and run command if exists
    if (command == "help") {
        if (!params.empty()) { std::cout << "Error: help command does not take any arguments."; return EXIT_FAILURE; }
        return command_help();
    } else if (command == "version") {
        if (!params.empty()) { std::cout << "Error: version command does not take any arguments."; return EXIT_FAILURE; }
        return command_version();
    } else if (command == "info") {
        return command_info(params);
    } else if (command == "status") {
        return command_status(params);
    } else if (command == "init") {
        if (params.size() != 1) { std::cout << "Error: init command takes 1 argument ('new' or 'recover')."; return EXIT_FAILURE; }
        return command_init(params);
    } else if (command == "reload") {
        if (params.size() != 1) { std::cout << "Error: reload command takes 1 argument (absolute path to config file)."; return EXIT_FAILURE; }
        return command_reload(params);
    } else if (command == "start") {
        if (!params.empty()) { std::cout << "Error: start command does not take any arguments."; return EXIT_FAILURE; }
        return command_start();
    } else {
        std::cout << "Error: unrecognized command. Run 'omniserver help' for a list of available commands.\n";
        return EXIT_FAILURE;
    }
}

int CommandHandler::command_help() {
    std::cout << "Usage: omniserver <command> [options]\n"
              << "Commands:\n"
              << "   help      Show available commands, arguments, and flags\n"
              << "   version   Print application version\n"
              << "   info      Print server information (verbose mode available)\n"
              << "             Usage: omniserver info (--verbose)\n"
              << "   status    Show OmniFolder network status (advanced status info available)\n"
              << "             Usage: omniserver status (--advanced)\n"
              << "   init      Create new OmniFolder network or recover an existing one\n"
              << "             Usage: omniserver init [new | recover]\n"
              << "   reload    Reload a new OmniFolder network into the server using a config file, overwriting the old network\n"
              << "             Usage: omniserver reload [path_to_config_file]\n"
              << "   start     Bring server online\n";
    return EXIT_SUCCESS;
}

int CommandHandler::command_version() {
    // Print version
    std::cout << "OmniFolder Server v" << APP_VERSION;
    return EXIT_SUCCESS;
}

int CommandHandler::command_info(const std::vector<std::string_view>& params) {
    // Can't display info if the server doesn't exist yet
    if (!(OmniServer::getInstance())) {
        std::cout << "Error: no OmniServer exists in the system. Please run the 'init' command to create an OmniServer.\n";
        return EXIT_FAILURE;
    }

    // TODO Startup: load config file into server

    // Print info or exit if invalid flag
    if (params.empty()) {
        OmniServer::getInstance()->printInfo();
    } else if (params[0] == "--verbose") {
        OmniServer::getInstance()->printInfoVerbose();
    } else {
        std::cout << "Error: info command only supports the flag '--verbose'.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int CommandHandler::command_status(const std::vector<std::string_view>& params) {
    // TODO Startup: load config file into server

    // Print status or exit if invalid flag
    if (params.empty()) {
        OmniServer::getInstance()->printStatus();
    } else if (params[0] == "--advanced") {
        OmniServer::getInstance()->printStatusAdvanced();
    } else {
        std::cout << "Error: status command only supports the optional flag '--advanced'.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int CommandHandler::command_init(const std::vector<std::string_view>& params) {
    // Don't set up a server if one already exists
    if (OmniServer::exists()) {
        std::cout << "Error: cannot create or recover a network, as a server already exists in the system.\n"
                  "If creating a new network, please configure the new server on a different host machine"
                  "to avoid communication conflicts and performance issues.\n"
                  "If you are recovering a network and the server you are replacing is in this system,"
                  "please copy all associated '.omnimeta' files to another location on your computer"
                  "(outside the OmniFolder Server directory), delete the OmniFolder Server directory,"
                  "install a fresh version of the app, and configure the server there.\n";
        return EXIT_FAILURE;
    }

    // Enter setup wizard or exit if invalid subcommand
    if (params[0] == "new") {
        Wizards::newNetworkWizard();
    } else if (params[0] == "recover") {
        Wizards::recoverNetworkWizard();
    } else {
        std::cout << "Error: unrecognized subcommand. init command only supports the subcommands 'new' and 'recover'.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int CommandHandler::command_reload(const std::vector<std::string_view>& params) {
    OmniServer::wipeAndReload();
    return EXIT_SUCCESS;
}

int CommandHandler::command_start() {
    OmniServer::boot();

    // Start the server loop
    OmniServer::getInstance()->start();
    return EXIT_SUCCESS;
}