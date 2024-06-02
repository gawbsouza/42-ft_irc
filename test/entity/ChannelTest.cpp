/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelTest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 20:17:52 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:15:31 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../src/entity/Channel.hpp"

TEST(ChannelTest, DefaultStateAndGetters)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel(user, "channelTest");

    ChannelUser *chUser = channel.getUser(user);

    // User
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_OPERATOR, chUser->type);
    EXPECT_EQ(&user, &chUser->user);
    // Others
    EXPECT_EQ("channelTest", channel.getName());
    EXPECT_EQ("", channel.getTopic());
    EXPECT_EQ(0, channel.getLimit());
    EXPECT_FALSE(channel.isInviteOnly());
    EXPECT_TRUE(channel.isRestrictTopicChange());
    EXPECT_EQ("", channel.getPassword());
    EXPECT_FALSE(channel.hasPassword());
}

TEST(ChannelTest, GetAndSetTopic)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel(user, "channelTest");

    EXPECT_EQ("", channel.getTopic());
    channel.setTopic("Some other topic");
    EXPECT_EQ("Some other topic", channel.getTopic());
}

TEST(ChannelTest, GetAndSetLimit)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel(user, "channelTest");

    EXPECT_EQ(0, channel.getLimit());
    channel.setLimit(100);
    EXPECT_EQ(100, channel.getLimit());
}

TEST(ChannelTest, NewUserIsAddedAsCommon)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    Channel channel(user1, "channelTest");
    channel.addUser(user2);
    ChannelUser *chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_COMMON, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);
}

TEST(ChannelTest, GetUserNotAddedMustReturnNull)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    Channel channel(user1, "channelTest");
    ChannelUser *chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser == NULL);
}

TEST(ChannelTest, GetAndSetPassword)
{
    Connection conn1(0, 0, "", 0, "");
    User user1(conn1);

    Channel channel(user1, "channelTest");
    
    EXPECT_EQ("", channel.getPassword());
    EXPECT_FALSE(channel.hasPassword());

    channel.setPassword("my new password");

    EXPECT_EQ("my new password", channel.getPassword());
    EXPECT_TRUE(channel.hasPassword());
}

TEST(ChannelTest, GetAndSetInviteMode)
{
    Connection conn1(0, 0, "", 0, "");
    User user1(conn1);

    Channel channel(user1, "channelTest");
    
    EXPECT_FALSE(channel.isInviteOnly());
    channel.setInviteOnly(true);
    EXPECT_TRUE(channel.isInviteOnly());
    channel.setInviteOnly(false);
    EXPECT_FALSE(channel.isInviteOnly());
}

TEST(ChannelTest, GetAndSetRestrictTopicChange)
{
    Connection conn1(0, 0, "", 0, "");
    User user1(conn1);

    Channel channel(user1, "channelTest");
    
    EXPECT_TRUE(channel.isRestrictTopicChange());
    channel.setRestrictTopicChange(false);
    EXPECT_FALSE(channel.isRestrictTopicChange());
    channel.setRestrictTopicChange(true);
    EXPECT_TRUE(channel.isRestrictTopicChange());
}

TEST(ChannelTest, Invite)
{
    Connection conn1(0, 0, "", 0, "");
    User user1(conn1);

    Channel channel(user1, "channelTest");
    
    EXPECT_FALSE(channel.isInvited("MyTestNick"));
    EXPECT_FALSE(channel.isInvited("MyNickTest1"));
    EXPECT_FALSE(channel.isInvited("MyAnotherTest2"));
    EXPECT_FALSE(channel.isInvited("SomeOneNick"));
    channel.invite("MyTestNick");
    channel.invite("MyNickTest1");
    channel.invite("MyAnotherTest2");
    channel.invite("SomeOneNick");
    EXPECT_TRUE(channel.isInvited("MyTestNick"));
    EXPECT_TRUE(channel.isInvited("MyNickTest1"));
    EXPECT_TRUE(channel.isInvited("MyAnotherTest2"));
    EXPECT_TRUE(channel.isInvited("SomeOneNick"));
}

TEST(ChannelTest, SetOperatorWithCommonStatusMustUpdateToOperator)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    Channel channel(user1, "channelTest");
    channel.addUser(user2);
    ChannelUser *chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_COMMON, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);

    channel.setOperator(user2);
    chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_OPERATOR, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);
}

TEST(ChannelTest, SetOperatorByNickNameWithCommonStatusMustUpdateToOperator)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    user2.setNickName("user2");
    Channel channel(user1, "channelTest");
    channel.addUser(user2);
    ChannelUser *chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_COMMON, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);

    channel.setOperator("user2");
    chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_OPERATOR, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);
}

TEST(ChannelTest, RemoveOperatorWithOperatorStatusMustUpdateToCommon)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    Channel channel(user1, "channelTest");
    
    channel.addUser(user2);
    channel.setOperator(user2);
    
    ChannelUser *chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_OPERATOR, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);

    channel.removeOperator(user2);
    chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_COMMON, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);
}

