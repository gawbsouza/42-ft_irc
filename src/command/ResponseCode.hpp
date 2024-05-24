/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseCode.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 21:00:07 by bluiz-al          #+#    #+#             */
/*   Updated: 2024/05/23 22:00:45 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_CODE_HPP
# define RESPONSE_CODE_HPP

#define SERVER_PREFIX 		":ft_irc"
#define CRLF 				"\r\n"
#define SPACE 				" "

//messages
#define MSG_AUTHENTICATED 	":You already authenticated"
#define MSG_NEEDMOREPARAMS 	":Not enough parameters"
#define MSG_PASSWDMISMATCH 	":Password incorrect"

//error codes
#define ERR_UNKNOWNERROR	"400"
#define ERR_NEEDMOREPARAMS	"461" 
#define ERR_PASSWDMISMATCH	"464" 

#endif