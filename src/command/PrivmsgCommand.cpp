/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 21:29:28 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 15:47:57 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivmsgCommand.hpp"
#include "../helper/Log.hpp"
#include "../helper/Strings.hpp"
#include "IRCProtocol.hpp"

PrivmsgCommand::PrivmsgCommand(UserService & userService, ChannelService & channelService)
                              :_userService(userService), _channelService(channelService) {}
     
PrivmsgCommand::~PrivmsgCommand(void) {}

void PrivmsgCommand::execute(User & user, std::vector<std::string> args) const
{
	Connection &conn = user.getConnection();
    
	Log::info(Strings::f(PRIVMSG_CMD " command called from %s", conn.str()));

    if (user.isRegistered() != REGISTERED) {
        conn.sendMessage(Strings::f(ERR_NOTREGISTERED, PRIVMSG_CMD));
        return;
    }

    if (args.size() < 2) {
        user.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, PRIVMSG_CMD));
        return;
    }

    std::string receiver = args.at(0);
    std::string message  = Strings::join(args.begin() + 1, args.end(), " ");
    message = Strings::removePrefix(message, ":");

    bool isChannel = receiver.at(0) == '#';

    if (isChannel) {
        std::string channelName = receiver.substr(1);
        Channel *channel = this->_channelService.getChannelByName(channelName);

        if (channel == NULL) {
            user.sendMessage(Strings::f(ERR_NOSUCHCHANNEL, user.getNickName(), channelName));
            return;
        }

        if (channel->getUser(user) == NULL) {
            user.sendMessage(Strings::f(ERR_NOTONCHANNEL, user.getNickName(), channelName));
            return;
        }

        channel->broadCast(user, Strings::f(":%s PRIVMSG %s :%s\r\n", user.getNickName(), receiver, message));
        return;
    }

    User * userReceiver = this->_userService.getUserByNickName(receiver);

    if (userReceiver == NULL)  {
        user.sendMessage(Strings::f(ERR_NOSUCHNICK, user.getNickName(), receiver));
        return;
    }

    userReceiver->sendMessage(Strings::f(":%s PRIVMSG %s :%s\r\n", user.getNickName(), receiver, message));
}