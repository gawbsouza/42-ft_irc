/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 14:19:11 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/27 08:53:45 by gasouza          ###   ########.fr       */
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
# define MODE_OPERATOR_FLAG "o"

typedef enum
{
    CHANNEL_COMMON,
    CHANNEL_OPERATOR,
    CHANNEL_CREATOR
} ChannelUserType;

struct ChannelUser
{
    User & user;
    ChannelUserType type;
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
    
    void broadCast(User & from, const std::string & message);

    std::vector<ChannelUser> getUsers(void);
    
    std::string getModeStr(void);
};

#endif