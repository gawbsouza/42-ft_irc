
#include "UserCommand.hpp"
#include "ResponseCode.hpp"
#include "../helper/Strings.hpp"
#include "../helper/Log.hpp"

#include <sstream>

#define USERNAME_ARG_INDEX 			0
#define REALNAME_ARG_INDEX 			3
#define REALNAME_MAX_LENGTH 		60
#define USERNAME_MAX_LENGTH 		30
#define USERNAME_PATTERN 			"-._"
#define	USERNAME_CMD 				"USER"
#define USERNAME_SEP				SPACE USERNAME_CMD SPACE
#define USERNAME_SEP2				SPACE ASTERISK USERNAME_SEP
#define USERNAME_SEP3				SPACE + nickname + SPACE
#define MSG_USERNAME_ERROR			":Username contains invalid characters"
#define MSG_REALNAME_ERROR			":Realname contains no printable characters"
#define MSG_NONICKNAMEPROVIDED		":No nickname provided"
#define JOINED_WELCOME_MESSAGE		SERVER_PREFIX SPACE WELCOME_MESSAGE USERNAME_SEP3 MSG_WELCOME_MESSAGE
#define JOINED_USERNAME_ERROR		SERVER_PREFIX SPACE ERR_UNKNOWNERROR USERNAME_SEP MSG_USERNAME_ERROR CRLF
#define JOINED_REALNAME_ERROR		SERVER_PREFIX SPACE ERR_UNKNOWNERROR USERNAME_SEP MSG_REALNAME_ERROR CRLF
#define JOINED_NEEDMOREPARAMS		SERVER_PREFIX SPACE ERR_NEEDMOREPARAMS USERNAME_SEP2 MSG_NEEDMOREPARAMS CRLF
#define JOINED_NONICKNAMEPROVIDED	SERVER_PREFIX SPACE ERR_UNKNOWNERROR USERNAME_SEP MSG_NONICKNAMEPROVIDED CRLF
#define JOINED_ALREADYREGISTERED	SERVER_PREFIX SPACE ERR_ALREADYREGISTERED USERNAME_SEP MSG_ALREADYREGISTERED CRLF


UserCommand::UserCommand(UserService & service): _service(service) {}
UserCommand::~UserCommand(void) {}

void UserCommand::execute(User & user, std::vector<std::string> args) const
{
	Log::info("USER command called");
	Connection &conn = user.getConnection();

    if (args.size() < 4) {
        conn.sendMessage(JOINED_NEEDMOREPARAMS);
        return;
    }

	std::string username = args[USERNAME_ARG_INDEX];
    
	if (!_checkUsername(username)) {
        conn.sendMessage(JOINED_USERNAME_ERROR);
		return;
	}

	std::string realname = Strings::removePrefix(args[REALNAME_ARG_INDEX] , ":");

	if (args.size() > 4) {
		std::string remainingNameParts = Strings::join(args.begin() + REALNAME_ARG_INDEX + 1, args.end(), " ");
		realname.append(" ");
		realname.append(remainingNameParts);
	}
	
	if (!_checkRealname(realname)){
		conn.sendMessage(JOINED_REALNAME_ERROR);
		return;
	}

    if(user.getNickName().size() == 0) {
        conn.sendMessage(JOINED_NONICKNAMEPROVIDED);
        return;
    }

    if(user.isRegistered()) {
        conn.sendMessage(JOINED_ALREADYREGISTERED);
        return;
    }
	
	std::string nickname = user.getNickName();
	Strings::truncateBySize(username, USERNAME_MAX_LENGTH);
	Strings::truncateBySize(realname, REALNAME_MAX_LENGTH);

	user.setUserName(username);
	user.setRealName(realname);
	
	conn.sendMessage(JOINED_WELCOME_MESSAGE SPACE + nickname + CRLF);
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