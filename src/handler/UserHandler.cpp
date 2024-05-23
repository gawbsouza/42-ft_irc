/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 23:20:58 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/22 18:27:09 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserHandler.hpp"
#include "../helper/Log.hpp"
#include <sstream>

UserHandler::UserHandler(UserService & service): _service(service) {}

UserHandler::~UserHandler(void) {}
    
void UserHandler::handle(Event event)
{
    Connection &conn = event.conn;

    if (event.type == EVENT_CONNECT)
    {
        User *user = new User(conn);
        this->_service.addUser(*user);
        Log::debug("User created for connection: " + conn.str());
    }

    if (event.type == EVENT_DISCONNECT)
    {
        User *user = this->_service.getUserByConnection(conn);
        if (user == NULL) {
            Log::error("User not found to remove for connection: " + conn.str());
            return;
        }
        this->_service.removeUser(*user);
        delete user;
        Log::debug("User removed for connection: " + conn.str());
    }

    std::stringstream ss; ss << "Users count: " << this->_service.usersCount();
    Log::debug(ss.str());
}