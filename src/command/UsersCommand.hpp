/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UsersCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 19:40:43 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/26 11:04:04 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERS_COMMAND_HPP
# define USERS_COMMAND_HPP

#include "../entity/UserService.hpp"
#include "../command/Command.hpp"

class UsersCommand: public Command
{
private:

    UserService & _service;

public:

    UsersCommand(UserService & service);
    ~UsersCommand();
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif