/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 09:29:54 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 09:29:57 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_COMMAND_HPP
# define NICK_COMMAND_HPP

#include "Command.hpp"
#include "../entity/UserService.hpp"
#include "../entity/ChannelService.hpp"

#define	NICK_CMD "NICK"

#define NICKNAME_ARG_INDEX      0
#define NICKNAME_MAX_LENGTH     20

class NickCommand: public Command
{
    
private:

    UserService     & _userService;
    ChannelService  & _channelService;
	
	bool _checkNickname(const std::string & username) const;

public:

    NickCommand(UserService & userService, ChannelService & channelService);
    ~NickCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif