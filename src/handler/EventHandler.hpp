#ifndef EVENT_HANDLER_HPP
# define EVENT_HANDLER_HPP

# include "../entity/Connection.hpp"

typedef enum 
{
    EVENT_CONNECT,
    EVENT_CLOSE,
    EVENT_DISCONNECT,
    EVENT_MESSAGE
}   EventType;

struct Event
{
    EventType   		type;
	std::string 		message;
    Session&			session;
    Session::iterator	sessionIter;

	Event(EventType t, const std::string& m, Session& s, Session::iterator& sI)
    : type(t), message(m), session(s), sessionIter(sI) {}
};


class EventHandler
{
public:
    virtual void handle(Event event) = 0;
	virtual ~EventHandler() {};	
};

#endif