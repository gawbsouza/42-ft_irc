/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 22:34:40 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/17 19:50:57 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../handler/EventHandler.hpp"
#include "../helper/Log.hpp"
#include "../server/Server.hpp"
#include "../entity/Connection.hpp"

#include <iostream>
#include <sstream>
#include <csignal>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define VALID_PORT_CHARS "0123456789"
#define SERVER_NAME "ft_irc"

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
	
	std::stringstream ss; ss << argv[1];
	if (ss.str().find_first_not_of(VALID_PORT_CHARS) != std::string::npos)
	{
		std::cerr << "ircserv error: Port must be a number" << std::endl;
		return EXIT_FAILURE;
	}

	int port; ss >> port;
	std::string password = argv[2];

	try
	{
		server = new Server(SERVER_NAME, port, password);
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
