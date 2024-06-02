/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelsCommand.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:34:28 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:48:44 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelsCommand.hpp"
#include <sstream>

ChannelsCommand::ChannelsCommand(ChannelService & channelService): _channelService(channelService) {}
ChannelsCommand::~ChannelsCommand(void) {}

void ChannelsCommand::execute(User & user, std::vector<std::string> args) const
{
    (void)args;
    
    Connection userConn = user.getConnection();
    std::vector<Channel *> channels = this->_channelService.getChannels();

    std::stringstream ss;
    ss << std::endl << channels.size() << " Channels Created on Server" << std::endl << std::endl;
    userConn.sendMessage(ss.str());

    for(size_t i = 0; i < channels.size(); i++)
    {
        Channel *channel = channels.at(i);
        if (channel == NULL) {
            continue;
        }

        std::vector<ChannelUser> users = channel->getUsers();
        
        ss.str("");
        ss  << "(" << users.size() << ") " << "#"+channel->getName() << " {" << channel->getModeStr() << "}" << std::endl;
        userConn.sendMessage(ss.str());

        for(size_t x = 0; x < users.size(); x++)
        {
            User & user = users.at(x).user;
            ChannelUserType type = users.at(x).type;

            ss.str("");
            ss << (userConn == user.getConnection() ? " ◉ " : "   ");
            ss << (type == CHANNEL_OPERATOR ? " 🎩 " : " 👤 ");
            ss << user.getConnection().str() << " - ";
            ss << "(" << user.getNickName() << ", " << user.getUserName() << ", " << user.getRealName()  << ")";
            ss << std::endl;
            
            userConn.sendMessage(ss.str());
        }
    }

    userConn.sendMessage("\r\n");
}
