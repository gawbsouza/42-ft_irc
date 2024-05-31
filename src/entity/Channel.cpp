/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 20:10:34 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/30 21:02:57 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

#include <sstream>

Channel::Channel(User & creator, const std::string name)
{
    ChannelUser channelUser = {
        user: creator,
        type: CHANNEL_OPERATOR
    };

    this->_users.push_back(channelUser);
    this->_name = name;
    this->_topic = "";
    this->_password = "";
    this->_limit = 0;
    this->_inviteOnly = false;
    this->_restrictTopicChange = false;
}

Channel::~Channel() { }

std::string Channel::getName(void)
{
    return this->_name;
}

std::string Channel::getTopic(void)
{
    return this->_topic;
}

size_t Channel::getLimit(void)
{
    return this->_limit;
}

bool Channel::isInviteOnly(void)
{
    return this->_inviteOnly;
}

bool Channel::isRestrictTopicChange(void)
{
    return this->_restrictTopicChange;
}

std::string Channel::getPassword(void)
{
    return this->_password;
}

bool Channel::hasPassword(void)
{
    return this->_password != "";
}

ChannelUser * Channel::getUser(User & user)
{
    std::list<ChannelUser>::iterator it;

    for(it = this->_users.begin(); it != this->_users.end(); it++) {
        ChannelUser chUser = *it;
        if (&chUser.user == &user) {
            return &(*it);
        }
    }

    return NULL;
}

ChannelUser * Channel::getUser(const std::string & nick)
{
    std::list<ChannelUser>::iterator it;

    for(it = this->_users.begin(); it != this->_users.end(); it++) {
        ChannelUser chUser = *it;
        if (chUser.user.getNickName() == nick) {
            return &(*it);
        }
    }

    return NULL;
}


void Channel::setTopic(const std::string & topic)
{
    this->_topic = topic;
}

void Channel::setLimit(size_t limit)
{
    this->_limit = limit;
}

void Channel::addUser(User & user)
{
    if (this->getUser(user) != NULL) {
        return;
    }
    
    this->_inviteList.remove(user.getNickName());

    ChannelUser chUser = {
        user: user,
        type: CHANNEL_COMMON
    };

    this->_users.push_back(chUser);
}

void Channel::removeUser(User & user)
{
    std::list<ChannelUser>::iterator it;
    
    for(it = this->_users.begin(); it != this->_users.end(); it++) {
        ChannelUser chUser = *it;
        if (&chUser.user == &user) {
            this->_users.remove(chUser);
            return;
        }
    }
}

void Channel::removeUser(const std::string & nick)
{
    std::list<ChannelUser>::iterator it;
    
    for(it = this->_users.begin(); it != this->_users.end(); it++) {
        ChannelUser chUser = *it;
        if (chUser.user.getNickName() == nick) {
            this->_users.remove(chUser);
            return;
        }
    }
}

void Channel::setPassword(const std::string & password)
{
    this->_password = password;
}

void Channel::setInviteOnly(bool status)
{
    this->_inviteOnly = status;
}

void Channel::setRestrictTopicChange(bool status)
{
    this->_restrictTopicChange = status;
}

void Channel::invite(const std::string & nick)
{
    if (!this->isInvited(nick)) {
        this->_inviteList.push_back(nick);
    }
}

bool Channel::isInvited(const std::string & nick)
{
    std::list<std::string> copy = this->_inviteList;
    std::list<std::string>::iterator it;

    for (it = copy.begin(); it != copy.end(); it++)
    {
        if (*it == nick) {
            return true;
        }
    }

    return false;
}

void Channel::setOperator(User & user)
{
    ChannelUser *chUser = this->getUser(user);
    
    if (chUser == NULL) {
        return;
    }

    chUser->type = CHANNEL_OPERATOR;
}

void Channel::setOperator(const std::string & nick)
{
    ChannelUser *chUser = this->getUser(nick);
    
    if (chUser == NULL) {
        return;
    }

    chUser->type = CHANNEL_OPERATOR;
}

void Channel::removeOperator(User & user)
{
    ChannelUser *chUser = this->getUser(user);
    
    if (chUser == NULL) {
        return;
    }

    chUser->type = CHANNEL_COMMON;
}

void Channel::removeOperator(const std::string & nick)
{
    ChannelUser *chUser = this->getUser(nick);
    
    if (chUser == NULL) {
        return;
    }

    chUser->type = CHANNEL_COMMON;
}

void Channel::broadCast(User & from, const std::string & message)
{
    std::list<ChannelUser> copy = this->_users;
    std::list<ChannelUser>::iterator it;

    for (it = copy.begin(); it != copy.end(); it++)
    {
        User & user = (*it).user;

        if (&user == &from) {
            continue;
        }

        Connection & conn = user.getConnection();
        conn.sendMessage(message);
    }
}

std::vector<ChannelUser> Channel::getUsers(void)
{
    std::vector<ChannelUser> copy(this->_users.begin(), this->_users.end());
    return copy;
}

size_t Channel::usersCount(void)
{
    return this->_users.size();
}

std::string Channel::getModeStr(void)
{
    std::stringstream modes;
    
    if (this->isInviteOnly()) {
        modes << MODE_INVITE_FLAG;
    }

    if (this->isRestrictTopicChange()) {
        modes << MODE_TOPIC_FLAG;
    }

    if (this->_limit > 0) {
        modes << MODE_LIMIT_FLAG;
    }
    
    if (this->hasPassword()) {
        modes << MODE_PASSWORD_FLAG;
    }

    if (this->_limit > 0) {
        modes << " " << this->getLimit();
    }

    if (this->hasPassword()) {
        modes << " " << this->getPassword();
    }

    if (modes.str().size() > 0) {
        return "+" + modes.str();
    }

    return "";
}