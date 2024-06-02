/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 23:20:58 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 00:46:15 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserHandler.hpp"
#include "../helper/Log.hpp"
#include "../helper/Strings.hpp"

#include <sstream>

UserHandler::UserHandler(UserService & userService, ChannelService & channelService)
                        : _userService(userService), _channelService(channelService) {}

UserHandler::~UserHandler(void) {}
    
void UserHandler::handle(Event event)
{
    Connection &conn = event.conn;

    if (event.type == EVENT_CONNECT)
    {
        User *user = new User(conn);
        this->_userService.addUser(*user);
        Log::notice("User created for " + conn.str());
    }

    if (event.type == EVENT_DISCONNECT)
    {
        User *user = this->_userService.getUserByConnection(conn);
        if (user == NULL) {
            return;
        }

        if (user->isRegistered())
        {
            std::vector<Channel *> channels = this->_channelService.getChannelsFromNickname(user->getNickName());

            for(size_t i = 0; i < channels.size(); i++)
            {
                Channel *channel = channels.at(i);
                if (channel == NULL) {
                    continue;
                }
                channel->broadCast(*user, Strings::f(":%s QUIT %s\r\n", user->getNickName(), "Connection closed"));
                channel->removeUser(*user);
            }
        }
        
        conn.close();
        this->_channelService.removeEmptyChannels();
        this->_userService.removeDisconnectedUsers();
    }
}