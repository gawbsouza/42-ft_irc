
#include "UserCommand.hpp"
#include "ResponseCode.hpp"
#include "../helper/Strings.hpp"

#include <sstream>

#define REALNAME_ARG_INDEX 		3
#define REALNAME_MAX_LENGTH 	60
#define USERNAME_MAX_LENGTH 	30
#define USERNAME_PATTERN 		"-._"
#define	USER_NAME 				"USER"
#define MSG_USERNAME_EROR		":Username must be composed of alphanumeric, underscore, dash and/or dot characters"
#define MSG_REALNAME_EROR		":Realname must be composed of printable characters"


UserCommand::UserCommand(UserService & service): _service(service) {}
UserCommand::~UserCommand(void) {}

void UserCommand::execute(User & user, std::vector<std::string> args) const
{
	std::string realname;
	std::string username = args[0];
    Connection &conn = user.getConnection();

    if(user.isRegistered()) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_ALREADYREGISTERED SPACE MSG_ALREADYREGISTERED CRLF);
        return;
    }

    if (args.size() < 4) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_NEEDMOREPARAMS SPACE MSG_NEEDMOREPARAMS CRLF);
        return;
    }
    
	if (!_checkUsername(username)){
        conn.sendMessage(SERVER_PREFIX SPACE ERR_UNKNOWNERROR SPACE USER_NAME SPACE MSG_USERNAME_EROR CRLF);
		return;
	}

	if (!_checkRealname(realname, args)){
		conn.sendMessage(SERVER_PREFIX SPACE ERR_UNKNOWNERROR SPACE USER_NAME SPACE MSG_REALNAME_EROR CRLF);
		return;
	}
	
	Strings::truncateBySize(username, USERNAME_MAX_LENGTH);
	Strings::truncateBySize(realname, REALNAME_MAX_LENGTH);

	user.setUserName(username);
	user.setRealName(realname);

}

bool UserCommand::_checkUsername(const std::string & username) const
{
	if (!Strings::isOnPattern(username, ALPHANUM_PATTERN, 0))
		return false;
	
	if (!Strings::isOnPattern(username, ALPHANUM_PATTERN USERNAME_PATTERN))
		return false;
	
	return true;
}


bool UserCommand::_checkRealname(std::string & realname, const std::vector<std::string> args) const
{
	size_t argsSize = args.size();

	for (size_t vecIndex = REALNAME_ARG_INDEX; vecIndex < argsSize; vecIndex++){
		size_t itemSize = args.at(vecIndex).size();
		
		for (size_t strIndex = 0; strIndex < itemSize; strIndex++){
			if(!std::isprint(args.at(vecIndex).at(strIndex))){
        		return false;
			}
		}
		
		if (vecIndex == REALNAME_ARG_INDEX)
				realname.append(args.at(vecIndex).substr(1));
		else 
			realname.append(args.at(vecIndex));
		
		if (vecIndex < argsSize - 1){
			realname.append(" ");
		}
	}

	return true;
}