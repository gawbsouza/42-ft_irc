/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:46:43 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 01:02:50 by gasouza          ###   ########.fr       */
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
void serverHeader();

Server::Server(int port, const std::string password)
{
	this->_port = port;
	this->_password = password;
	this->_serverRunning = false;
	this->_idsCount = 0;
	this->_serverSocket = -1;
	this->_handlers[EVENT_CONNECT] = NULL;
	this->_handlers[EVENT_DISCONNECT] = NULL;
	this->_handlers[EVENT_MESSAGE] = NULL;
}

Server::~Server(void) {}

void Server::addHandler(EventType type, EventHandler & handler)
{
	if (type == EVENT_CONNECT || type == EVENT_DISCONNECT || type == EVENT_MESSAGE ) {
		this->_handlers[type] = &handler;
	}
}

void Server::run(void)
{
	this->_serverRunning = true;

	serverHeader();
	
	std::stringstream ss; ss << "Server started on port: " << this->_port;
	Log::notice(ss.str());
	Log::notice("Server password: " + this->_password);
	
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

void Server::_setup(void)
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

	std::stringstream ss; ss << "Server socket bind on port " << this->_port;
    Log::debug(ss.str());

	if (listen(this->_serverSocket, CONN_QUEUE_SIZE) == -1)  {
		throw std::runtime_error("Server socket start listening");
	}

	std::string connAddrStr = inet_ntoa(serverAddr.sin_addr);
	int connPort			= ntohs(serverAddr.sin_port);

	Connection *newConn = new Connection(this->_nextConnId(), this->_serverSocket, connAddrStr, connPort, this->_password);
	this->_connections.push_back(newConn);

	Log::info("Server listening for connections!");
}

void Server::_connectionMonitor(void)
{  
	Log::debug("Connections monitor started");
	while (this->_serverRunning)
    {
		this->_connectionEvents();
	}
	Log::debug("Connections monitor stopped");
}

void Server::_connectionEvents(void)
{
	std::list<Connection *> connToRemove;
	std::list<Connection *>::iterator it;
	
	for (it = this->_connections.begin(); it != this->_connections.end(); it++) 
	{
		Connection * conn = *it;
		std::stringstream ss; 
		
		if (conn->isClosed()) {
			this->_connectionClosedHandle(conn, connToRemove);
			continue;
		}

		pollfd pollFD	= {};
		
		pollFD.fd 		= conn->getFd();
		pollFD.events	= POLLIN | POLLHUP;
		pollFD.revents	= 0;

		int activity	= poll(&pollFD, 1, 0);
		
		if (activity == -1 || pollFD.revents & POLLHUP)
		{
			this->_connectionClosedHandle(conn, connToRemove);
			continue;
		}

		if (!(pollFD.revents & POLLIN)) {
			continue;
		}

		if (pollFD.fd == this->_serverSocket)
		{
			this->_serverConnectionEvent();
			continue;
		}
		
		std::string message = conn->readMessage();

		if (conn->isClosed())
		{
			this->_connectionClosedHandle(conn, connToRemove);
			continue;
		}

		EventHandler *handler = this->_handlers[EVENT_MESSAGE];
		if (handler != NULL) {
			handler->handle(Event(EVENT_MESSAGE, *conn, message));
		}
	}

	for(it = connToRemove.begin(); it != connToRemove.end(); it++) {
		Connection * conn = *it;
		this->_connections.remove(conn);
		delete conn;
	}
}

void Server::_serverConnectionEvent(void)
{
	sockaddr_in connAddr	= {};
	socklen_t connAddSize	= sizeof(connAddr);
	sockaddr *sockAddr		= reinterpret_cast<sockaddr*>(&connAddr);

	const int connSocket = accept(this->_serverSocket, sockAddr, &connAddSize);
	
	setFdNonBlocking(connSocket);

	std::string connAddrStr = inet_ntoa(connAddr.sin_addr);
	int connPort			= ntohs(connAddr.sin_port);

	Connection *newConn = new Connection(this->_nextConnId(), connSocket, connAddrStr, connPort, this->_password);

	this->_connections.push_back(newConn);
	
	Log::notice("New connection stablished: " + newConn->str());

	std::stringstream ss; ss << "Connections count: " << this->_connections.size();
	Log::debug(ss.str());

	EventHandler *handler = this->_handlers[EVENT_CONNECT];
	if (handler == NULL) {
		return;
	}
	
	handler->handle(Event(EVENT_CONNECT, *newConn, ""));
}

void Server::stop(void)
{
	this->_serverRunning = false;
	Log::info("Server stopping...");
}

void Server::_destroyConnections(void)
{
	Log::info("Server connections closing...");
	
	std::list<Connection *> connToRemove; 
	std::list<Connection *>::iterator it;

	for (it = this->_connections.begin(); it != this->_connections.end(); it++)
	{
		Connection * conn = *it;
		if (conn == NULL) {
			continue;
		}
		Log::notice("Connection closed: " + conn->str());
		this->_connectionClosedHandle(conn, connToRemove);
	}
	
	for (it = connToRemove.begin(); it != connToRemove.end(); it++)
	{
		Connection * conn = *it;
		this->_connections.remove(conn);
		delete conn;
	}
	
	Log::info("Server connections closed");
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

void Server::_connectionClosedHandle(Connection * conn, std::list<Connection *> & connToRemove)
{
	conn->close();
	connToRemove.push_back(conn);
	
	if (conn->getFd() == this->_serverSocket) {
		return;
	}
	
	Log::notice("Connection closed: " + conn->str());
	
	std::stringstream ss;
	ss << "Connections count: " << (this->_connections.size() - connToRemove.size());
	Log::debug(ss.str());
	
	EventHandler *handler = this->_handlers[EVENT_DISCONNECT];
	if (handler != NULL) {
		handler->handle(Event(EVENT_DISCONNECT, *conn, ""));
	}
}

void serverHeader()
{
	Log::info("");
	Log::info("-------------------------------------------");
	Log::info("");
	Log::info(" .d888 888             d8b                 ");
	Log::info("d88P\"  888             Y8P                 ");
	Log::info("888    888                                 ");
	Log::info("888888 888888          888 888d888 .d8888b ");
	Log::info("888    888             888 888P\"  d88P\"    ");
	Log::info("888    888             888 888    888      ");
	Log::info("888    Y88b.           888 888    Y88b.    ");
	Log::info("888     \"Y888 88888888 888 888     \"Y8888P");
	Log::info("");
	Log::info("-----    Gabriel Souza & Bruno Luiz   -----");
	Log::info("");
}