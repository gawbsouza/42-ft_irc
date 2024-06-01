/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 21:26:09 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/30 21:33:39 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_COMMAND_HPP
# define TOPIC_COMMAND_HPP

# include "Command.hpp"
# include "../entity/UserService.hpp"
# include "../entity/ChannelService.hpp"

# define TOPIC_CMD "TOPIC"

class TopicCommand: public Command
{

private:

    ChannelService & _channelService;

public:

    TopicCommand(ChannelService & channelService);
    ~TopicCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif