/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 21:54:52 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/31 22:02:41 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_COMMAND_HPP
# define PART_COMMAND_HPP

# include "Command.hpp"
# include "../entity/ChannelService.hpp"

# define PART_CMD "PART"

class PartCommand: public Command
{
    
private:

    ChannelService & _channelService;
    
public:

    PartCommand(ChannelService & channelService);
    ~PartCommand(void);

    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif