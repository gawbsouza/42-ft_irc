/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 20:24:15 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/21 08:37:48 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HANDLER_HPP
# define EVENT_HANDLER_HPP

# include "../entity/Connection.hpp"

typedef enum 
{
    EVENT_CONNECT,
    EVENT_DISCONNECT,
    EVENT_MESSAGE
}   EventType;

struct Event
{
    EventType type;
    Connection & conn;
	std::string message;

	Event(EventType t, Connection & conn, const std::string & message): type(t), conn(conn), message(message){}
};

class EventHandler
{
public:
    virtual void handle(Event event) = 0;
};

#endif