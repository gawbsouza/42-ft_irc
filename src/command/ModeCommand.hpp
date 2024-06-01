/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 23:38:07 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:51:26 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_COMMAND_HPP
# define MODE_COMMAND_HPP

# include "Command.hpp"
# include "../service/ChannelService.hpp"

# define MODE_CMD "MODE"

class ModeCommand: public Command
{

private:

    ChannelService  & _channelService;

public:

    ModeCommand(ChannelService & channelService);
    ~ModeCommand(void);

    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif