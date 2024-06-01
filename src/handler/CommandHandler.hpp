/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:36:07 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 19:49:32 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

#include "EventHandler.hpp"
#include "../command/Command.hpp"
#include "../entity/UserService.hpp"

#include <list>
#include <map>
#include <vector>

class CommandHandler: public EventHandler
{

private:

    std::map<std::string, Command*> _cmds;
    UserService & _service;

public:

    CommandHandler(UserService & service);
    ~CommandHandler();

    void handle(Event event);
    
    void addCommand(const std::string & name, Command & cmd);
    std::vector<std::string> listCommands(void);
    
};

#endif