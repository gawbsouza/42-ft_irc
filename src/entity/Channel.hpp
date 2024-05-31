/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 14:19:11 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/30 13:44:17 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"

# include <string>
# include <list>
# include <vector>

# define MODE_PASSWORD_FLAG "k"
# define MODE_LIMIT_FLAG    "l"
# define MODE_TOPIC_FLAG    "t"
# define MODE_INVITE_FLAG   "i"

typedef enum
{
    CHANNEL_COMMON,
    CHANNEL_OPERATOR,
}   ChannelUserType;

struct ChannelUser
{
    User & user;
    ChannelUserType type;

    bool operator==(const ChannelUser & chUser) const {
        return &user == &chUser.user; 
    }
};

class Channel
{
private:

    std::string _name;
    std::string _topic;
    
    std::list<ChannelUser> _users;
    std::list<std::string> _inviteList;

    size_t      _limit;
    bool        _inviteOnly;
    bool        _restrictTopicChange;
    std::string _password;

public:

    Channel(User & creator, const std::string name);
    ~Channel();

    std::string getName(void);
    
    std::string getTopic(void);
    void        setTopic(const std::string & topic);
    
    size_t  getLimit(void);
    void    setLimit(size_t limit);
    
    void          addUser(User & user);
    
    ChannelUser * getUser(User & user);
    void          removeUser(User & user);
    ChannelUser * getUser(const std::string & nick);
    void          removeUser(const std::string & nick);
    
    void        setPassword(const std::string & password);
    std::string getPassword(void);
    bool        hasPassword(void);
    
    void setInviteOnly(bool status);
    bool isInviteOnly(void);
    
    void setRestrictTopicChange(bool status);
    bool isRestrictTopicChange(void);

    void invite(const std::string & nick);
    bool isInvited(const std::string & nick);

    void setOperator(User & user);
    void removeOperator(User & user);
    void setOperator(const std::string & nick);
    void removeOperator(const std::string & nick);
    
    void broadCast(User & from, const std::string & message);

    std::vector<ChannelUser> getUsers(void);
    
    size_t usersCount(void);
    
    std::string getModeStr(void);
};

#endif