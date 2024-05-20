/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionTest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 13:05:34 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/19 17:21:34 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../src/entity/Connection.hpp"

#include <fcntl.h>
#include <fstream>

TEST(ConnectionTest, Getters)
{
    int id = 100;
    int fd = 100;
    int port = 8080;
    std::string address = "127.0.0.1";
    std::string password = "12345";
    
    Connection conn(id, fd, address, port, password);
    
    EXPECT_EQ(id, conn.getId());
    EXPECT_EQ(fd, conn.getFd());
    EXPECT_EQ(address, conn.getAddress());
    EXPECT_EQ(port, conn.getPort());
    EXPECT_EQ(password, conn.getPassword());
}

TEST(ConnectionTest, StartOppened)
{
    Connection conn(0, 0, "adress", 0, "pass");
    
    EXPECT_EQ(false, conn.isClosed());
}

TEST(ConnectionTest, Close)
{
    Connection conn(0, 0, "adress", 0, "pass");
    
    EXPECT_EQ(false, conn.isClosed());
    conn.close();
    EXPECT_EQ(true, conn.isClosed());
}

TEST(ConnectionTest, SendMessage)
{
    std::string tempfile = "temp-output-file.txt";
   std::string msg = "Test Message";
   
    int fileFd = open(tempfile.c_str(), O_CREAT | O_TRUNC | O_RDWR);
    Connection conn(0, fileFd, "adress", 0, "pass");
    
    EXPECT_TRUE(fileFd != -1);
    EXPECT_TRUE(conn.sendMessage(msg) > 0);

    close(fileFd);
    unlink(tempfile.c_str());
}

TEST(ConnectionTest, ReadMessage)
{
    std::string tempfile = "temp-input-file.txt";
    std::string msg = "Test Message";
    
    std::ofstream file(tempfile.c_str());
    file << msg;
    file.close();

    int fileFd = open(tempfile.c_str(), O_RDONLY);
    
    Connection conn(0, fileFd, "adress", 0, "pass");
    
    EXPECT_TRUE(fileFd != -1);
    EXPECT_EQ(msg, conn.readMessage());

    close(fileFd);
    unlink(tempfile.c_str());
}

TEST(ConnectionTest, SendMessageWithClosedFdMustCloseConnection)
{
    int invalidFd = 99;
    
    Connection conn(0, invalidFd, "adress", 0, "pass");
    
    EXPECT_FALSE(conn.isClosed());
    EXPECT_EQ("", conn.readMessage());
    EXPECT_TRUE(conn.isClosed());
}

TEST(ConnectionTest, ReadMessageWithClosedFdMustCloseConnection)
{
    int invalidFd = 99;
    
    Connection conn(0, invalidFd, "adress", 0, "pass");
    
    EXPECT_FALSE(conn.isClosed());
    EXPECT_EQ(0, conn.sendMessage("test"));
    EXPECT_TRUE(conn.isClosed());
}