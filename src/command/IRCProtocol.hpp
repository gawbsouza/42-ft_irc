/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCProtocol.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 21:00:07 by bluiz-al          #+#    #+#             */
/*   Updated: 2024/05/30 01:23:56 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_CODE_HPP
# define RESPONSE_CODE_HPP

#define XCHARS_PATTERN  "-_."

// Custom
#define MSG_GENERICERROR            ":ft_irc 400 %s :%s\r\n"
#define MSG_ALREADYAUTHENTICATED    ":ft_irc 400 %s :You already authenticated\r\n"
#define MSG_NOTAUTHENTICATED        ":ft_irc 400 %s :You are not authenticated\r\n"
#define MSG_USERNAMEINVALIDCHARS    ":ft_irc 400 %s :Username contains invalid characters\r\n"
#define MSG_REALNAMEINVALIDCHARS    ":ft_irc 400 %s :Realname contains no printable characters\r\n"
#define MSG_ALREADYREGISTERED       ":ft_irc 400 %s :You already registered\r\n"

// Protocol Default
#define MSG_WELCOMEMESSAGE          ":ft_irc 001 %s :Welcome to the ft_irc Network, %s!\r\n"
#define MSG_ERRONEUSNICKNAME        ":ft_irc 432 %s :Invalid nickname format\r\n"
#define MSG_NICKNAMEINUSE           ":ft_irc 433 %s :This nickname is already in use\r\n"
#define MSG_NEEDMOREPARAMS          ":ft_irc 461 %s :Not enough parameters\r\n"
#define MSG_PASSWDMISMATCH          ":ft_irc 464 %s :Password incorrect\r\n"

#endif