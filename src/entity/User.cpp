/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 09:21:17 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/29 11:29:02 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(Connection & conn): _conn(conn)
{
    this->_authenticated = false;
}

User::~User(void) {}

Connection & User::getConnection(void)
{
    return this->_conn;
}

bool User::isAuthenticated(void)
{
    return this->_authenticated;
}

void User::authenticate(void)
{
    this->_authenticated = true;
}

void User::sendMessage(const std::string & message)
{
    this->_conn.sendMessage(message);
}

bool User::isRegistered(void)
{
    if (!this->isAuthenticated()) { return false; }
    if (this->_nickName.size() == 0) { return false; }
    if (this->_userName.size() == 0) { return false; }
    return true;
}

std::string User::getNickName(void)
{
    return this->_nickName;
}

std::string User::getUserName(void)
{
    return this->_userName;
}

std::string User::getRealName(void)
{
    return this->_realName;
}

void User::setNickName(const std::string & name)
{
    this->_nickName = name;
}

void User::setUserName(const std::string & name)
{
    this->_userName = name;
}

void User::setRealName(const std::string & name)
{
    this->_realName = name;
}