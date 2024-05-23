/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandlerTest.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 22:21:21 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/22 22:33:31 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/handler/CommandHandler.hpp"
#include "../mock/MockCommand.hpp"

using ::testing::_;

TEST(CommandHandlerTest, InitialState)
{
    UserService service;
    CommandHandler handler(service);

    EXPECT_EQ(0, handler.listCommands().size());
}

TEST(CommandHandlerTest, AddCommand)
{
    UserService service;
    CommandHandler handler(service);
    MockCommand command;
    
    handler.addCommand("TEST1", command);
    handler.addCommand("TEST2", command);

    std::vector<std::string> commands = handler.listCommands();
    
    EXPECT_EQ(2, commands.size());
    EXPECT_EQ("TEST1", commands.at(0));
    EXPECT_EQ("TEST2", commands.at(1));
}

TEST(CommandHandlerTest, AddCommandWithoutUpperCase)
{
    UserService service;
    CommandHandler handler(service);
    MockCommand command;
    
    handler.addCommand("test1", command);
    handler.addCommand("test2", command);

    std::vector<std::string> commands = handler.listCommands();
    
    EXPECT_EQ(2, commands.size());
    EXPECT_EQ("TEST1", commands.at(0));
    EXPECT_EQ("TEST2", commands.at(1));
}

TEST(CommandHandlerTest, ExecuteCommandWithoutUser)
{
    UserService service;
    CommandHandler handler(service);
    
    Connection conn(0, 0, "", 0, "");
    Event event(EVENT_MESSAGE, conn, "");

    EXPECT_NO_THROW(handler.handle(event));

}

TEST(CommandHandlerTest, ExecuteOneCommandWithoutArgs)
{
    Connection conn(0, 0, "", 0, "");
    Event event(EVENT_MESSAGE, conn, "TEST");
    User user(conn);
    
    UserService service;
    service.addUser(user);
    
    CommandHandler handler(service);
    
    MockCommand command;
    handler.addCommand("TEST", command);

    std::vector<std::string> args;

    EXPECT_CALL(command, execute(_, args));
    
    handler.handle(event);
}

TEST(CommandHandlerTest, NotExecuteCommandWhenNameIsInvalid)
{
    Connection conn(0, 0, "", 0, "");
    Event event(EVENT_MESSAGE, conn, "TEST@");
    User user(conn);
    
    UserService service;
    service.addUser(user);
    
    CommandHandler handler(service);
    
    MockCommand command;
    handler.addCommand("TEST", command);

    EXPECT_CALL(command, execute(_, _)).Times(0);

    std::vector<std::string> args;
    
    handler.handle(event);
}

TEST(CommandHandlerTest, ExecuteMultiplesCommandsWithoutArgs)
{
    Connection conn(0, 0, "", 0, "");
    Event event(EVENT_MESSAGE, conn, "TEST1 \nTEST2   \r\nTEST3");
    User user(conn);
    
    UserService service;
    service.addUser(user);
    
    CommandHandler handler(service);
    
    MockCommand command1;
    MockCommand command2;
    MockCommand command3;
    
    handler.addCommand("TEST1", command1);
    handler.addCommand("TEST2", command2);
    handler.addCommand("TEST3", command3);

    std::vector<std::string> args;

    EXPECT_CALL(command1, execute(_, args));
    EXPECT_CALL(command2, execute(_, args));
    EXPECT_CALL(command3, execute(_, args));
    
    handler.handle(event);
}

TEST(CommandHandlerTest, ExecuteOneCommandWithArgs)
{
    Connection conn(0, 0, "", 0, "");
    Event event(EVENT_MESSAGE, conn, "TEST VAL val :#$   ");
    User user(conn);
    
    UserService service;
    service.addUser(user);
    
    CommandHandler handler(service);
    
    MockCommand command;
    handler.addCommand("TEST", command);

    std::vector<std::string> args;

    args.push_back("VAL");
    args.push_back("val");
    args.push_back(":#$");

    EXPECT_CALL(command, execute(_, args));
    
    handler.handle(event);
}

TEST(CommandHandlerTest, ExecuteMultiplesCommandsWithArgs)
{
    Connection conn(0, 0, "", 0, "");
    Event event(EVENT_MESSAGE, conn, "TEST1 VAL1   VAL2 \nTEST2 VAL1 VAL2  \r\nTEST3  VAL1 VAL2");
    User user(conn);
    
    UserService service;
    service.addUser(user);
    
    CommandHandler handler(service);
    
    MockCommand command1;
    MockCommand command2;
    MockCommand command3;
    
    handler.addCommand("TEST1", command1);
    handler.addCommand("TEST2", command2);
    handler.addCommand("TEST3", command3);

    std::vector<std::string> args;

    args.push_back("VAL1");
    args.push_back("VAL2");

    EXPECT_CALL(command1, execute(_, args));
    EXPECT_CALL(command2, execute(_, args));
    EXPECT_CALL(command3, execute(_, args));
    
    handler.handle(event);
}