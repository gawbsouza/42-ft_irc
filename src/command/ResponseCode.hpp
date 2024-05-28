/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseCode.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 21:00:07 by bluiz-al          #+#    #+#             */
/*   Updated: 2024/05/28 01:56:08 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_CODE_HPP
# define RESPONSE_CODE_HPP

#define SERVER_PREFIX	":ft_irc"
#define CRLF			"\r\n"
#define SPACE			" "
#define COLON			":"
#define ASTERISK		"*"

//messages
#define MSG_ALREADYAUTHENTICATED	":You already authenticated"
#define MSG_NOTAUTHENTICATED       	":You are not authenticated"
#define MSG_NEEDMOREPARAMS      	":Not enough parameters"
#define MSG_PASSWDMISMATCH      	":Password incorrect"
#define MSG_NOTREGISTERED       	":You have not registered"
#define MSG_ALREADYREGISTERED   	":You already registered"
#define MSG_WELCOME_MESSAGE   		":Welcome to the ft_irc Network,"

//error codes
#define WELCOME_MESSAGE			"001"
#define ERR_UNKNOWNERROR        "400"
#define	ERR_ERRONEUSNICKNAME	"432"
#define ERR_NICKNAMEINUSE 		"433"
#define ERR_NOTREGISTERED       "451"
#define ERR_NEEDMOREPARAMS      "461" 
#define ERR_ALREADYREGISTERED   "462" 
#define ERR_PASSWDMISMATCH      "464"

#endif