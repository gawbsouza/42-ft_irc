/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:46:43 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/16 23:51:32 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../entity/Connection.hpp"
#include "../helper/Log.hpp"

#include <sys/socket.h>
#include <stdexcept>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <poll.h>
#include <list>

#define CONN_QUEUE_SIZE 5

Server::Server(const std::string & name, int port, const std::string password)
{
	this->_name = name;
	this->_port = port;
	this->_password = password;
	this->_serverRunning = false;
	this->_idsCount = 0;
	this->_serverSocket = -1;
	this->_handlers[EVENT_CONNECT] = NULL;
	this->_handlers[EVENT_DISCONNECT] = NULL;
	this->_handlers[EVENT_MESSAGE] = NULL;
}

Server::~Server()
{
	_destroyConnections();
}

void Server::addHandler(EventType type, EventHandler *handler)
{
	if (type == EVENT_CONNECT || type == EVENT_DISCONNECT || type == EVENT_MESSAGE ) {
		this->_handlers[type] = handler;
	}
}

void Server::run()
{
	this->_serverRunning = true;
	std::stringstream ss; ss << "Server started on port: " << this->_port;
	Log::info(ss.str());
	try
	{   
    	_setup();
		_connectionMonitor();
	}
	catch(const std::exception& e)
	{
		this->_serverRunning = false;
		_destroyConnections();
		throw;
	}
}

void Server::_setup()
{
	Log::info("Server password: " + this->_password);
	
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1) {
		throw std::runtime_error("creating socket");
	}

	this->_serverSocket = socketFd;

	Log::debug("Server socket created!");
	
	sockaddr_in serverAddr; memset(&serverAddr, 0, sizeof(serverAddr)); 
	
	serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->_port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(_serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
    	throw std::runtime_error("binding port to the server socket");
	}

	std::stringstream ss; ss << "Server socket bind at port " << this->_port;
    Log::debug(ss.str());

	if (listen(_serverSocket, CONN_QUEUE_SIZE) == -1)  {
		throw std::runtime_error("socket listening");
	}
	
	std::string address = inet_ntoa(serverAddr.sin_addr);

	Log::info("Server listening for connections!");
}

void Server::_connectionMonitor()
{  
	Log::debug("Connections monitor started");
	while (this->_serverRunning)
    {
		this->_serverEvents();
		this->_clientEvents();
	}
	this->_destroyConnections();
}

void Server::_serverEvents(void)
{
	this->_serverEventsRunning = true;
	
	pollfd pollFD = {.fd = _serverSocket, .events = POLLIN, .revents = 0};
	int activity = poll(&pollFD, 1, 0);

	if (activity == -1) {
		throw std::runtime_error("processing poll from server socket");
	}
	
	if (activity != 1 || !(pollFD.revents & POLLIN)) {
		return;
	}
	
	sockaddr_in connAddr; memset(&connAddr, 0, sizeof(connAddr));
	socklen_t connAddSize = sizeof(connAddr);

	const int connSocket = accept(this->_serverSocket, reinterpret_cast<sockaddr*>(&connAddr), &connAddSize);
	
	std::string connAddrStr = inet_ntoa(connAddr.sin_addr);
	int connPort = ntohs(connAddr.sin_port);

	Connection *newConn = new Connection(this->_nextConnId(), connSocket, connAddrStr, connPort, this->_password);

	this->_connections.push_back(newConn);
	
	std::stringstream ss; ss << "New connection stablished: " << newConn->str();
	Log::info(ss.str());

	ss.str(""); ss << "Connections count: " << this->_connections.size();
	Log::debug(ss.str());

	EventHandler *handler = this->_handlers[EVENT_CONNECT];
	if (handler == NULL) {
		return;
	}
	
	handler->handle(Event(EVENT_CONNECT, newConn, ""));

	ss.str(""); ss << "Event (EVENT_CONNECT) dispatched for connection: " << newConn->str();
	Log::debug(ss.str());

	this->_serverEventsRunning = false;
}

int Server::_nextConnId()
{
	return ++_idsCount;
}

