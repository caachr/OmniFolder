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
 * @p init
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
    static void execute_command(const std::string& command, const std::vector<std::string_view>& params);

private:
    /**
     * @p help : Explains the application to the user and provides a list of available commands.
     * Informs the user that command-specific help can be provided by typing "<command> --help".
     * @return Status code indicating success or failure of command execution attempt.
     */
    static void command_help();

    /**
     * @p version : Prints the version of this OmniFolder Server application.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static void command_version();

    /**
     * @p info : Displays information about this OmniFolder server.
     * \n "--verbose": provides comprehensive information.
     * @param args The additional arguments and flags specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static void command_info(const std::vector<std::string_view>& params);

    /**
     * @p init : Enters a setup wizard to initialize an OmniServer along with an OmniNetwork. Options:
     * \n "new" argument: create a new OmniServer and OmniNetwork.
     * \n "recover" argument: recover a missing or damaged OmniNetwork using a config file. Creates a new server.
     * \n If an OmniFolder server already exists in the system, the wizard is not entered and the user is informed of the existing server.
     * @param args The additional arguments and flags specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static void command_init(const std::vector<std::string_view>& params);

    /**
     * @p start : Starts the OmniFolder server. Note: Ctrl+C stops the server.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static void command_start();
};

#endif //COMMAND_HANDLER_H
