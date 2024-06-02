/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 21:29:28 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:30:45 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"
#include "../helper/Log.hpp"
#include "../helper/Strings.hpp"
#include "IRCProtocol.hpp"

TopicCommand::TopicCommand(ChannelService & channelService): _channelService(channelService) {}
     
TopicCommand::~TopicCommand(void) {}

void TopicCommand::execute(User & user, std::vector<std::string> args) const
{
	Connection &conn = user.getConnection();
    
	Log::info(Strings::f(TOPIC_CMD " command called from %s", conn.str()));

    if (!user.isRegistered()) {
        conn.sendMessage(Strings::f(ERR_NOTREGISTERED, TOPIC_CMD));
        return;
    }

	size_t argSize = args.size();

    if (argSize < 1) {
        user.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, TOPIC_CMD));
        return;
    }

    std::string channelPrefixed = args.at(0);
    std::string topic = (argSize > 1) ? Strings::join(args.begin() + 1, args.end(), " ") : ":";
    std::string channelName = Strings::removePrefix(channelPrefixed, "#");
    Channel *channel = this->_channelService.getChannelByName(channelName);

    if (channelPrefixed.at(0) != '#') {
		user.sendMessage(Strings::f(ERR_GENERICERROR, TOPIC_CMD, "Wrong channel prefix"));
		return;	
	}

	if (channel == NULL) {
		user.sendMessage(Strings::f(ERR_NOSUCHCHANNEL, user.getNickName(), channelPrefixed));
		return;
	}

	ChannelUser * userOnChannel = channel->getUser(user);
	
	if (userOnChannel == NULL) {
		user.sendMessage(Strings::f(ERR_NOTONCHANNEL, user.getNickName(), channelPrefixed));
		return;
	}
	
	if (channel->isRestrictTopicChange() && userOnChannel->type != CHANNEL_OPERATOR){
		user.sendMessage(Strings::f(ERR_CHANOPRIVSNEEDED, user.getNickName(), channelPrefixed));
        return;
	}

	channel->setTopic(Strings::removePrefix(topic, ":"));
	user.sendMessage(Strings::f(":%s TOPIC %s %s\r\n", user.getNickName(), channelPrefixed, topic));
	channel->broadCast(user, Strings::f(":%s TOPIC %s %s\r\n", user.getNickName(), channelPrefixed, topic));
}
