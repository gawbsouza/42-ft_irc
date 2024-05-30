/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCProtocol.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 21:00:07 by bluiz-al          #+#    #+#             */
/*   Updated: 2024/05/30 14:10:51 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_CODE_HPP
# define RESPONSE_CODE_HPP

#define XCHARS_PATTERN  "-_."

// Custom
#define ERR_GENERICERROR            ":ft_irc 400 %s :%s\r\n"
#define ERR_ALREADYAUTHENTICATED    ":ft_irc 400 %s :You already authenticated\r\n"
#define ERR_NOTAUTHENTICATED        ":ft_irc 400 %s :You are not authenticated\r\n"
#define ERR_USERNAMEINVALIDCHARS    ":ft_irc 400 %s :Username contains invalid characters\r\n"
#define ERR_REALNAMEINVALIDCHARS    ":ft_irc 400 %s :Realname contains no printable characters\r\n"
#define ERR_ALREADYREGISTERED       ":ft_irc 400 %s :You already registered\r\n"

// Protocol Default
#define ERR_NOSUCHNICK              ":ft_irc 401 %s %s :No such nick\r\n"
#define ERR_NOSUCHCHANNEL           ":ft_irc 403 %s %s :No such channel\r\n"
#define ERR_ERRONEUSNICKNAME        ":ft_irc 432 %s :Invalid nickname format\r\n"
#define ERR_NICKNAMEINUSE           ":ft_irc 433 %s :This nickname is already in use\r\n"
#define ERR_NOTONCHANNEL            ":ft_irc 442 %s %s :You're not on that channel\r\n"
#define ERR_NOTREGISTERED 			":ft_irc 451 %s :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS          ":ft_irc 461 %s :Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH          ":ft_irc 464 %s :Password incorrect\r\n"
#define ERR_CHANNELISFULL 			":ft_irc 471 %s :Cannot join channel, channel is full (+l)\r\n"
#define ERR_INVITEONLYCHAN 			":ft_irc 473 %s :Cannot join channel, you must be invited first (+i)\r\n"
#define ERR_BADCHANNELKEY 			":ft_irc 475 %s :Cannot join channel, wrong channel key (+k)\r\n"
#define ERR_CHANOPRIVSNEEDED		":ft_irc 482 %s %s :You're not channel operator\r\n"

#define RPL_WELCOMEMESSAGE			":ft_irc 001 %s :Welcome to the ft_irc Network, %s!\r\n"
#define RPL_CHANNELMODEIS           ":ft_irc 324 %s %s %s\r\n"
#define RPL_NOTOPIC					":ft_irc 331 %s %s :No topic is set\r\n"
#define RPL_TOPIC					":ft_irc 332 %s %s :%s\r\n"
#define RPL_NAMREPLY				":ft_irc 353 %s = %s :%s\r\n"
#define RPL_ENDOFNAMES				":ft_irc 366 %s :End of /NAMES List\r\n"

#endif