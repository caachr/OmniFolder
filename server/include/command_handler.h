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

class CommandHandler {
public:
    CommandHandler() = delete;
    CommandHandler(const CommandHandler&) = delete;
    CommandHandler& operator=(const CommandHandler&) = delete;
    ~CommandHandler() = delete;

    /**
     * Executes the user's command. Returns a failure code if the command's execution failed or if the user entered an invalid command.
     * @param input The raw user input.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int execute_command(const std::string& command, const std::vector<std::string_view>& params);

private:
    /**
     * Explains the application to the user and provides a list of available commands.
     * Informs the user that command-specific help can be provided by typing "<command> --help".
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_help();

    /**
     * Prints the version of this OmniFolder Server application.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_version();

    /**
     * Displays information about this OmniFolder server.
     * \n "--verbose": provides comprehensive information.
     * @param args The additional arguments and flags specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_info(const std::vector<std::string_view>& params);

    /**
     * Reports the current status of this OmniFolder server and the OmniFolder network it is a part of.
     * \n "--advanced": provides comprehensive status information for advanced users.
     * @param args The additional arguments and flags specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_status(const std::vector<std::string_view>& params);

    /**
     * Enters a setup wizard to either create a new OmniFolder server or replace a missing/damaged one.
     * \n "new" argument: create a new OmniFolder server.
     * \n "replace" argument: replace a missing or damaged OmniFolder server.
     * \n If an OmniFolder server already exists in the system, the wizard is not entered and the user is informed of the existing server.
     * @param args The additional arguments and flags specified by the user.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_init(const std::vector<std::string_view>& params);

    /**
     * Starts the OmniFolder server. Note: Ctrl+C stops the server.
     * @return Status code indicating success or failure of command execution attempt.
     */
    static int command_start();
};

#endif //COMMAND_HANDLER_H
