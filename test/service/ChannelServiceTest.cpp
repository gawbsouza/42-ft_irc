/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelServiceTest.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 22:48:43 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 20:56:16 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "../../src/service/ChannelService.hpp"

TEST(ChannelServiceTest, AddChannel)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel(user, "channelTest");
    
    ChannelService service;

    EXPECT_EQ(0, service.channelsCount());
    service.addChannel(channel);
    EXPECT_EQ(1, service.channelsCount());
}

TEST(ChannelServiceTest, RemoveChannel)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel(user, "channelTest");
    
    ChannelService service;

    EXPECT_EQ(0, service.channelsCount());
    service.addChannel(channel);
    EXPECT_EQ(1, service.channelsCount());
    service.removeChannel(channel);
    EXPECT_EQ(0, service.channelsCount());
}

TEST(ChannelServiceTest, RemoveChannelNotAdded)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel(user, "channelTest");
    Channel channel2(user, "channelTest2");
    
    ChannelService service;

    service.addChannel(channel);
    EXPECT_EQ(1, service.channelsCount());
    service.removeChannel(channel2);
    EXPECT_EQ(1, service.channelsCount());
    service.removeChannel(channel);
    EXPECT_EQ(0, service.channelsCount());
}

TEST(ChannelServiceTest, ChannelExists)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel(user, "channelTest");
    
    ChannelService service;
    service.addChannel(channel);
    
    EXPECT_TRUE(service.channelExists("channelTest"));
    EXPECT_FALSE(service.channelExists("OtherChannelName"));
}

TEST(ChannelServiceTest, GetChannelByName)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel(user, "channelTest");
    
    ChannelService service;
    service.addChannel(channel);
    
    EXPECT_EQ(&channel, service.getChannelByName("channelTest"));
    EXPECT_EQ(NULL, service.getChannelByName("InvalidChannelName"));
}

TEST(ChannelServiceTest, GetChannels)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);
    Channel channel1(user, "channelTest");
    Channel channel2(user, "channelTest2");
    
    ChannelService service;
    service.addChannel(channel1);
    service.addChannel(channel2);

    std::vector<Channel *> channels = service.getChannels();

    EXPECT_EQ(2, channels.size());
    EXPECT_EQ(&channel1, channels.at(0));
    EXPECT_EQ(&channel2, channels.at(1));
}

TEST(ChannelServiceTest, GetUserChannels)
{
    Connection conn(0, 0, "", 0, "");
    User user(conn);

    user.setNickName("userTest");

    Channel channel1(user, "channelTest");
    Channel channel2(user, "channelTest2");
    
    ChannelService service;
    service.addChannel(channel1);
    service.addChannel(channel2);

    std::vector<Channel *> channels = service.getChannelsFromNickname("userTest");

    EXPECT_EQ(2, channels.size());
    EXPECT_EQ(&channel1, channels.at(0));
    EXPECT_EQ(&channel2, channels.at(1));
}

