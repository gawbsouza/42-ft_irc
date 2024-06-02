/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:00:00 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 19:56:12 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandHandler.hpp"
#include "../helper/Strings.hpp"
#include "../helper/Log.hpp"

#include <iostream>
#include <string>

CommandHandler::CommandHandler(UserService & service):_service(service) {}
CommandHandler::~CommandHandler() {}

void CommandHandler::handle(Event event)
{
    Connection & conn = event.conn;
    User * user = this->_service.getUserByConnection(conn);

    if (user == NULL) {
        Log::error("User not found to execute command from " + conn.str());
        return;
    }
        
    std::string eventMessage = event.message;

    if (eventMessage.size() == 0) {
        return;
    }

    Strings::replace(eventMessage, "  ", " "); // remove unused spaces
    Strings::replace(eventMessage, "\r", ""); // remove carriage return

    std::vector<std::string> commandsStr = Strings::split(eventMessage, "\n");
    std::vector<std::string>::iterator it;

    for (it = commandsStr.begin(); it != commandsStr.end(); it++)
    {
        std::vector<std::string> tokens = Strings::split(*it, " ");

        if (tokens.size() == 0) {
            continue;
        }

        std::string commandName = Strings::toUpper(tokens[0]);
        Command *command = this->_cmds[commandName];

        if (command == NULL) {
            Log::warning("Unknown command " + commandName + " from " + conn.str());
            conn.sendMessage(Strings::f(":ft_irc 421 %s %s :Unknown command\r\n", user->getNickName(), commandName));
            continue;
        }

        std::vector<std::string> commandArgs;

        if (tokens.size() > 1) {
            for (size_t i = 1; i < tokens.size(); i++) {
                commandArgs.push_back(tokens.at(i)); 
            }
        }

        command->execute(*user, commandArgs);
    }
}

void CommandHandler::addCommand(const std::string & name, Command & cmd)
{
    this->_cmds[Strings::toUpper(name)] = &cmd;
}

std::vector<std::string> CommandHandler::listCommands(void)
{
    std::map<std::string, Command *> copy = this->_cmds;
    std::map<std::string, Command *>::iterator it;
    std::vector<std::string> commands;

    for(it = copy.begin(); it != copy.end(); it++) {
        commands.push_back(it->first);
    }

    return commands;
}