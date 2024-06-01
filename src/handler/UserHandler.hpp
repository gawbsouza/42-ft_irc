/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 23:09:17 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:52:15 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HANDLER_HPP
# define USER_HANDLER_HPP

# include "EventHandler.hpp"
# include "../service/UserService.hpp"
# include "../service/ChannelService.hpp"

class UserHandler: public EventHandler
{
    
private:

    UserService     & _userService;
    ChannelService  & _channelService;

public:

    UserHandler(UserService & userService, ChannelService & channelService);
    ~UserHandler(void);
    
    void handle(Event event);
    
};

#endif