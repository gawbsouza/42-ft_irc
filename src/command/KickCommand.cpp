/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 20:17:41 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/31 21:32:46 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickCommand.hpp"

#include "../helper/Log.hpp"
#include "../helper/Strings.hpp"
#include "IRCProtocol.hpp"

KickCommand::KickCommand(ChannelService & channelService): _channelService(channelService) {}

KickCommand::~KickCommand() {}

void KickCommand::execute(User & user, std::vector<std::string> args) const
{
    Connection &conn = user.getConnection();
    
	Log::info(KICK_CMD " command called from " + conn.str());

    if (!user.isRegistered()) {
        conn.sendMessage(Strings::f(ERR_NOTREGISTERED, KICK_CMD));
        return;
    }

    if (args.size() < 2) {
        user.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, KICK_CMD));
        return;
    }
    
    std::string channelName = args.at(0);

    if (channelName.size() == 0 || channelName.at(0) != '#') {
        user.sendMessage(Strings::f(ERR_GENERICERROR, KICK_CMD, "Wrong channel prefix"));
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

    std::string userToKick = args.at(1);
    std::string message;

    if (args.size() > 2) {
        message = ":" + Strings::removePrefix(Strings::join(args.begin() + 2, args.end(), " "), ":");
    }

    if (channel->getUser(userToKick) == NULL) {
        user.sendMessage(Strings::f(ERR_NOSUCHNICK, user.getNickName(), userToKick));
        return;
    }

    std::string notification = Strings::f(":%s KICK %s %s %s\r\n", user.getNickName(), channelName, userToKick, message);
    user.sendMessage(notification);
    channel->broadCast(user, notification);
    
    channel->removeUser(userToKick);

    if (channel->usersCount() == 0) {
        this->_channelService.removeChannel(*channel);
        delete channel;
        Log::info("Channel " + channelName + " removed because it's empty");
    }
    
}

