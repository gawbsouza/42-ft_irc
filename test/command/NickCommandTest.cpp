/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommandTest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigravity <antigravity@42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 00:00:00 by antigravity       */
/*   Updated: 2024/06/02 00:00:00 by antigravity       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/command/NickCommand.hpp"
#include "../../src/service/UserService.hpp"
#include "../../src/service/ChannelService.hpp"
#include "../mock/MockConnection.hpp"

using ::testing::_;
using ::testing::HasSubstr;

class NickCommandTest : public ::testing::Test {
protected:
    UserService userService;
    ChannelService channelService;
    MockConnection conn;
    User* user;

    void SetUp() override {
        user = new User(conn);
        user->setNickName("tester");
        user->setUserName("tester");
        // By default, authentication might be needed for some operations
        user->authenticate();
    }

    void TearDown() override {
        delete user;
    }
};

TEST_F(NickCommandTest, NeedMoreParams) {
    NickCommand cmd(userService, channelService);
    std::vector<std::string> args;

    EXPECT_CALL(conn, sendMessage(HasSubstr("461"))); // ERR_NEEDMOREPARAMS
    cmd.execute(*user, args);
}

TEST_F(NickCommandTest, SuccessfulNickSet) {
    NickCommand cmd(userService, channelService);
    std::vector<std::string> args;
    args.push_back("newnick");

    // Expecting some confirmation or welcome if registration completes
    EXPECT_CALL(conn, sendMessage(_)).Times(::testing::AtLeast(0));
    cmd.execute(*user, args);

    EXPECT_EQ("newnick", user->getNickName());
}

TEST_F(NickCommandTest, NicknameInUse) {
    User otherUser(conn);
    otherUser.setNickName("taken");
    userService.addUser(otherUser);

    NickCommand cmd(userService, channelService);
    std::vector<std::string> args;
    args.push_back("taken");

    EXPECT_CALL(conn, sendMessage(HasSubstr("433"))); // ERR_NICKNAMEINUSE
    cmd.execute(*user, args);
}
