/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 21:50:26 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/30 14:10:23 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include "../entity/ChannelService.hpp"
#include "../helper/Strings.hpp"
#include "IRCProtocol.hpp"

#include "stdlib.h"

typedef enum
{
    UNCHANGED,
    ADD,
    REMOVE
} modeFlagStatus;

ModeCommand::ModeCommand(ChannelService & channelService): _channelService(channelService) {}
ModeCommand::~ModeCommand() {}

void setChannelModes(User & user, Channel * channel, const std::string & modes, const std::vector<std::string> & args);

void ModeCommand::execute(User & user, std::vector<std::string> args) const
{
    if (!user.isRegistered()) {
        user.sendMessage(Strings::f(ERR_GENERICERROR, MODE_CMD, "You are not registered"));
        return;
    }

    if (args.size() == 0) {
        user.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, MODE_CMD));
        return;
    }

    std::string channelName = args.at(0);

    if (channelName.size() == 0 || channelName.at(0) != '#') {
        return;
    }
    
    Channel *channel = this->_channelService.getChannelByName(channelName.substr(1));

    if (channel == NULL) {
        user.sendMessage(Strings::f(ERR_NOSUCHCHANNEL, user.getNickName(), channelName));
        return;
    }

    ChannelUser *chUser = channel->getUser(user); 

    if(chUser == NULL) {
        user.sendMessage(Strings::f(ERR_NOTONCHANNEL, user.getNickName(), channelName));
        return;
    }
    
    if (args.size() == 1) {
        user.sendMessage(Strings::f(RPL_CHANNELMODEIS, user.getNickName(), channelName, channel->getModeStr()));
        return;
    }

    if (chUser->type != CHANNEL_OPERATOR) {
        user.sendMessage(Strings::f(ERR_CHANOPRIVSNEEDED, user.getNickName(), channelName));
        return;
    }
    
    std::string modes = args.at(1);
    std::vector<std::string> argsToMode(args.begin() + 2, args.end());
    
    setChannelModes(user, channel, modes, argsToMode);
}

void setChannelModes(User & user, Channel * channel, const std::string & modes, const std::vector<std::string> & args)
{
    std::string::const_iterator it;

    modeFlagStatus modeOperation = UNCHANGED;

    modeFlagStatus inviteMode = UNCHANGED;
    modeFlagStatus passwordMode = UNCHANGED;
    modeFlagStatus limitMode = UNCHANGED;
    modeFlagStatus topicMode = UNCHANGED;
    modeFlagStatus operatorMode = UNCHANGED;

    std::string passwordValue = "";
    std::string operatorName = "";
    int limitValue = 0;

    int passwordArgValueIndex = -1;
    int operatorArgNameIndex  = -1;
    int limitArgValueIndex    = -1;

    size_t argsUsed = 0;

    for (it = modes.begin(); it != modes.end(); it++)
    {
        char token = *it;

        if (token == '+') { modeOperation = ADD; }
        if (token == '-') { modeOperation = REMOVE;}

        if (modeOperation == UNCHANGED) { continue; }

        if (token == 'i') { inviteMode   = (modeOperation == ADD)? ADD : REMOVE; }
        if (token == 't') { topicMode    = (modeOperation == ADD)? ADD : REMOVE; }
        if (token == 'k') { passwordMode = (modeOperation == ADD)? ADD : REMOVE; }
        if (token == 'l') { limitMode    = (modeOperation == ADD)? ADD : REMOVE; }
        if (token == 'o') { operatorMode = (modeOperation == ADD)? ADD : REMOVE; }

        // password args
        if (token == 'k' && passwordMode == ADD && argsUsed < args.size())
        {
            if (passwordArgValueIndex == -1) { 
                passwordArgValueIndex = argsUsed;
                argsUsed++;
            }
            passwordValue = args.at(passwordArgValueIndex);
        }

        // operator args
        if (token == 'o' && operatorMode != UNCHANGED && argsUsed < args.size())
        {
            if (operatorArgNameIndex == -1) { 
                operatorArgNameIndex = argsUsed;
                argsUsed++;
            }
            operatorName = args.at(operatorArgNameIndex);
        }

        // limit args
        if (token == 'l' && limitMode == ADD && argsUsed < args.size())
        {
            if (limitArgValueIndex == -1) { 
                limitArgValueIndex = argsUsed;
                argsUsed++;
            }
            limitValue = atoi(args.at(limitArgValueIndex).c_str());
        }
    }

    std::string channelName = "#" + channel->getName();
    
    if (operatorMode != UNCHANGED)
    {
        ChannelUser *operatorUser = channel->getUser(operatorName);
        
        if (operatorUser == NULL) {
            user.sendMessage(Strings::f(ERR_NOSUCHNICK, user.getNickName(), operatorName));
            return;
        }

        if (operatorMode == ADD)    { channel->setOperator(operatorName); }
        if (operatorMode == REMOVE) { channel->removeOperator(operatorName); }

        // notificar quem virou operator
    }
    
    if (inviteMode == ADD)    { channel->setInviteOnly(true);  }
    if (inviteMode == REMOVE) { channel->setInviteOnly(false); }

    if (topicMode == ADD)    { channel->setRestrictTopicChange(true);  }
    if (topicMode == REMOVE) { channel->setRestrictTopicChange(false); }

    if (limitMode == ADD && limitValue > 0 ) { channel->setLimit(limitValue);  }
    if (limitMode == REMOVE ) { channel->setLimit(0);  }

    if (passwordMode == ADD && passwordValue.size() != 0 ) { channel->setPassword(passwordValue);  }
    if (passwordMode == REMOVE ) { channel->setPassword("");  }
    
    user.sendMessage(Strings::f(RPL_CHANNELMODEIS, user.getNickName(), channelName, channel->getModeStr()));
    channel->broadCast(user, Strings::f(RPL_CHANNELMODEIS, user.getNickName(), channelName, channel->getModeStr()));
}
