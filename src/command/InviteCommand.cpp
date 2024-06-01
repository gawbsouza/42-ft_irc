/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 20:17:41 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/31 21:32:46 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"

#include "../helper/Log.hpp"
#include "../helper/Strings.hpp"
#include "IRCProtocol.hpp"

InviteCommand::InviteCommand(UserService & userService, ChannelService & channelService)
                              :_userService(userService), _channelService(channelService) {}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(User & user, std::vector<std::string> args) const
{
    Connection &conn = user.getConnection();
    
	Log::info(INVITE_CMD " command called from " + conn.str());

    if (!user.isRegistered()) {
        conn.sendMessage(Strings::f(ERR_NOTREGISTERED, INVITE_CMD));
        return;
    }

    if (args.size() < 2) {
        user.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, INVITE_CMD));
        return;
    }
    
    std::string channelName = args.at(1);

    if (channelName.size() == 0 || channelName.at(0) != '#') {
        user.sendMessage(Strings::f(ERR_GENERICERROR, INVITE_CMD, "Wrong channel prefix"));
        return;
    }
    
    Channel *channel = this->_channelService.getChannelByName(channelName.substr(1));

    if (channel == NULL) {
        user.sendMessage(Strings::f(ERR_NOSUCHCHANNEL, user.getNickName(), channelName));
        return;
    }

    ChannelUser *chUser = channel->getUser(user.getNickName());

    if (chUser == NULL) {
        user.sendMessage(Strings::f(ERR_NOTONCHANNEL, user.getNickName(), channelName));
        return;
    }

    if (chUser->type != CHANNEL_OPERATOR) {
        user.sendMessage(Strings::f(ERR_CHANOPRIVSNEEDED, user.getNickName(), channelName));
        return;
    }

    std::string nicknameToInvite = args.at(0);

    if (_userService.getUserByNickName(nicknameToInvite) == NULL) {
        user.sendMessage(Strings::f(ERR_NOSUCHNICK, user.getNickName(), nicknameToInvite));
        return;
    }
    
    channel->invite(nicknameToInvite);
	user.sendMessage(Strings::f(RPL_INVITING, user.getNickName(), nicknameToInvite, channelName));

	User * userToInvite = _userService.getUserByNickName(nicknameToInvite);
	userToInvite->sendMessage(Strings::f(":%s INVITE %s :%s\r\n", user.getNickName(), nicknameToInvite, channelName));
}