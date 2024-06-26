/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 22:34:40 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 02:33:58 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handler/EventHandler.hpp"
#include "helper/Log.hpp"
#include "helper/Strings.hpp"
#include "server/Server.hpp"
#include "entity/Connection.hpp"
#include "service/UserService.hpp"
#include "service/ChannelService.hpp"
#include "handler/CommandHandler.hpp"
#include "handler/UserHandler.hpp"
#include "command/JoinCommand.hpp"
#include "command/NickCommand.hpp"
#include "command/PassCommand.hpp"
#include "command/UserCommand.hpp"
#include "command/ModeCommand.hpp"
#include "command/PrivmsgCommand.hpp"
#include "command/TopicCommand.hpp"
#include "command/KickCommand.hpp"
#include "command/InviteCommand.hpp"
#include "command/PartCommand.hpp"
#include "command/QuitCommand.hpp"
#include "command/UsersCommand.hpp"
#include "command/ChannelsCommand.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <csignal>

#define EXIT_SUCCESS 		0
#define EXIT_FAILURE		1
#define PORT_ARG_INDEX		1
#define VALID_PORT_CHARS	"0123456789"

void	setupSignalHandler(int signalNumber);
void	signalsHandler(int signalNumber);

Server	*server;

int	main(int argc, char** argv) 
{
	setupSignalHandler(SIGINT);
	setupSignalHandler(SIGQUIT);

	Log::setLevel(Log::ALL);
	
	if (argc != 3)
	{
		std::cerr << "ircserv usage: <port> <password>" << std::endl;
		return EXIT_FAILURE;
	} 
	
	std::stringstream ss; ss << argv[PORT_ARG_INDEX];
	if (!Strings::isOnPattern(ss.str(), NUM_PATTERN))
	{
		std::cerr << "ircserv error: Port must be a number" << std::endl;
		return EXIT_FAILURE;
	}

	int port; ss >> port;
	std::string password = argv[2];

	// Services
	UserService userService;
	ChannelService channelService;
	
	// Handlers
	UserHandler userHandler(userService, channelService);
	CommandHandler commandHandler(userService);
	
	// Commands
	NickCommand nickCommand(userService, channelService);
	JoinCommand joinCommand(channelService);
	PassCommand passCommand;
	UserCommand userCommand;
	ModeCommand modeCommand(channelService);
	PrivmsgCommand privmsgCommand(userService, channelService);
	TopicCommand topicCommand(channelService);
	KickCommand kickCommand(channelService);
	InviteCommand inviteCommand(userService, channelService);
	PartCommand partCommand(channelService);
	QuitCommand quitCommand(channelService);
	UsersCommand usersCommand(userService);
	ChannelsCommand channelsCommand(channelService);
	
	// Protocol Commands
	commandHandler.addCommand(PASS_CMD, passCommand);
	commandHandler.addCommand(USER_CMD, userCommand);
	commandHandler.addCommand(NICK_CMD, nickCommand);
	commandHandler.addCommand(JOIN_CMD, joinCommand);
	commandHandler.addCommand(MODE_CMD, modeCommand);
	commandHandler.addCommand(PRIVMSG_CMD, privmsgCommand);
	commandHandler.addCommand(TOPIC_CMD, topicCommand);
	commandHandler.addCommand(KICK_CMD, kickCommand);
	commandHandler.addCommand(PART_CMD, partCommand);
	commandHandler.addCommand(INVITE_CMD, inviteCommand);
	commandHandler.addCommand(QUIT_CMD, quitCommand);

	// System commands
	commandHandler.addCommand("@USERS", usersCommand);
	commandHandler.addCommand("@CHANNELS", channelsCommand);
	
	try
	{
		server = new Server(port, password);
		
		server->addHandler(EVENT_CONNECT, userHandler);
		server->addHandler(EVENT_DISCONNECT, userHandler);
		server->addHandler(EVENT_MESSAGE, commandHandler);
		
		server->run();
	}
	catch(const std::exception& e)
	{
		delete server;
		Log::error(std::string(e.what()));
    	return EXIT_FAILURE;
	}

	delete server;
    return EXIT_SUCCESS;
}

void	setupSignalHandler(int signalNumber)
{
	struct sigaction action = {};
	
    action.sa_handler = signalsHandler;
    action.sa_flags = SA_RESTART;
	
   	sigaction(signalNumber, &action, NULL);
}

void	signalsHandler(int signalNumber) 
{
	std::cout << "\r";
	switch (signalNumber)
	{
		case SIGINT:  Log::warning("SIGINT signal caught");  break;
		case SIGQUIT: Log::warning("SIGQUIT signal caught"); break;
		default: return;
	}
	server->stop();
}
