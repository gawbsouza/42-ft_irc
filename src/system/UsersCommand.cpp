/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UsersCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 19:44:20 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/26 14:15:42 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UsersCommand.hpp"
#include <iostream>
#include <sstream>

UsersCommand::UsersCommand(UserService & service): _service(service) {}
UsersCommand::~UsersCommand() {}
    
void UsersCommand::execute(User & user, std::vector<std::string> args) const
{
    (void)args;
    
    std::list<User*> users = this->_service.getUsers();
    Connection userConn = user.getConnection();

    std::stringstream ss;
    ss << std::endl << users.size() << " Users Connected to Server" << std::endl; userConn.sendMessage(ss.str());

    std::list<User *>::iterator it;

    for(it = users.begin(); it != users.end(); it++)
    {
        User *user = *it;
        
        if (user == NULL) {
            continue;
        }
        
        Connection & conn = user->getConnection();

        ss.str("");
        if (conn == userConn) {
            ss << " ◉ ";
        } else {
            ss  << "   ";
        }

        if (!user->isAuthenticated() && !user->isRegistered()) {
            ss << " 🔒 ";
        }
        
        if (user->isAuthenticated() && !user->isRegistered()) {
            ss << " 🔑 ";
        } 

        if (user->isAuthenticated() && user->isRegistered()) {
            ss << " 👤 ";
        }

        ss << conn.str() << " - ";
        
        ss << "(" << user->getNickName() << ", " 
            << user->getUserName() << ", "
            << user->getRealName() 
            << ")";
            
        ss << std::endl;

        userConn.sendMessage(ss.str());
    }

    userConn.sendMessage("\n");
    
}