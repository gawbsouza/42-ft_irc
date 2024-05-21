/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 09:33:23 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/21 08:37:03 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserService.hpp"

UserService::UserService() {}
UserService::~UserService() {}

size_t UserService::usersCount()
{
    return this->_users.size();
}

void UserService::addUser(User & user)
{
    this->_users.push_back(&user);
}

void UserService::removeUser(User & user)
{
    this->_users.remove(&user);
}

User * UserService::getUserByConnection(const Connection & conn)
{
    std::list<User*> copy = this->_users;
    std::list<User*>::iterator it;

    for (it = copy.begin(); it != copy.end(); it++)
    {
        User *user = *it;
        if (user == NULL) { 
            continue;
        }
        Connection & userConn = user->getConnection();
        if (userConn == conn) {
            return user;
        }
    }
    
    return NULL;
}

User *UserService::getUserByNickName(const std::string & nick)
{
    std::list<User*> copy = this->_users;
    std::list<User*>::iterator it;

    for (it = copy.begin(); it != copy.end(); it++)
    {
        User *user = *it;
        if (user == NULL) { 
            continue;
        }
        if (user->getNickName() == nick) {
            return user;
        }
    }
    
    return NULL;
}


bool UserService::nickNameExists(const std::string & nickName)
{
    if (this->getUserByNickName(nickName) != NULL) {
        return true;
    }
    
    return false;
}
