/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:32:40 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/22 23:37:48 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "../entity/User.hpp"

#include <vector>
#include <string>

class Command
{
public:

    virtual void execute(User & user, std::vector<std::string> args) const = 0;
};

#endif
