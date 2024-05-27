
#include "UserCommand.hpp"
#include "ResponseCode.hpp"
#include "../helper/Strings.hpp"
#include "../helper/Log.hpp"

#include <sstream>

#define REALNAME_ARG_INDEX 		3
#define REALNAME_MAX_LENGTH 	60
#define USERNAME_MAX_LENGTH 	30
#define USERNAME_PATTERN 		"-._"
#define	USER_NAME 				"USER"
#define MSG_USERNAME_ERROR		":Username must be composed of alphanumeric, underscore, dash and/or dot characters"
#define MSG_REALNAME_ERROR		":Realname must be composed of printable characters"


UserCommand::UserCommand(UserService & service): _service(service) {}
UserCommand::~UserCommand(void) {}

void UserCommand::execute(User & user, std::vector<std::string> args) const
{
	Log::info(USER_NAME " command called");
	Connection &conn = user.getConnection();

    if(!user.isAuthenticated()) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_NOTREGISTERED SPACE MSG_NOTREGISTERED CRLF);
        return;
    }

    if(user.isRegistered()) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_ALREADYREGISTERED SPACE MSG_ALREADYREGISTERED CRLF);
        return;
    }

    if (args.size() < 4) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_NEEDMOREPARAMS SPACE MSG_NEEDMOREPARAMS CRLF);
        return;
    }

	std::string username = args[0];
    
	if (!_checkUsername(username)) {
        conn.sendMessage(SERVER_PREFIX SPACE ERR_UNKNOWNERROR SPACE USER_NAME SPACE MSG_USERNAME_ERROR CRLF);
		return;
	}

	std::string realname = Strings::removePrefix(args[REALNAME_ARG_INDEX] , ":");

	if (args.size() > 4) {
		std::string remainingNameParts = Strings::join(args.begin() + REALNAME_ARG_INDEX + 1, args.end(), " ");
		realname.append(" ");
		realname.append(remainingNameParts);
	}
	
	if (!_checkRealname(realname)){
		conn.sendMessage(SERVER_PREFIX SPACE ERR_UNKNOWNERROR SPACE USER_NAME SPACE MSG_REALNAME_ERROR CRLF);
		return;
	}
	
	Strings::truncateBySize(username, USERNAME_MAX_LENGTH);
	Strings::truncateBySize(realname, REALNAME_MAX_LENGTH);

	user.setUserName(username);
	user.setRealName(realname);
	Log::debug("User set username/realname from " + conn.str());
}

bool UserCommand::_checkUsername(const std::string & username) const
{
	if (!Strings::isOnPattern(username, ALPHANUM_PATTERN, 0)){
		return false;
	}
	
	if (!Strings::isOnPattern(username, ALPHANUM_PATTERN USERNAME_PATTERN)){
		return false;
	}
	
	return true;
}

bool UserCommand::_checkRealname(std::string & realname) const
{
	std::string::iterator iter = realname.begin();
	std::string::iterator end = realname.end();
	
	for (; iter != end; iter++){
		if(!std::isprint(*iter)){
			return false;
		}
	}

	return true;
}