/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:45:03 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/20 21:45:24 by gasouza          ###   ########.fr       */
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
    
    size_t usersCount(void);
    
    User * getUserByConnection(const Connection & conn);
    User * getUserByNickName(const std::string & nick);
    
    bool nickNameExists(const std::string & nickName);
    
};

#endif