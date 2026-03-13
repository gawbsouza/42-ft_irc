/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommandTest.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antigravity <antigravity@42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 00:00:00 by antigravity       */
/*   Updated: 2024/06/02 00:00:00 by antigravity       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/command/PrivmsgCommand.hpp"
#include "../../src/service/UserService.hpp"
#include "../../src/service/ChannelService.hpp"
#include "../mock/MockConnection.hpp"

using ::testing::_;
using ::testing::HasSubstr;

class PrivmsgCommandTest : public ::testing::Test {
protected:
    UserService userService;
    ChannelService channelService;
    MockConnection conn;
    User* user;

    void SetUp() override {
        user = new User(conn);
        user->setNickName("sender");
        user->setUserName("sender");
        user->authenticate();
        user->setRealName("Sender Real"); // Optional but good for completeness
    }

    void TearDown() override {
        delete user;
    }
};

TEST_F(PrivmsgCommandTest, TargetNotFound) {
    PrivmsgCommand cmd(userService, channelService);
    std::vector<std::string> args;
    args.push_back("nonexistent");
    args.push_back("hello");

    EXPECT_CALL(conn, sendMessage(HasSubstr("401"))); // ERR_NOSUCHNICK
    cmd.execute(*user, args);
}

TEST_F(PrivmsgCommandTest, SuccessfulPrivateMessage) {
    MockConnection receiverConn;
    User receiver(receiverConn);
    receiver.setNickName("receiver");
    userService.addUser(receiver);

    PrivmsgCommand cmd(userService, channelService);
    std::vector<std::string> args;
    args.push_back("receiver");
    args.push_back("hello");

    EXPECT_CALL(receiverConn, sendMessage(HasSubstr("hello")));
    cmd.execute(*user, args);
}
