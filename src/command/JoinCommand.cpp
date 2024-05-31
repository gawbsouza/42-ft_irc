
#include "JoinCommand.hpp"
#include "IRCProtocol.hpp"
#include "../helper/Strings.hpp"
#include "../helper/Log.hpp"

#include <sstream>

JoinCommand::JoinCommand(ChannelService & service) : _service(service) {}
JoinCommand::~JoinCommand(void) {}

void JoinCommand::execute(User & user, std::vector<std::string> args) const
{
	Connection &conn = user.getConnection();
	Log::info(Strings::f(JOIN_CMD " command called from %s", conn.str()));
	std::string nickname = user.getNickName();
	std::string channelKey = "";

    if (args.size() == 0) {
        conn.sendMessage(Strings::f(ERR_NEEDMOREPARAMS, JOIN_CMD));
        return;
    }

    if (args.size() == 2) {
        channelKey = args[CHANNEL_KEY_ARG_INDEX];
    }
    
	std::string channel = args[CHANNEL_ARG_INDEX];

	if (!_checkChannelName(channel)) {
        conn.sendMessage(Strings::f(ERR_GENERICERROR, JOIN_CMD, "Invalid size or characters on channel name"));
		return;
	}

    if (!user.isRegistered()) {
        conn.sendMessage(Strings::f(ERR_NOTREGISTERED, JOIN_CMD));
        return;
    }
	
    std::string joinedMessage = Strings::f(":%s %s %s\r\n", nickname, JOIN_CMD, channel);
	std::string channelName = Strings::removePrefix(channel, "#");

	if (_service.channelExists(channelName) == true) {
		Channel * channelGoted = _service.getChannelByName(channelName);
		
        if (channelGoted->isInviteOnly() && !channelGoted->isInvited(nickname)) {
			conn.sendMessage(Strings::f(ERR_INVITEONLYCHAN, nickname, channel));
            return;
		}

        if (channelGoted->hasPassword() && channelKey != channelGoted->getPassword()) {             
            conn.sendMessage(Strings::f(ERR_BADCHANNELKEY, nickname, channel));
            return;
		}

		size_t usersCount = channelGoted->usersCount();
		size_t usersLimit = channelGoted->getLimit();
		if (usersLimit > 0 && usersCount >= usersLimit) {
			conn.sendMessage(Strings::f(ERR_CHANNELISFULL, nickname, channel));
			return;
		}

		channelGoted->addUser(user);
        std::string newUserMessage = Strings::f(":%s %s :#%s\r\n", nickname, JOIN_CMD, channelGoted->getName());
        channelGoted->broadCast(user, newUserMessage);
		
		conn.sendMessage(joinedMessage);
		conn.sendMessage(Strings::f(RPL_TOPIC, nickname, channel, channelGoted->getTopic()));
		conn.sendMessage(Strings::f(RPL_NAMREPLY, nickname, channel, _joinedchannelUserList(channelGoted->getUsers())));
		conn.sendMessage(Strings::f(RPL_ENDOFNAMES, channel));

        Log::debug(Strings::f("User \"%s\" joined %s from %s", nickname, channel, conn.str()));
		return;
	}
	
	Channel * newChannel = new Channel(user, channelName);
	_service.addChannel(*newChannel);
	newChannel->addUser(user);
	
	conn.sendMessage(joinedMessage);
	conn.sendMessage(Strings::f(RPL_NOTOPIC, nickname, channel));
    conn.sendMessage(Strings::f(RPL_NAMREPLY, nickname, channel, _joinedchannelUserList(newChannel->getUsers()))); 
	conn.sendMessage(Strings::f(RPL_ENDOFNAMES, channel));
	
	Log::debug(Strings::f("User \"%s\" joined %s from %s", nickname, channel, conn.str()));
}

bool JoinCommand::_checkChannelName(const std::string & channel) const
{
	if (channel.size() < CHANNEL_MIN_LENGTH) {
        return false;
	}

	if (channel.size() > CHANNEL_MAX_LENGTH) {
        return false;
	}

	if (!Strings::isOnPattern(channel, "#", 0)) {
		return false;
	}
	
	if (!Strings::isOnPattern(channel.substr(1), ALPHANUM_PATTERN XCHARS_PATTERN)) {
		return false;
	}

	return true;
}

std::string JoinCommand::_joinedchannelUserList(const std::vector<ChannelUser> & userList) const
{
	std::string joinedChanList;
	std::vector<ChannelUser>::const_iterator iter = userList.begin();
	std::vector<ChannelUser>::const_iterator end = userList.end();

	for (;iter != end; iter++)
	{
		if ( iter->type == CHANNEL_OPERATOR ){
			joinedChanList.append("@");
		}

		joinedChanList.append(iter->user.getNickName());

		if (iter != end - 1){
			joinedChanList.append(" ");
		}
	}

	return joinedChanList;
}
