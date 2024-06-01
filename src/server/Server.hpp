/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 20:34:37 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 15:42:03 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../entity/Connection.hpp"
#include "../handler/EventHandler.hpp"

#include <string>
#include <list>
#include <map>

class Server
{
	
private:
		
	std::map<EventType, EventHandler*>	_handlers;
	std::list<Connection *> _connections;
	
	std::string	_name;
	int			_port;
	std::string _password;

	bool	_serverRunning;
	int		_serverSocket;
	int		_idsCount;

	void	_setup(void);
	void	_connectionMonitor(void);
	void	_serverEvents(void);
	void	_clientEvents(void);
	void	_destroyConnections(void);
	int		_nextConnId(void);

	void	_connectionClosedHandle(Connection * conn, std::list<Connection *> & connToRemove);

public:

	Server(const std::string & name, int port, const std::string password);
	~Server(void);

	void run(void);
	void stop(void);
	void addHandler(EventType type, EventHandler & handler);
};

#endif