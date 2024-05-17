/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 22:34:40 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/16 21:44:53 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../handler/EventHandler.hpp"
#include "../helper/Log.hpp"
#include "../server/Server.hpp"
#include "../entity/Connection.hpp"

#include <iostream>
#include <sstream>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void registrySignalsHandler(void);
void signalsHandler(int signalNumber);

Server *server = NULL;

int main(int argc, char** argv) 
{
	std::stringstream ss; ss << argv[1];
	
	if (argc != 3)
	{
		std::cerr << "ircserv usage: <port> <password>" << std::endl;
		return EXIT_FAILURE;
	} 
	
	if (ss.str().find_first_not_of("0123456789") != std::string::npos)
	{
		std::cerr << "ircserv error: Port must be a number" << std::endl;
		return EXIT_FAILURE;
	}

	registrySignalsHandler();

	int port; ss >> port;
	std::string password = argv[2];

	try
	{
		server = new Server("ft_irc", port, password);
		server->run();
	}
	catch(const std::exception& e)
	{
		delete server;
		Log::error(std::string(e.what()));
    	return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}

void registrySignalsHandler(void)
{
	signal(SIGINT, signalsHandler);
 	// signal(SIGTSTP, signalsHandler);
  	signal(SIGQUIT, signalsHandler);
}

void signalsHandler(int signalNumber) 
{
	std::cout << "\e[2D";
	switch (signalNumber)
	{
		case SIGINT:  Log::warning("SIGINT signal caught");  break;
		case SIGQUIT: Log::warning("SIGQUIT signal caught"); break;
		default: return;
	}
	if (server != NULL) {
		server->stop();
	}
}
