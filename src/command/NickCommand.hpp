
#ifndef NICK_COMMAND_HPP
# define NICK_COMMAND_HPP

#include "Command.hpp"
#include "../entity/UserService.hpp"

#define	NICK_CMD "NICK"

#define NICKNAME_ARG_INDEX      0
#define NICKNAME_MAX_LENGTH     20

class NickCommand: public Command
{
    
private:

    UserService & _service;
	
	bool _checkNickname(const std::string & username) const;

public:

    NickCommand(UserService & userService);
    ~NickCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif