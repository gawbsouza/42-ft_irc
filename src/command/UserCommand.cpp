/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 20:27:33 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 02:27:49 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"
#include "IRCProtocol.hpp"
#include "IRCProtocol.hpp"
#include "../helper/Strings.hpp"
#include "../helper/Log.hpp"

#include <sstream>

UserCommand::UserCommand(void) {}
UserCommand::~UserCommand(void) {}

bool checkUsername(const std::string & username);
bool checkRealname(const std::string & realname);

void UserCommand::execute(User & user, std::vector<std::string> args) const
{
	Connection &conn = user.getConnection();

	Log::info(USER_CMD " command called from " + conn.str());

	if(!user.isAuthenticated()) {
        conn.sendMessage(Strings::f(ERR_NOTAUTHENTICATED, USER_CMD));
        return;
    }

    if (args.size() < 4) {
        conn.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, USER_CMD));
        return;
    }

	std::string username = args[USERNAME_ARG_INDEX];
    
	if (!checkUsername(username)) {
        conn.sendMessage(Strings::f(ERR_GENERICERROR, USER_CMD, "Username contains invalid characters"));
		return;
	}

	std::string realname = Strings::removePrefix(args[REALNAME_ARG_INDEX] , ":");

	if (args.size() > 4) {
		std::string remainingNameParts = Strings::join(args.begin() + REALNAME_ARG_INDEX + 1, args.end(), " ");
		realname.append(" ");
		realname.append(remainingNameParts);
	}
	
	if (!checkRealname(realname)){
		conn.sendMessage(Strings::f(ERR_GENERICERROR, USER_CMD, "Username contains invalid characters"));
		return;
	}

    if(user.isRegistered()) {
        conn.sendMessage(Strings::f(ERR_ALREADYREGISTERED, USER_CMD));
        return;
    }
	
	std::string nickname = user.getNickName();
	Strings::truncateBySize(username, USERNAME_MAX_LENGTH);
	Strings::truncateBySize(realname, REALNAME_MAX_LENGTH);

	user.setUserName(username);
	user.setRealName(realname);
	
    if(user.isRegistered()) {
		conn.sendMessage(Strings::f(RPL_WELCOMEMESSAGE, nickname, nickname));
    }
	
	Log::notice("User set \"" + username + "\" username and \"" + realname + "\" realname from " + conn.str());
}

bool checkUsername(const std::string & username)
{
	if (!Strings::isOnPattern(username, ALPHANUM_PATTERN, 0)){
		return false;
	}
	
	if (!Strings::isOnPattern(username, ALPHANUM_PATTERN XCHARS_PATTERN)){
		return false;
	}
	
	return true;
}

bool checkRealname(const std::string & realname)
{
	std::string::const_iterator iter = realname.begin();
	std::string::const_iterator end = realname.end();
	
	for (; iter != end; iter++){
		if(!std::isprint(*iter)){
			return false;
		}
	}

	return true;
}