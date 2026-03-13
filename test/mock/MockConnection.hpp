/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 00:00:00 by antigravity       */
/*   Updated: 2024/06/02 00:00:00 by antigravity       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOCK_CONNECTION_HPP
# define MOCK_CONNECTION_HPP

# include <gmock/gmock.h>
# include "../../src/entity/Connection.hpp"

class MockConnection : public Connection
{
public:

    MockConnection() : Connection(0, -1, "127.0.0.1", 0, "") {}
    MockConnection(int id) : Connection(id, -1, "127.0.0.1", 0, "") {}
    ~MockConnection() {}

    MOCK_METHOD(size_t, sendMessage, (const std::string & msg), (override));
    MOCK_METHOD(std::string, readMessage, (), (override));
};

#endif
