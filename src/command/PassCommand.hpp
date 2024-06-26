/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:36:23 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 02:24:51 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_COMMAND_HPP
# define PASS_COMMAND_HPP

# include "Command.hpp"
# include "../service/UserService.hpp"

# define PASS_CMD "PASS"

class PassCommand: public Command
{

public:

    PassCommand(void);
    ~PassCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif