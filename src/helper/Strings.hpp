/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Strings.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:51:32 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/19 17:44:18 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGS_HPP
# define STRING_HPP

#include <string>
#include <vector>

class Strings
{
    
private:

    Strings();
    ~Strings();
    
public:

    static void replace(std::string & src, const std::string & need, const std::string & rplc);
    static std::vector<std::string> split(const std::string & str, const std::string & delimit);
    
};

#endif