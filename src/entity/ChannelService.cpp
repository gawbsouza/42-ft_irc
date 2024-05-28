/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 22:43:50 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/27 23:18:15 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelService.hpp"

ChannelService::ChannelService() {}
ChannelService::~ChannelService() {}

void ChannelService::addChannel(Channel & channel)
{
    this->_channels.push_back(&channel);
}

void ChannelService::removeChannel(Channel & channel)
{
    this->_channels.remove(&channel);
}

bool ChannelService::channelExists(const std::string & channelName)
{
    return this->getChannelByName(channelName) != NULL;
}

size_t ChannelService::channelsCount(void)
{
    return this->_channels.size();
}

Channel * ChannelService::getChannelByName(const std::string & channelName)
{
    std::list<Channel *> copy = this->_channels;
    std::list<Channel *>::iterator it;

    for(it = copy.begin(); it != copy.end(); it++)
    {
        Channel *ch = *it;
        if (ch != NULL && ch->getName() == channelName) {
            return ch;
        }
    }

    return NULL;
}

std::vector<Channel *> ChannelService::getChannels(void)
{
    std::vector<Channel *> channels(this->_channels.begin(), this->_channels.end());
    return channels;
}