/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelsCommand.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:31:47 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:48:29 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_COMMAND_HPP
# define CHANNELS_COMMAND_HPP

# include "../command/Command.hpp"
# include "../entity/ChannelService.hpp"

class ChannelsCommand: public Command
{
    
private:

    ChannelService & _channelService;

public:

    ChannelsCommand(ChannelService & channelService);
    ~ChannelsCommand(void);

    void execute(User & user, std::vector<std::string> args) const;
};


#endif