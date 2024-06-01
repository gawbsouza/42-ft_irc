/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:45:03 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 15:21:27 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_SERVICE_HPP
# define USER_SERVICE_HPP

#include "User.hpp"
#include <list>

class UserService
{

private:

    std::list<User *> _users;

public:

    UserService();
    ~UserService();

    void addUser(User & user);
    void removeUser(User & user);

    std::list<User *> getUsers() const;
    
    size_t usersCount(void);
    
    User * getUserByConnection(const Connection & conn);
    User * getUserByNickName(const std::string & nick);
    
    bool nickNameExists(const std::string & nickName);
    
    void removeDisconnectedUsers(void);
};

#endif
