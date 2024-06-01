/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 09:21:17 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 15:43:43 by bluiz-al         ###   ########.fr       */
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

int User::isRegistered(void)
{
	int status = 0;

    if (this->isAuthenticated()) { status += AUTHENTICATED; }
    if (this->_nickName.size() > 0) { status += NICKNAME_SET; }
    if (this->_userName.size() > 0) { status += USERNAME_SET; }
    return status;
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