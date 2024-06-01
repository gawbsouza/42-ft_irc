/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 20:11:53 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/31 20:29:00 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_COMMAND_HPP
# define KICK_COMMAND_HPP

# include "Command.hpp"
# include "../entity/ChannelService.hpp"

# define KICK_CMD "KICK"

class KickCommand: public Command
{
    
private:

    ChannelService & _channelService;

public:

    KickCommand(ChannelService & channelService);
    ~KickCommand();

    void execute(User & user, std::vector<std::string> args) const;
};

#endif