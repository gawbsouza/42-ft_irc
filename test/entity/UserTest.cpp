/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserTest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 23:03:21 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 16:32:48 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../src/entity/User.hpp"

TEST(UserTest, GettersAndSetters)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);

    EXPECT_EQ(&conn, &user.getConnection());
    EXPECT_EQ("", user.getNickName());
    EXPECT_EQ("", user.getUserName());
    EXPECT_EQ("", user.getRealName());
    EXPECT_FALSE(user.isAuthenticated());
    EXPECT_FALSE(user.isRegistered() == REGISTERED);

    user.setNickName("nickName");
    user.setRealName("realName");
    user.setUserName("userName");
    
    EXPECT_EQ("nickName", user.getNickName());
    EXPECT_EQ("userName", user.getUserName());
    EXPECT_EQ("realName", user.getRealName());

    user.authenticate();
    
    EXPECT_TRUE(user.isAuthenticated());
}

TEST(UserTest, RegisteredLogic)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);

    EXPECT_EQ("", user.getNickName());
    EXPECT_EQ("", user.getUserName());
    EXPECT_EQ("", user.getRealName());
    EXPECT_FALSE(user.isAuthenticated());
    EXPECT_FALSE(user.isRegistered());

    user.setNickName("nickName");
    EXPECT_FALSE(user.isRegistered() == REGISTERED);
    
    user.setRealName("realName");
    EXPECT_FALSE(user.isRegistered() == REGISTERED);
    
    user.setUserName("userName");
    EXPECT_FALSE(user.isRegistered() == REGISTERED);

    user.authenticate();
    EXPECT_TRUE(user.isRegistered() == REGISTERED);
}