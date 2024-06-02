/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:51:58 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 00:33:21 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QuitCommand.hpp"
#include "../helper/Log.hpp"
#include "../helper/Strings.hpp"
#include "IRCProtocol.hpp"

QuitCommand::QuitCommand( ChannelService & channelService): _channelService(channelService) {}
QuitCommand::~QuitCommand(void) {}

void QuitCommand::execute(User & user, std::vector<std::string> args) const
{
	Connection &conn = user.getConnection();

	Log::info(QUIT_CMD " command called from " + conn.str());

    if (!user.isRegistered()) {
        conn.close();
        return;
    }

    std::string message;
    
    if (args.size() > 0) {
        message = ":" + Strings::removePrefix(Strings::join(args.begin(), args.end(), " "), ":");
    }

    std::vector<Channel *> channels = this->_channelService.getChannelsFromNickname(user.getNickName());

    for(size_t i = 0; i < channels.size(); i++)
    {
        Channel *channel = channels.at(i);
        if (channel == NULL) {
            continue;
        }
        channel->broadCast(user, Strings::f(":%s QUIT %s\r\n", user.getNickName(), message));
        channel->removeUser(user);
    }

    conn.close();
    this->_channelService.removeEmptyChannels();
}