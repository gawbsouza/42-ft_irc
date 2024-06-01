/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 21:59:08 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/31 23:02:41 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCommand.hpp"

#include "../helper/Log.hpp"
#include "../helper/Strings.hpp"
#include "IRCProtocol.hpp"

PartCommand::PartCommand(ChannelService & channelService): _channelService(channelService) {}
PartCommand::~PartCommand(void) {}

void PartCommand::execute(User & user, std::vector<std::string> args) const
{
    Connection &conn = user.getConnection();
    
	Log::info(PART_CMD " command called from " + conn.str());

    if (!user.isRegistered()) {
        conn.sendMessage(Strings::f(ERR_NOTREGISTERED, PART_CMD));
        return;
    }

    if (args.size() < 1) {
        user.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, PART_CMD));
        return;
    }

    std::string channelName = args.at(0);

    if (channelName.size() == 0 || channelName.at(0) != '#') {
        user.sendMessage(Strings::f(ERR_GENERICERROR, PART_CMD, "Wrong channel prefix"));
        return;
    }
    
    Channel *channel = this->_channelService.getChannelByName(channelName.substr(1));

    if (channel == NULL) {
        user.sendMessage(Strings::f(ERR_NOSUCHCHANNEL, user.getNickName(), channelName));
        return;
    }  
    
    std::string message;

    if (args.size() > 1) {
        message = ":" + Strings::removePrefix(Strings::join(args.begin() + 1, args.end(), " "), ":");
    }

    std::string notification = Strings::f(":%s PART %s %s\r\n", user.getNickName(), channelName, message);
    user.sendMessage(notification);
    channel->broadCast(user, notification);
    
    channel->removeUser(user);

    if (channel->usersCount() == 0) {
        this->_channelService.removeChannel(*channel);
        delete channel;
        Log::info("Channel " + channelName + " removed because it's empty");
    }

}