
#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

#include "Command.hpp"
#include "../entity/UserService.hpp"
#include "../entity/ChannelService.hpp"

#define CHANNEL_ARG_INDEX		0
#define CHANNEL_KEY_ARG_INDEX	1	
#define CHANNEL_MIN_LENGTH		5
#define CHANNEL_MAX_LENGTH		20
#define	JOIN_CMD				"JOIN"

class JoinCommand: public Command
{
    
private:

    ChannelService 	& _service;
	
	bool _checkChannelName(const std::string & username) const;
	std::string _joinedchannelUserList(const std::vector<ChannelUser> & userList) const;

public:

    JoinCommand(ChannelService & service);
    ~JoinCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif