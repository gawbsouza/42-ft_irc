/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Strings.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bluiz-al <bluiz-al@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:54:38 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/24 17:33:07 by bluiz-al         ###   ########.fr       */
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

bool Strings::isOnPattern(const std::string & str, const std::string & pattern, const size_t limitIndex)
{
	size_t strLength = str.length();
	size_t patternLength = pattern.length();
	
	if (!strLength || !patternLength){
		return false;
	}

	if (limitIndex >= strLength){
		const_cast<size_t&>(limitIndex) = strLength - 1;
	}
	
	for (size_t index = 0; index <= limitIndex; index++){
		if(pattern.find(str.at(index)) == STR_NPOS){
			return false;	
		}
	}
	
	return true;
}

void Strings::truncateBySize(std::string &str, const size_t size)
{
	if (size >= str.length()){
		return;
	}

	str.erase(size, STR_NPOS);
}
