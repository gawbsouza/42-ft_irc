#include <iostream>
#include <sstream>
#include "../handler/SessionEventHandler.hpp"
#include "../handler/EventHandler.hpp"
#include "../helper/Log.hpp"
#include "../server/Server.hpp"
#include "../entity/Connection.hpp"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

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
	
	try
	{
		int port; ss >> port;
		std::string password = argv[2]; 
		
		SessionEventHandler sessionHandler;
		Server::onEvent(&sessionHandler);
		Server::run(port, password);
	}
	catch(const std::exception& e)
	{
		Log::error(e.what());
    	return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}
