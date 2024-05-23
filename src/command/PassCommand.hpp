/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:36:23 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/21 22:42:55 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_COMMAND_HPP
# define PASS_COMMAND_HPP

#include "Command.hpp"
#include "../entity/UserService.hpp"

class PassCommand: public Command
{
    
private:

    UserService & _service;

public:

    PassCommand(UserService & userService);
    ~PassCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif