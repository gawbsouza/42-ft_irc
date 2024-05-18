/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:46:43 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/17 20:23:57 by gasouza          ###   ########.fr       */
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
#include <fcntl.h>

#define CONN_QUEUE_SIZE 5

void setFdNonBlocking(int fd);

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

Server::~Server() {}

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
	Log::info("Server password: " + this->_password);
	
	try
	{   
    	this->_setup();
		this->_connectionMonitor();
	}
	catch(const std::exception & e)
	{
		this->_serverRunning = false;
		this->_destroyConnections();
		throw;
	}
	
	this->_destroyConnections();
	
	Log::info("Server stopped!");
}

void Server::_setup()
{
	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (socketFd == -1) {
		throw std::runtime_error("Creating server socket");
	}
	
	Log::debug("Server socket created!");
	
	setFdNonBlocking(socketFd);
	
	this->_serverSocket = socketFd;

	sockaddr_in serverAddr = {};

	serverAddr.sin_family		= AF_INET;
    serverAddr.sin_port 		= htons(this->_port);
    serverAddr.sin_addr.s_addr	= htonl(INADDR_ANY);

	sockaddr *socketAddr		= reinterpret_cast<sockaddr*>(&serverAddr);
	
	if (bind(this->_serverSocket, socketAddr, sizeof(serverAddr)) == -1) {
    	throw std::runtime_error("Binding port to the server socket");
	}

	std::stringstream ss; ss << "Server socket bind at port " << this->_port;
    Log::debug(ss.str());

	if (listen(this->_serverSocket, CONN_QUEUE_SIZE) == -1)  {
		throw std::runtime_error("Server socket start listening");
	}

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
	Log::debug("Connections monitor stopped");
}

void Server::_serverEvents(void)
{
	pollfd pollFD = {};

	pollFD.fd		= this->_serverSocket;
	pollFD.events	= POLLIN;
	pollFD.revents	= 0;
	
	int activity	= poll(&pollFD, 1, 0);

	if (activity == -1) {
		Log::debug("Function poll interrupted for server socket");
		return;
	}
	
	if (activity != 1 || !(pollFD.revents & POLLIN)) {
		return;
	}
	
	sockaddr_in connAddr	= {};
	socklen_t connAddSize	= sizeof(connAddr);
	sockaddr *sockAddr		= reinterpret_cast<sockaddr*>(&connAddr);

	const int connSocket = accept(this->_serverSocket, sockAddr, &connAddSize);
	
	setFdNonBlocking(connSocket);

	std::string connAddrStr = inet_ntoa(connAddr.sin_addr);
	int connPort			= ntohs(connAddr.sin_port);

	Connection *newConn = new Connection(this->_nextConnId(), connSocket, connAddrStr, connPort, this->_password);

	this->_connections.push_back(newConn);
	
	Log::info("New connection stablished: " + newConn->str());

	std::stringstream ss; ss << "Connections count: " << this->_connections.size();
	Log::debug(ss.str());

	EventHandler *handler = this->_handlers[EVENT_CONNECT];
	if (handler == NULL) {
		return;
	}
	
	handler->handle(Event(EVENT_CONNECT, newConn, ""));

	Log::debug("Event (EVENT_CONNECT) dispatched for connection: " + newConn->str());
}

void Server::_clientEvents()
{
	std::list<Connection*> connToRemove;
	std::list<Connection*>::iterator 	it;
	
	for (it = this->_connections.begin(); it != this->_connections.end(); it++) 
	{
		Connection *conn = *it;
		std::stringstream ss; 
		
		pollfd pollFD	= {};
		
		pollFD.fd 		= conn->getFd();
		pollFD.events	= POLLIN | POLLHUP;
		pollFD.revents	= 0;

		int activity	= poll(&pollFD, 1, 0);
		
		if (activity == -1 || pollFD.revents & POLLHUP)
		{
			conn->close();
			connToRemove.push_back(conn);
			
			Log::info("Connection closed: " + conn->str());
			
			ss.str(""); ss << "Connections count: " << (this->_connections.size() - connToRemove.size());
			Log::debug(ss.str());
			
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
			
			ss.str(""); ss << "Connections count: " << (this->_connections.size() - connToRemove.size());
			Log::debug(ss.str());

			EventHandler *handler = this->_handlers[EVENT_DISCONNECT];
			if (handler != NULL) {
				handler->handle(Event(EVENT_DISCONNECT, conn, ""));
				Log::debug("Event (EVENT_DISCONNECT) dispatched for connection: " + conn->str());
			}

			continue;
		}

		Log::debug("Message from " + conn->str() + ": " + message);

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
}

void Server::stop(void)
{
	this->_serverRunning = false;
	Log::info("Server stopping...");
}

void Server::_destroyConnections(void)
{
	Log::info("Server closing connections...");
	
	std::list<Connection*> copy = this->_connections; 
	std::list<Connection*>::iterator it;

	for (it = copy.begin(); it != copy.end(); it++)
	{
		Connection *conn = *it;
		
		Log::info("Connection closed: " + conn->str());
		
		this->_connections.remove(conn);
		conn->close();
		
		delete conn;
	}
	
	Log::info("Server closed connections");
}

int Server::_nextConnId()
{
	return ++_idsCount;
}

void setFdNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
	
    if (flags == -1) {
        Log::error("Getting fd flags");
        return;
    }
	
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		Log::error("Setting fd O_NONBLOCK flag");
    }
}