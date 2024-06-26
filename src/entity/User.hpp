/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:04:06 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 19:58:39 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "Connection.hpp"

# include <string>

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
    bool isRegistered(void);

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