/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 16:58:16 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 15:18:15 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_SERVICE_HPP
# define CHANNEL_SERVICE_HPP

# include "Channel.hpp"

# include <list>
# include <vector>

class ChannelService
{
private:

    std::list<Channel *> _channels;

public:

    ChannelService(void);
    ~ChannelService(void);

    void addChannel(Channel & channel);
    void removeChannel(Channel & channel);

    bool channelExists(const std::string & channelName);

    Channel * getChannelByName(const std::string & channelName);

    std::vector<Channel *> getChannels(void);
    std::vector<Channel *> getChannelsFromNickname(const std::string & nick);

    size_t channelsCount(void);
    void removeEmptyChannels(void);
    
};

#endif