/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Strings.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:51:32 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/24 16:13:47 by bluiz-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGS_HPP
# define STRING_HPP

#include <string>
#include <vector>

#define ALPHANUM_PATTERN	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define STR_NPOS			18446744073709551615UL	

class Strings
{
    
private:

    Strings();
    ~Strings();
    
public:

    static void replace(std::string & src, const std::string & need, const std::string & rplc);
    static std::vector<std::string> split(const std::string & str, const std::string & delimit);
    static std::string toUpper(const std::string & str);
    static bool isOnPattern(const std::string & str, const std::string & pattern, const size_t limitIndex = STR_NPOS);
    static void truncateBySize(std::string & str, const size_t size);

};

#endif