
#include "NickCommand.hpp"
#include "IRCProtocol.hpp"
#include "IRCProtocol.hpp"
#include "../helper/Strings.hpp"
#include "../helper/Log.hpp"

#include <sstream>

NickCommand::NickCommand(UserService & service): _service(service) {}
NickCommand::~NickCommand(void) {}

void NickCommand::execute(User & user, std::vector<std::string> args) const
{
	Connection &conn = user.getConnection();

	Log::info(NICK_CMD " command called from " + conn.str());

    if (args.size() == 0) {
        conn.sendMessage(Strings::f(MSG_NEEDMOREPARAMS, NICK_CMD));
        return;
    }
    
	std::string nickname = args[NICKNAME_ARG_INDEX];

	if (!_checkNickname(nickname)) {
        conn.sendMessage(Strings::f(MSG_ERRONEUSNICKNAME, nickname));
		return;
	}

	if (!user.isAuthenticated()) {
        conn.sendMessage(Strings::f(MSG_NOTAUTHENTICATED, NICK_CMD));
        return;
    }

	if (_service.nickNameExists(nickname)) {
		conn.sendMessage(Strings::f(MSG_NICKNAMEINUSE, NICK_CMD));
        return;
	}

	if (user.getNickName().size() == 0){
		conn.sendMessage(Strings::f("%s %s\r\n", NICK_CMD, nickname));
	}

	if (user.getNickName().size() > 0){
		conn.sendMessage(Strings::f(":%s %s %s\r\n", user.getNickName(), NICK_CMD, nickname));
	}

	user.setNickName(nickname);
	
	Log::info("User set \"" + nickname + "\" nickname from " + conn.str());
}

bool NickCommand::_checkNickname(const std::string & nickname) const
{
	if (nickname.size() > NICKNAME_MAX_LENGTH){
        return false;
	}

	if (!Strings::isOnPattern(nickname, ALPHA_PATTERN XCHARS_PATTERN, 0)){
		return false;
	}
	
	if (!Strings::isOnPattern(nickname.substr(1), ALPHANUM_PATTERN XCHARS_PATTERN)){
		return false;
	}

	return true;
}