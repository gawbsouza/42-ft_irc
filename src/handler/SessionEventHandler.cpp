#include "SessionEventHandler.hpp"
#include "../entity/Connection.hpp"
#include "../helper/Log.hpp"
#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include <string.h>
#include <cstdlib>

SessionEventHandler::SessionEventHandler() 
{
	_handlers[EVENT_CONNECT] = &SessionEventHandler::_eventConnect;
    _handlers[EVENT_CLOSE] = &SessionEventHandler::_eventClose;
    _handlers[EVENT_DISCONNECT] = &SessionEventHandler::_eventDisconnect;
    _handlers[EVENT_MESSAGE] = &SessionEventHandler::_eventMessage;
}

SessionEventHandler::~SessionEventHandler() 
{
	Log::debug("IRC Server session terminated.");
}

void SessionEventHandler::handle(const Event event)
{
	Handlers::iterator iter = _handlers.find(event.type);
	if (iter != _handlers.end())
		return (this->*iter->second)(event);
	else
		throw std::logic_error("Unknown connnection Event.");
}

void SessionEventHandler::_eventConnect(const Event event)
{
	static int IDgenerator = 1;
	sockaddr_in connAddr; memset(&connAddr, 0, sizeof(connAddr));
	socklen_t connAddSize = sizeof(connAddr);

	const int connSocket = accept(event.sessionIter->getFd(), reinterpret_cast<sockaddr*>(&connAddr), &connAddSize);
	
	Connection newClient(IDgenerator, connSocket, ntohs(connAddr.sin_port), 
						 inet_ntoa(connAddr.sin_addr), "");
	event.session.push_back(newClient);
	
	std::stringstream ss; ss << "Client " << newClient.getId()
							 << " connected from " 
			  				 << newClient.getAddress() << ":" 
			  				 << newClient.getPort() << '.';
	Log::debug(ss.str());
	IDgenerator++;
}

void SessionEventHandler::_eventClose(const Event event)
{
	event.sessionIter->setStatus(true);

	std::stringstream ss; ss << "Client " << event.sessionIter->getId()
				  			 << " disconnected.";
	Log::debug(ss.str());
}

void SessionEventHandler::_eventDisconnect(const Event event)
{
    event.session.erase(event.sessionIter);
}

void SessionEventHandler::_eventMessage(const Event event)
{
	if (*event.message.begin() != ':')
	{
		event.sessionIter->sendMessage("Server echo: " + event.message);
		return;
	}

	bool InvalidTarget = true;
	size_t targetLength = event.message.find(' ');
	std::string target = event.message.substr(1, targetLength - 1);
	int targetID = std::atoi(target.c_str());
	std::stringstream ss; ss << "Client " << event.sessionIter->getId();
	
	if (target == "all" )
	{
		ss << ":" << event.message.substr(targetLength);
		InvalidTarget = false;
	}
	else 
		ss << " (Private):" << event.message.substr(targetLength);

	for (Session::iterator sessionIter = event.session.begin() + 1; 
		 sessionIter != event.session.end(); sessionIter++)
	{
		if (target == "all" && 
			sessionIter->getId() != event.sessionIter->getId())
			sessionIter->sendMessage(ss.str());
		else if (targetID == sessionIter->getId() && 
				 targetID != event.sessionIter->getId())
		{
			sessionIter->sendMessage(ss.str());
			return;
		}
	}
	if (InvalidTarget)
		event.sessionIter->sendMessage("Server: invalid client destiny.\n");
}
