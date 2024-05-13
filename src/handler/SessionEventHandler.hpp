#ifndef SESSION_EVENT_HANDLER_HPP
#define SESSION_EVENT_HANDLER_HPP

#include "EventHandler.hpp"
#include <map>


class SessionEventHandler: public EventHandler 
{
	typedef std::map<EventType,void (SessionEventHandler::*)(const Event event)> Handlers;
	Handlers _handlers;

	void _eventConnect(const Event event);
	void _eventClose(const Event event);
	void _eventDisconnect(const Event event);
	void _eventMessage(const Event event);

	public:

    	SessionEventHandler(void);
    	~SessionEventHandler(void);

    	void handle(const Event event);
};

#endif 
