/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserServiceTest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 23:25:47 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:56:08 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../src/service/UserService.hpp"

TEST(UserServiceTest, InitialGetter)
{
    UserService service;

    EXPECT_EQ(0, service.usersCount());
}

TEST(UserServiceTest, AddUsers)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    Connection conn3(2, 2, "", 0, "");
    User user1(conn1);
    User user2(conn2);
    User user3(conn3);

    UserService service;

    EXPECT_EQ(0, service.usersCount());

    service.addUser(user1);
    service.addUser(user2);
    service.addUser(user3);

    EXPECT_EQ(3, service.usersCount());
}

TEST(UserServiceTest, RemoveUsers)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    UserService service;

    service.addUser(user1);
    service.addUser(user2);

    EXPECT_EQ(2, service.usersCount());

    service.removeUser(user2);
    EXPECT_EQ(1, service.usersCount());
}

TEST(UserServiceTest, RemoveUserNotAdded)
{
    Connection conn1(0, 0, "", 0, "");
    Connection conn2(1, 1, "", 0, "");
    User user1(conn1);
    User user2(conn2);

    UserService service;

    service.addUser(user1);

    EXPECT_EQ(1, service.usersCount());

    service.removeUser(user2);
    EXPECT_EQ(1, service.usersCount());
}

TEST(UserServiceTest, GetUserByConnection)
{
    Connection conn1(0, 0, "", 0, "");
    User user1(conn1);
    UserService service;
    
    service.addUser(user1);

    User *caught = service.getUserByConnection(conn1);
    
    EXPECT_EQ(caught, &user1);
    
    Connection conn2(1, 1, "", 0, "");
    EXPECT_EQ(NULL, service.getUserByConnection(conn2));
}

TEST(UserServiceTest, GetUserByNickName)
{
    Connection conn1(0, 0, "", 0, "");
    User user(conn1);
    UserService service;
    
    user.setNickName("test_user");
    service.addUser(user);

    User *caught = service.getUserByNickName(user.getNickName());
    EXPECT_EQ(caught, &user);
    
    // Not added previously
    EXPECT_EQ(NULL, service.getUserByNickName(""));
    EXPECT_EQ(NULL, service.getUserByNickName("some_other_nick"));
}

TEST(UserServiceTest, NickNameExists)
{
    Connection conn1(0, 0, "", 0, "");
    User user(conn1);
    UserService service;
    
    user.setNickName("test_user");
    service.addUser(user);

    EXPECT_TRUE(service.nickNameExists("test_user"));
    EXPECT_FALSE(service.nickNameExists(""));
    EXPECT_FALSE(service.nickNameExists("some_other_nick"));
}