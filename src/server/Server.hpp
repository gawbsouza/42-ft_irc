#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <vector>
#include <signal.h>

#include "../entity/Connection.hpp"
#include "../handler/EventHandler.hpp"


class Server
{
	private:
		
		static EventHandler *_eventHandler;
		static Session _session;
		
		~Server(void);
		Server(void);
		static void _setup(const int port, const std::string& password);
		static void _connectionMonitor(void);
		static void _serverEvents(void);
		static void _clientEvents(Session::iterator& sessionIter);
		static void _sessionIterUpdate(Session::iterator& sIter, size_t& sIdx);
		static void _destroySession(void);
		static void _handleSignal(int signalNumber);
		static void _signalListener(void);

	public:

		static void run(const int port, const std::string password);
		static void onEvent(EventHandler *handler);
};

#endif