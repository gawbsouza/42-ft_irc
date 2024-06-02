/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 09:29:49 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 00:32:02 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include "IRCProtocol.hpp"
#include "IRCProtocol.hpp"
#include "../helper/Strings.hpp"
#include "../helper/Log.hpp"

#include <sstream>

NickCommand::NickCommand(UserService & userService, ChannelService & channelService)
						:_userService(userService), _channelService(channelService) {}
						
NickCommand::~NickCommand(void) {}

bool checkNickname(const std::string & username);

void NickCommand::execute(User & user, std::vector<std::string> args) const
{
	Connection &conn = user.getConnection();

	Log::info(NICK_CMD " command called from " + conn.str());

	if(!user.isAuthenticated()) {
        conn.sendMessage(Strings::f(ERR_NOTAUTHENTICATED, NICK_CMD));
        return;
    }

    if (args.size() == 0) {
        conn.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, NICK_CMD));
        return;
    }
    
	std::string nickname = args[NICKNAME_ARG_INDEX];

	if (!checkNickname(nickname)) {
        conn.sendMessage(Strings::f(ERR_ERRONEUSNICKNAME, nickname));
		return;
	}

	if (this->_userService.nickNameExists(nickname)) {
		conn.sendMessage(Strings::f(ERR_NICKNAMEINUSE, NICK_CMD, nickname));
        return;
	}

	if (user.getNickName().size() == 0){
		conn.sendMessage(Strings::f("%s %s\r\n", NICK_CMD, nickname));
	}

	if (user.getNickName().size() > 0)
	{
		std::string message = Strings::f(":%s %s %s\r\n", user.getNickName(), NICK_CMD, nickname);
		conn.sendMessage(message);
		
		std::vector<Channel *> channels = this->_channelService.getChannelsFromNickname(user.getNickName());
		
		for(size_t i = 0; i < channels.size(); i++)
		{
			Channel *channel = channels.at(i);
			if (channel != NULL) {
				channel->broadCast(user, message);
			}
		}
	}

	user.setNickName(nickname);
	   
	if(user.isRegistered()) {
		conn.sendMessage(Strings::f(RPL_WELCOMEMESSAGE, nickname, nickname));
    }
	
	Log::notice("User set \"" + nickname + "\" nickname from " + conn.str());
}

bool checkNickname(const std::string & nickname)
{
	if (nickname.size() > NICKNAME_MAX_LENGTH){
        return false;
	}

	if (!Strings::isOnPattern(nickname, ALPHA_PATTERN XCHARS_PATTERN, 0)){
		return false;
	}
	
	if (!Strings::isOnPattern(nickname.substr(1), ALPHANUM_PATTERN XCHARS_PATTERN)){
		return false;
	}

	return true;
}