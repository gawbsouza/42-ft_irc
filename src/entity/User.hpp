/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:04:06 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 16:28:36 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "Connection.hpp"
#include <string>

typedef enum
{
	CONNECTED,
    NICKNAME_SET,
    USERNAME_SET,
    AUTHENTICATED = 4,
    REGISTERED = 7,
}   RegistrationStatus;

class User
{
    
private:

    std::string _nickName;
    std::string _userName;
    std::string _realName;
    
    Connection & _conn;
    
    bool _authenticated;

public:

    User(Connection & conn);
    ~User(void);
    
    Connection & getConnection(void);

    bool isAuthenticated(void);
    int isRegistered(void);

    void authenticate(void);
    void sendMessage(const std::string & message);
    
    std::string getNickName(void);
    std::string getUserName(void);
    std::string getRealName(void);

    void setNickName(const std::string & name);
    void setUserName(const std::string & name);
    void setRealName(const std::string & name);
};

#endif