/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Strings.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:54:38 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/22 19:55:00 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Strings.hpp"

Strings::Strings() {}
Strings::~Strings() {}
    
void Strings::replace(std::string & src, const std::string & need, const std::string & rplc)
{
    if (need.size() == 0) {
        return;
    }
    size_t findAt = src.find(need);
    while (findAt != std::string::npos)
    {
        src.replace(findAt, need.size(), rplc);
        findAt = src.find(need);
    }
}

std::vector<std::string> Strings::split(const std::string & str, const std::string & delimit)
{
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = str.find(delimit);
    
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimit, start);
    }

    std::string endSub = str.substr(start);
    if (endSub != "") {
        tokens.push_back(str.substr(start));
    }
    
    return tokens;
}

std::string Strings::toUpper(const std::string & str)
{
    std::string copy = str;

    for (size_t i = 0; i < str.size(); i++) {
        copy[i] = toupper(copy[i]);
    }

    return copy;
}