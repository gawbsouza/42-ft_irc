/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Strings.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:51:32 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/27 14:09:34 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGS_HPP
# define STRING_HPP

#include <string>
#include <vector>

#define ALPHA_PATTERN		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define NUM_PATTERN			"0123456789"
#define ALPHANUM_PATTERN	NUM_PATTERN ALPHA_PATTERN 

class Strings
{
    
private:

    Strings();
    ~Strings();
    
public:

    static void replace(std::string & src, const std::string & need, const std::string & rplc);
    static std::vector<std::string> split(const std::string & str, const std::string & delimit);
    static std::string toUpper(const std::string & str);
    static bool isOnPattern(const std::string & str, const std::string & pattern, 
							const size_t limitIndex = std::string::npos);
    static void truncateBySize(std::string & str, const size_t size);
    static std::string join(const std::vector<std::string>::iterator & begin, 
							const std::vector<std::string>::iterator & end, 
							const std::string & sep);
	static std::string removePrefix(const std::string & str, const std::string & prefix);

};

#endif