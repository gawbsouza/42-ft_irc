/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 20:11:53 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/31 20:29:00 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITE_COMMAND_HPP
# define INVITE_COMMAND_HPP

# include "Command.hpp"
# include "../entity/ChannelService.hpp"
# include "../entity/UserService.hpp"

# define INVITE_CMD "INVITE"

class InviteCommand: public Command
{
    
private:

    UserService    & _userService;
    ChannelService & _channelService;

public:

    InviteCommand(UserService & userService, ChannelService & channelService);
    ~InviteCommand(void);

    void execute(User & user, std::vector<std::string> args) const;
};

#endif