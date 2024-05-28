/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:40:11 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/28 03:40:25 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"
#include "ResponseCode.hpp"
#include "../helper/Log.hpp"

#include <sstream>

#define	PASS_CMD					"PASS"
#define PASS_SEP					SPACE PASS_CMD SPACE
#define PASS_SEP2					SPACE ASTERISK PASS_SEP
#define	JOINED_ALREADYAUTHENTICATED	SERVER_PREFIX SPACE ERR_UNKNOWNERROR PASS_SEP MSG_ALREADYAUTHENTICATED CRLF
#define	JOINED_PASSWDMISMATCH		SERVER_PREFIX SPACE ERR_PASSWDMISMATCH PASS_SEP MSG_PASSWDMISMATCH CRLF
#define JOINED_NEEDMOREPARAMS		SERVER_PREFIX SPACE ERR_NEEDMOREPARAMS PASS_SEP2 MSG_NEEDMOREPARAMS CRLF

PassCommand::PassCommand(UserService & service): _service(service) {}
PassCommand::~PassCommand(void) {}

void PassCommand::execute(User & user, std::vector<std::string> args) const
{
	Log::info("PASS command called");
    
	Connection &conn = user.getConnection();

    if(user.isAuthenticated()) {
        conn.sendMessage(JOINED_ALREADYAUTHENTICATED);
        return;
    }

    if (args.size() == 0) {
        conn.sendMessage(JOINED_NEEDMOREPARAMS);
        return;
    }
    
    if (conn.getPassword() != args.at(0)) {
        conn.sendMessage(JOINED_PASSWDMISMATCH);
        return;
    }
	
	Log::debug("User authenticated from " + conn.str());
    user.authenticate();	
}