TEST(ChannelTest, RemoveOperatorByNickNameWithOperatorStatusMustUpdateToCommon)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);
    user2.setNickName("user2");

    Channel channel(user1, "channelTest");
    
    channel.addUser(user2);
    channel.setOperator(user2);
    
    ChannelUser *chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_OPERATOR, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);

    channel.removeOperator("user2");
    chUser = channel.getUser(user2);
    
    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(CHANNEL_COMMON, chUser->type);
    EXPECT_EQ(&user2, &chUser->user);
}

TEST(ChannelTest, GetUsers)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    Connection conn3(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);
    User user3(conn3);

    Channel channel(user1, "channelTest");
    
    channel.addUser(user2);
    channel.addUser(user3);
    channel.setOperator(user3);
    
    std::vector<ChannelUser> users = channel.getUsers();

    EXPECT_EQ(3, users.size());
    EXPECT_EQ(&user1, &users.at(0).user);
    EXPECT_EQ(&user2, &users.at(1).user);
    EXPECT_EQ(&user3, &users.at(2).user);
    EXPECT_EQ(CHANNEL_OPERATOR,  users.at(0).type);
    EXPECT_EQ(CHANNEL_COMMON,   users.at(1).type);
    EXPECT_EQ(CHANNEL_OPERATOR, users.at(2).type);
}

TEST(ChannelTest, GetModeStrWithInitialState)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    
    Channel channel(user, "channelTest");
    
    EXPECT_EQ("+t", channel.getModeStr());
}

TEST(ChannelTest, GetModeStrWithInviteOnlyFlag)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    
    Channel channel(user, "channelTest");
    channel.setInviteOnly(true);
    
    EXPECT_EQ("+it", channel.getModeStr());
}

TEST(ChannelTest, GetModeStrWithRestrictTopicFlag)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    
    Channel channel(user, "channelTest");
    channel.setRestrictTopicChange(true);
    
    EXPECT_EQ("+t", channel.getModeStr());
}

TEST(ChannelTest, GetModeStrWithPassword)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    
    Channel channel(user, "channelTest");
    channel.setPassword("test");
    
    EXPECT_EQ("+tk test", channel.getModeStr());
}

TEST(ChannelTest, GetModeStrWithLimit)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    
    Channel channel(user, "channelTest");
    channel.setLimit(10);
    
    EXPECT_EQ("+tl 10", channel.getModeStr());
}

TEST(ChannelTest, GetModeStrWithMultiFlags)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    
    Channel channel(user, "channelTest");
    
    channel.setLimit(10);
    EXPECT_EQ("+tl 10", channel.getModeStr());

    channel.setInviteOnly(true);
    EXPECT_EQ("+itl 10", channel.getModeStr());

    channel.setRestrictTopicChange(false);
    EXPECT_EQ("+il 10", channel.getModeStr());

    channel.setPassword("test");
    EXPECT_EQ("+ilk 10 test", channel.getModeStr());

    channel.setLimit(0);
    EXPECT_EQ("+ik test", channel.getModeStr());
}

TEST(ChannelTest, RemoveUserFromInviteListWhenAddedOnChannel)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    Channel channel(user1, "channelTest");

    user2.setNickName("UserTest2");

    EXPECT_FALSE(channel.isInvited("UserTest2"));
    channel.invite("UserTest2");
    EXPECT_TRUE(channel.isInvited("UserTest2"));
    channel.addUser(user2);
    EXPECT_FALSE(channel.isInvited("UserTest2"));
}

TEST(ChannelTest, RemoveUserCanRemoveOperator)
{
    Connection conn1(0, 0, "", 0, "");
    User user1(conn1);

    Channel channel(user1, "channelTest");

    channel.removeUser(user1);

    EXPECT_TRUE(channel.getUser(user1) == NULL);
    EXPECT_EQ(0, channel.usersCount());
}

TEST(ChannelTest, RemoveUserCanRemoveAllUsers)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    Channel channel(user1, "channelTest");

    channel.addUser(user2);

    EXPECT_EQ(2, channel.usersCount());
    channel.removeUser(user1);
    channel.removeUser(user2);
    EXPECT_EQ(0, channel.usersCount());
}

TEST(ChannelTest, GetUserByNickName)
{
    Connection conn1(0, 0, "", 0, "");
    User user1(conn1);
    
    user1.setNickName("testeUser");
    
    Channel channel(user1, "channelTest");
    ChannelUser *chUser = channel.getUser("testeUser");

    EXPECT_TRUE(chUser != NULL);
    EXPECT_EQ(&user1, &chUser->user);
}

TEST(ChannelTest, RemoveUserByNickName)
{
    Connection conn1(0, 0, "", 0, "");
    User user1(conn1);
    user1.setNickName("testeUser");
    
    Channel channel(user1, "channelTest");

    EXPECT_EQ(1, channel.usersCount());
    channel.removeUser("testeUser");
    EXPECT_EQ(0, channel.usersCount());
}