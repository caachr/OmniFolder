//
// Created by Christian Caamano on 5/13/25.
//

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "../include/defines.h"
#include "../include/server.h"

/**
 * Handles and executes commands input by the user.
 * Currently supported commands:
 * @p help
 * @p version
 * @p info
 * @p status
 * @p init
 * @p reload
 * @p start
 */
class CommandHandler {
public:
    CommandHandler() = delete;
    CommandHandler(const CommandHandler&) = delete;
    CommandHandler& operator=(const CommandHandler&) = delete;
    ~CommandHandler() = delete;

    /**
     * Validates that the user has input a recognized command with valid argument syntax
     * and then executes the command by calling the appropriate function.
     * Returns a failure code if the user entered an invalid command, if the argument syntax was invalid,
     * or if the command's execution failed.
     * @param input The raw user input.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int execute_command(const std::string& command, const std::vector<std::string_view>& params);

private:
    /**
     * @p help : Explains the application to the user and provides a list of available commands.
     * Informs the user that command-specific help can be provided by typing "<command> --help".
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_help();

    /**
     * @p version : Prints the version of this OmniFolder Server application.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_version();

    /**
     * @p info : Displays information about this OmniFolder server.
     * \n "--verbose": provides comprehensive information.
     * @param args The additional arguments and flags specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_info(const std::vector<std::string_view>& params);

    /**
     * @p status : Reports the current status of this OmniFolder server and the OmniFolder network it is a part of.
     * \n "--advanced": provides comprehensive status information for advanced users.
     * @param args The additional arguments and flags specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_status(const std::vector<std::string_view>& params);

    /**
     * @p init : Enters a setup wizard to initialize an OmniServer along with an OmniNetwork. Options:
     * \n "new" argument: create a new OmniServer and OmniNetwork.
     * \n "recover" argument: recover a missing or damaged OmniNetwork using a config file. Creates a new server.
     * \n If an OmniFolder server already exists in the system, the wizard is not entered and the user is informed of the existing server.
     * @param args The additional arguments and flags specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_init(const std::vector<std::string_view>& params);

    /**
     * @p reload : Reloads an OmniNetwork into an existing server using a specified config file.
     * More specifically, deletes the current OmniNetwork from the system and replaces it with a new one
     * according to the config specifications. Leaves the server otherwise unchanged.
     * @param params Single parameter: the config file path specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_reload(const std::vector<std::string_view>& params);

    /**
     * @p start : Starts the OmniFolder server. Note: Ctrl+C stops the server.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_start();
};

#endif //COMMAND_HANDLER_H