void Server::_clientEvents()
{
	this->_clientEventsRunning = true;
	std::list<Connection*> connToRemove;

	std::list<Connection*>::iterator it;
	for (it = this->_connections.begin(); it != this->_connections.end(); it++) 
	{
		Connection *conn = *it;
		std::stringstream ss; 
		
		pollfd pollFD = {.fd = conn->getFd(), .events = POLLIN | POLLHUP, .revents = 0};

		int activity = poll(&pollFD, 1, 0);
		
		if (activity == -1 || pollFD.revents & POLLHUP)
		{
			conn->close();
			connToRemove.push_back(conn);
			
			if (activity == -1) {
				Log::warning("Connection socket corrupted: " + conn->str());
			} else {
				Log::info("Connection closed: " + conn->str());
				ss.str(""); ss << "Connections count: " << this->_connections.size() - connToRemove.size();
				Log::debug(ss.str());
			}
			
			EventHandler *handler = this->_handlers[EVENT_DISCONNECT];
			if (handler != NULL) {
				handler->handle(Event(EVENT_DISCONNECT, conn, ""));
				Log::debug("Event (EVENT_DISCONNECT) dispatched for connection: " + conn->str());
			}

			continue;
		}

		if (!(pollFD.revents & POLLIN)) {
			continue;
		}
		
		std::string message = conn->readMessage();

		if (conn->isClosed())
		{
			connToRemove.push_back(conn);
			Log::info( "Connection closed: " + conn->str());
			ss.str(""); ss << "Connections count: " << this->_connections.size() - connToRemove.size();
			Log::debug(ss.str());

			EventHandler *handler = this->_handlers[EVENT_DISCONNECT];
			if (handler != NULL) {
				handler->handle(Event(EVENT_DISCONNECT, conn, ""));
				Log::debug("Event (EVENT_DISCONNECT) dispatched for connection: " + conn->str());
			}

			continue;
		}

		Log::debug("Message from " + conn->str() + ": " + message);

		//remover

		if (message.find("USER") != std::string::npos) {
			conn->sendMessage(":gabsouzas 001 :Welcome to the ft_irc Network, gabsouzas\r\n");
			conn->sendMessage(":gabsouzas 002 :Your host is ft_irc.org, running version 1.0.0\r\n");
			conn->sendMessage(":gabsouzas 003 :This server was created 2024-05-16\r\n");
			Log::debug("Send message to client!");
		}
		if (message.find("JOIN") != std::string::npos) {
			conn->sendMessage(":gabsouzas JOIN #teste\r\t:server 332 #teste teste\r\n:server 353  #teste :gabriel1 gabriel2\r\n");
			Log::debug("Send message to client!");
		}

		EventHandler *handler = this->_handlers[EVENT_MESSAGE];
		if (handler != NULL) {
			handler->handle(Event(EVENT_MESSAGE, conn, message));
			Log::debug("Event (EVENT_MESSAGE) dispatched for connection: " + conn->str());
		}
	}

	for(it = connToRemove.begin(); it != connToRemove.end(); it++) {
		this->_connections.remove(*it);
		delete *it;
	}
	
	this->_clientEventsRunning = false;
}

void Server::stop(void)
{
	if (!this->_serverRunning) {
		return ;
	}
	this->_serverRunning = false;
	
	// while (this->_clientEventsRunning || this->_serverEventsRunning) {}
	
	Log::info("Server stopping...");
	_destroyConnections();

	// Destruir socket do server
	Log::info("Server stopped!");
}

void Server::_destroyConnections(void)
{

	// while (this->_clientEventsRunning || this->_serverEventsRunning) {}
	
	Log::debug("Server connections closing...");
	
	std::list<Connection*> copy = this->_connections; 
	std::list<Connection*>::iterator it;

	for (it = copy.begin(); it != copy.end(); it++) {
		Connection *conn = *it;
		Log::debug("Connection closed: " + conn->str());
		this->_connections.remove(conn);
		conn->close();
		delete conn;
	}
	
	Log::debug("Server connections closed");
}
