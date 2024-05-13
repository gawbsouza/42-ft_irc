#include "Server.hpp"
#include "../entity/Connection.hpp"
#include "../handler/SessionEventHandler.hpp"
#include "../helper/Log.hpp"
#include <sys/socket.h>
#include <stdexcept>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <iostream>
#include <string.h>

#define CONN_QUEUE_SIZE 5

EventHandler* Server::_eventHandler = NULL;
Session Server::_session;

Server::~Server() {}

Server::Server() {}

void Server::onEvent(EventHandler *handler) {
    _eventHandler = handler;
}

void Server::run(const int port, const std::string password)
{
	try
	{   
		if (_eventHandler == NULL)
			throw std::runtime_error("sesssion event handler missing");
    	_setup(port, password);
		_connectionMonitor();
	}
	catch(const std::exception& e)
	{
		_destroySession();
		throw;
	}
}

void Server::_setup(const int port, const std::string& password)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr; memset(&serverAddr, 0, sizeof(serverAddr)); 
	
	serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (serverSocket == -1) 
		throw std::runtime_error("creating socket.");
    
	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), 
		sizeof(serverAddr)) == -1) 
    	throw std::runtime_error("binding port to the server socket.");
    
	if (listen(serverSocket, CONN_QUEUE_SIZE) == -1) 
		throw std::runtime_error("socket listening.");
	
	std::string address = inet_ntoa(serverAddr.sin_addr);

	_session.push_back(Connection(0, serverSocket, port, address, password));
	
	std::stringstream ss; ss << "IRC Server running at " + address + ':' 
							 << port << '.';

	Log::debug(ss.str());
}

void Server::_connectionMonitor()
{  
    Session::iterator sessionIter = _session.begin();
	size_t sessionIndex = 0;

	_signalListener();

	while (_session.begin()->isClosed() == false)
    {
		if (sessionIter->getId() == 0)
			_serverEvents();
		else if (sessionIter->getId() > 0)
			_clientEvents(sessionIter);

		_sessionIterUpdate(sessionIter, sessionIndex);
	}
	_destroySession();
}

void Server::_signalListener(void)
{
	signal(SIGINT, _handleSignal);
 	signal(SIGTSTP, _handleSignal);
  	signal(SIGQUIT, _handleSignal);
}

void Server::_serverEvents(void)
{
	pollfd pollFD; 
	pollFD.fd = _session.begin()->getFd();
	pollFD.events = POLLIN;
	pollFD.revents = 0;

	while (int activity = poll(&pollFD, 1, 0) && !_session.begin()->isClosed())
	{
		Session::iterator sessionIter = _session.begin();
		if (activity == -1)
			throw std::runtime_error("starting poll monitoring.");
		else if (pollFD.revents & POLLIN )
			_eventHandler->handle(Event(EVENT_CONNECT, "", _session, 	
									    sessionIter));
	}
}

void Server::_clientEvents(Session::iterator& sessionIter)
{
	pollfd pollFD; 
	pollFD.fd = sessionIter->getFd();
	pollFD.events = POLLIN | POLLHUP;
	pollFD.revents = 0;
	
	if (int activity = poll(&pollFD, 1, 0) && !_session.begin()->isClosed())
	{
		std::string message = sessionIter->readMessage();
		if (activity == -1)
            Log::error("client socket corrupted.");
		else if (sessionIter->isClosed())
			_eventHandler->handle(Event(EVENT_DISCONNECT, message, _session, 
								  sessionIter));
		else if ((message.empty() && pollFD.revents & POLLIN ) || 
				 pollFD.revents & POLLHUP)
			_eventHandler->handle(Event(EVENT_CLOSE, message, _session, 
								  sessionIter));
		else
			_eventHandler->handle(Event(EVENT_MESSAGE, message, _session, 
									  sessionIter));
	}
}

void Server::_sessionIterUpdate(Session::iterator& sIter, size_t& sIdx)
{
	sIter = _session.begin() + (sIdx - (sIdx > 0));
		
	if (sIter >= _session.end() -1)
	{
		sIter = _session.begin(); 
		sIdx = 0;	
	}
	else
	{
		sIter++; 
		sIdx++;
	}
}

void Server::_destroySession(void)
{
	for (Session::iterator iter = _session.begin(); 
		 iter != _session.end(); iter++)
	{
		close((*iter).getFd());
		if ( iter->getId() > 0)
		{
			std::stringstream ss; ss << "Client " << iter->getId()
				  			 	     << " disconnected.";
			Log::debug(ss.str());
		}
	} 
	
	_session.clear();
}

void Server::_handleSignal(int signalNumber) 
{
	std::cout << "\e[2D";
	if (signalNumber == SIGINT) Log::debug("SIGINT signal caught.");
	else if (signalNumber == SIGTSTP) Log::debug("SIGTSTP signal caught.");
	else if (signalNumber == SIGQUIT) Log::debug("SIGQUIT signal caught.");
	if (signalNumber) _session.begin()->setStatus(true);
}
