/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:40:11 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 17:02:46 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"
#include "IRCProtocol.hpp"
#include "IRCProtocol.hpp"
#include "../helper/Log.hpp"
#include "../helper/Strings.hpp"

#include <cstdio>
#include <sstream>

PassCommand::PassCommand(UserService & service): _service(service) {}
PassCommand::~PassCommand(void) {}

void PassCommand::execute(User & user, std::vector<std::string> args) const
{
	Connection &conn = user.getConnection();
    
	Log::info(PASS_CMD " command called from " + conn.str());

    if(user.isAuthenticated()) {
        conn.sendMessage(Strings::f(ERR_ALREADYAUTHENTICATED, PASS_CMD));
        return;
    }

    if (args.size() == 0) {
        conn.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, PASS_CMD));
        return;
    }
    
    if (conn.getPassword() != args.at(0)) {
        conn.sendMessage(Strings::f(ERR_PASSWDMISMATCH, PASS_CMD));
        return;
    }
	
    user.authenticate();

	Log::info("User authenticated from " + conn.str());
}