
#ifndef USER_COMMAND_HPP
# define USER_COMMAND_HPP

#include "Command.hpp"
#include "../entity/UserService.hpp"

#define	USER_CMD 				"USER"

#define USERNAME_ARG_INDEX      0
#define REALNAME_ARG_INDEX      3
#define REALNAME_MAX_LENGTH     60
#define USERNAME_MAX_LENGTH     30

class UserCommand: public Command
{
    
private:

    UserService & _service;
	
	bool _checkUsername(const std::string & username) const;
	bool _checkRealname(std::string & realname) const;

public:

    UserCommand(UserService & userService);
    ~UserCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif