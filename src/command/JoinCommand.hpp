/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 20:40:51 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:44:08 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

# include "Command.hpp"
# include "../entity/UserService.hpp"
# include "../entity/ChannelService.hpp"

# define JOIN_CMD "JOIN"

# define CHANNEL_ARG_INDEX		0
# define CHANNEL_KEY_ARG_INDEX	1	
# define CHANNEL_MIN_LENGTH		5
# define CHANNEL_MAX_LENGTH		20

class JoinCommand: public Command
{
    
private:

    ChannelService 	& _service;

public:

    JoinCommand(ChannelService & service);
    ~JoinCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif