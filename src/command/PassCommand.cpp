/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:40:11 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/28 00:13:13 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"
#include "ResponseCode.hpp"
#include "../helper/Log.hpp"

#include <sstream>

#define	PASS_NAME "PASS"

PassCommand::PassCommand(UserService & service): _service(service) {}
PassCommand::~PassCommand(void) {}

void PassCommand::execute(User & user, std::vector<std::string> args) const
{
	Log::info(PASS_NAME " command called");
    
	Connection &conn = user.getConnection();

    if(user.isAuthenticated()) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_UNKNOWNERROR SPACE PASS_NAME SPACE MSG_ALREADYAUTHENTICATED CRLF);
        return;
    }

    if (args.size() == 0) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_NEEDMOREPARAMS SPACE PASS_NAME SPACE MSG_NEEDMOREPARAMS CRLF);
        return;
    }
    
    if (conn.getPassword() != args.at(0)) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_PASSWDMISMATCH SPACE MSG_PASSWDMISMATCH CRLF);
        return;
    }
	
	Log::debug("User authenticated from " + conn.str());
    user.authenticate();	
}