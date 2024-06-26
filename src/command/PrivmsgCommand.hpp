/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 21:26:09 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:52:15 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_COMMAND_HPP
# define PRIVMSG_COMMAND_HPP

# include "Command.hpp"
# include "../service/UserService.hpp"
# include "../service/ChannelService.hpp"

# define PRIVMSG_CMD "PRIVMSG"

class PrivmsgCommand: public Command
{
private:

    UserService    & _userService;
    ChannelService & _channelService;

public:

    PrivmsgCommand(UserService & userService, ChannelService & channelService);
    ~PrivmsgCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
};

#endif