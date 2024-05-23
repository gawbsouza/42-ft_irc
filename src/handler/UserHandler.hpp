/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 23:09:17 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/21 23:21:52 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HANDLER_HPP
# define USER_HANDLER_HPP

#include "../entity/UserService.hpp"
#include "EventHandler.hpp"

class UserHandler: public EventHandler
{
    
private:

    UserService & _service;

public:

    UserHandler(UserService & service);
    ~UserHandler(void);
    
    void handle(Event event);
    
};

#endif