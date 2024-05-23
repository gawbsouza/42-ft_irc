/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:40:11 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/22 23:12:22 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"

#include <sstream>

PassCommand::PassCommand(UserService & service): _service(service) {}
PassCommand::~PassCommand(void) {}

void PassCommand::execute(User & user, std::vector<std::string> args) const
{
    Connection &conn = user.getConnection();

    if(user.isAuthenticated()) {
        conn.sendMessage(":ft_irc 400 :You already authenticated\r\n");
        return;
    }

    if (args.size() == 0) {
        conn.sendMessage(":ft_irc 461 PASS :Not enough parameters\r\n");
        return;
    }
    
    if (conn.getPassword() != args.at(0)) {
        conn.sendMessage(":ft_irc 464 :Password incorrect\r\n");
        return;
    }

    user.authenticate();
}