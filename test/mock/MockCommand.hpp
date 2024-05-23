/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 18:46:38 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/22 18:47:00 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gmock/gmock.h>

#include "../../src/command/Command.hpp"

class MockCommand: public Command
{
public:
    MOCK_METHOD (void, execute, (User & user, std::vector<std::string> args), (const, override) );
};