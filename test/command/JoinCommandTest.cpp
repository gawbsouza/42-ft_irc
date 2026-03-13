/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommandTest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigravity <antigravity@42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 00:00:00 by antigravity       */
/*   Updated: 2024/06/02 00:00:00 by antigravity       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/command/JoinCommand.hpp"
#include "../../src/service/ChannelService.hpp"
#include "../../src/service/UserService.hpp"
#include "../mock/MockConnection.hpp"

using ::testing::_;
using ::testing::HasSubstr;

class JoinCommandTest : public ::testing::Test {
protected:
    UserService userService;
    ChannelService channelService;
    MockConnection conn;
    User* user;

    void SetUp() override {
        user = new User(conn);
        user->setNickName("tester");
        user->setUserName("tester");
        user->authenticate(); // some commands check registration
        userService.addUser(*user);
    }

    void TearDown() override {
        // user is managed by userService
    }
};

TEST_F(JoinCommandTest, NeedMoreParams) {
    JoinCommand cmd(channelService);
    std::vector<std::string> args;

    EXPECT_CALL(conn, sendMessage(HasSubstr("461"))); // ERR_NEEDMOREPARAMS
    cmd.execute(*user, args);
}

TEST_F(JoinCommandTest, InvalidChannelName) {
    JoinCommand cmd(channelService);
    std::vector<std::string> args;
    args.push_back("invalid"); // Missing #

    EXPECT_CALL(conn, sendMessage(HasSubstr("400"))); // ERR_GENERICERROR or similar
    cmd.execute(*user, args);
}

TEST_F(JoinCommandTest, SuccessfulCreateAndJoin) {
    JoinCommand cmd(channelService);
    std::vector<std::string> args;
    args.push_back("#testchan");

    // Capture the messages sent
    EXPECT_CALL(conn, sendMessage(_)).WillRepeatedly(::testing::Return(1));
    EXPECT_CALL(conn, sendMessage(HasSubstr("JOIN #testchan"))).Times(::testing::AtLeast(1));
    
    cmd.execute(*user, args);

    EXPECT_TRUE(channelService.channelExists("testchan"));
}

TEST_F(JoinCommandTest, JoinExistingChannel) {
    JoinCommand cmd(channelService);
    
    // Setup existing channel with ANOTHER user
    MockConnection *otherConn = new MockConnection();
    User *otherUser = new User(*otherConn);
    otherUser->setNickName("other");
    userService.addUser(*otherUser); // Now it's managed
    
    Channel *existingChannel = new Channel(*otherUser, "existing");
    channelService.addChannel(*existingChannel);

    std::vector<std::string> args;
    args.push_back("#existing");

    EXPECT_CALL(conn, sendMessage(_)).WillRepeatedly(::testing::Return(1));
    EXPECT_CALL(conn, sendMessage(HasSubstr("JOIN #existing"))).Times(::testing::AtLeast(1));
    
    cmd.execute(*user, args);
    
    // otherConn needs to be cleaned up, but otherUser and existingChannel 
    // are managed by services (well, existingChannel is in channelService, 
    // and otherUser is in existingChannel?)
    // Wait, let's see Channel class again.
    delete otherConn;
}
