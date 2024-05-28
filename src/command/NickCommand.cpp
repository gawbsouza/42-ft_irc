
#include "NickCommand.hpp"
#include "ResponseCode.hpp"
#include "../helper/Strings.hpp"
#include "../helper/Log.hpp"

#include <sstream>

#define NICKNAME_ARG_INDEX		0
#define NICKNAME_MAX_LENGTH		20
#define NICKNAME_PATTERN		"-[]\\`{}^_"
#define	NICKNAME_CMD			"NICK"
#define NICKNAME_SUFFIX			SPACE ASTERISK SPACE + nickname + CRLF
#define NICKNAME_SEP			SPACE NICKNAME_CMD SPACE
#define NICKNAME_SEP2			SPACE ASTERISK NICKNAME_SEP
#define JOINED_ERRONEUSNICKNAME	SERVER_PREFIX SPACE ERR_ERRONEUSNICKNAME NICKNAME_SUFFIX
#define JOINED_NICKNAMEINUSE	SERVER_PREFIX SPACE ERR_NICKNAMEINUSE NICKNAME_SUFFIX
#define JOINED_NEEDMOREPARAMS	SERVER_PREFIX SPACE ERR_NEEDMOREPARAMS NICKNAME_SEP2 MSG_NEEDMOREPARAMS CRLF
#define JOINED_NOTAUTHENTICATED	SERVER_PREFIX SPACE ERR_UNKNOWNERROR NICKNAME_SEP MSG_NOTAUTHENTICATED CRLF

NickCommand::NickCommand(UserService & service): _service(service) {}
NickCommand::~NickCommand(void) {}

void NickCommand::execute(User & user, std::vector<std::string> args) const
{
	Log::info("Nick command called");
	Connection &conn = user.getConnection();

    if (args.size() == 0) {
        conn.sendMessage(JOINED_NEEDMOREPARAMS);
        return;
    }
    
	if (!user.isAuthenticated()) {
        conn.sendMessage(JOINED_NOTAUTHENTICATED);
        return;
    }

	std::string nickname = args[NICKNAME_ARG_INDEX];

	if (!_checkNickname(nickname)) {
        conn.sendMessage(JOINED_ERRONEUSNICKNAME);
		return;
	}

	if (_service.nickNameExists(nickname)) {
		conn.sendMessage(JOINED_NICKNAMEINUSE);
        return;
	}
	std::stringstream ss;

	if (user.getNickName().size() > 0){
		conn.sendMessage(COLON + user.getNickName() + NICKNAME_SEP + nickname + CRLF);
	}

	user.setNickName(nickname);
	
	Log::debug("User set \"" + nickname + "\" nickname from " + conn.str());
}

bool NickCommand::_checkNickname(const std::string & nickname) const
{
	if (nickname.size() > NICKNAME_MAX_LENGTH){
        return false;
	}

	if (!Strings::isOnPattern(nickname, ALPHA_PATTERN NICKNAME_PATTERN, 0)){
		return false;
	}
	
	if (!Strings::isOnPattern(nickname, ALPHANUM_PATTERN NICKNAME_PATTERN)){
		return false;
	}

	return true;
}