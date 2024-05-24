
#ifndef USER_COMMAND_HPP
# define USER_COMMAND_HPP

#include "Command.hpp"
#include "../entity/UserService.hpp"

class UserCommand: public Command
{
    
private:

    UserService & _service;
	
	bool _checkUsername(const std::string & username) const;
	bool _checkRealname(std::string & realnameStream, const std::vector<std::string> args) const;

public:

    UserCommand(UserService & userService);
    ~UserCommand(void);
    
    void execute(User & user, std::vector<std::string> args) const;
    
};

#endif