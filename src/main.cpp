/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 22:34:40 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/23 21:49:50 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handler/EventHandler.hpp"
#include "helper/Log.hpp"
#include "server/Server.hpp"
#include "entity/Connection.hpp"
#include "handler/CommandHandler.hpp"
#include "handler/UserHandler.hpp"
#include "command/PassCommand.hpp"
#include "command/ResponseCode.hpp"

#include <iostream>
#include <sstream>
#include <csignal>

#define EXIT_SUCCESS 		0
#define EXIT_FAILURE		1
#define SERVER_NAME		 	"ft_irc"
#define VALID_PORT_CHARS	"0123456789"

void	setupSignalHandler(int signalNumber);
void	signalsHandler(int signalNumber);

Server	*server;

int	main(int argc, char** argv) 
{
	setupSignalHandler(SIGINT);
	setupSignalHandler(SIGQUIT);

	Log::setLevel(Log::ALL);

	// AJUSTAR DELETE DE CONNECTION NO SERVER
	
	if (argc != 3)
	{
		std::cerr << "ircserv usage: <port> <password>" << std::endl;
		return EXIT_FAILURE;
	} 
	
	std::stringstream ss; ss << argv[1];
	if (ss.str().find_first_not_of(VALID_PORT_CHARS) != std::string::npos)
	{
		std::cerr << "ircserv error: Port must be a number" << std::endl;
		return EXIT_FAILURE;
	}

	int port; ss >> port;
	std::string password = argv[2];

	UserService userService;
	CommandHandler commandHandler(userService);
	UserHandler userHandler(userService);
	PassCommand passCommand(userService);

	commandHandler.addCommand("PASS", passCommand);
	
	try
	{
		server = new Server(SERVER_NAME, port, password);
		
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
