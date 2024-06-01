/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 23:47:54 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 00:26:05 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_COMMAND_HPP
# define QUIT_COMMAND_HPP

#include "Command.hpp"
#include "../entity/ChannelService.hpp"

#define QUIT_CMD "QUIT"

class QuitCommand: public Command
{
    
private:

    ChannelService & _channelService;

public:

    QuitCommand( ChannelService & channelService);
    ~QuitCommand(void);

    void execute(User & user, std::vector<std::string> args) const;
    
};


#endif