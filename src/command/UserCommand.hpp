/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 20:27:38 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 02:27:34 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_COMMAND_HPP
# define USER_COMMAND_HPP

# include "Command.hpp"
# include "../service/UserService.hpp"

# define USER_CMD "USER"

# define USERNAME_ARG_INDEX      0
# define REALNAME_ARG_INDEX      3
# define REALNAME_MAX_LENGTH     60
# define USERNAME_MAX_LENGTH     30

class UserCommand: public Command
{

public:

    UserCommand(void);
    ~UserCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif