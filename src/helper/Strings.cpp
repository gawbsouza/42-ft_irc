/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Strings.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:54:38 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/01 19:46:41 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Strings.hpp"
#include <cstdio>

#define NEW_STR_BUFFER_SIZE 1024

Strings::Strings(void) {}
Strings::~Strings(void) {}
    
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
    if (!endSub.empty()) {
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

bool Strings::isOnPattern(const std::string & str, const std::string & pattern, size_t limitIndex)
{
	size_t strLength = str.length();
	size_t patternLength = pattern.length();
	
	if (!strLength || !patternLength){
		return false;
	}

	if (limitIndex >= strLength){
		limitIndex = strLength - 1;
	}
	
	for (size_t index = 0; index <= limitIndex; index++){
		if(pattern.find(str.at(index)) == std::string::npos){
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

	str.erase(size, std::string::npos);
}

std::string Strings::join(const std::vector<std::string>::iterator &begin, 
						  const std::vector<std::string>::iterator &end, 
						  const std::string &sep)
{
	std::string joined;
	
	for (std::vector<std::string>::iterator iter = begin; iter != end; iter++)
	{
		joined.append(*iter);
		
		if(iter != end - 1){
			joined.append(sep);
		}
	}

	return joined;
}

std::string Strings::removePrefix(const std::string &str, const std::string &prefix)
{
	std::string cleaned = str;
	size_t prefixOccurence = str.find_first_of(prefix);
	
	if (prefixOccurence != std::string::npos){
		cleaned = str.substr(prefixOccurence + prefix.size());
	}	
	
	return cleaned;
}

std::string Strings::f(const std::string & str, const std::string & args)
{
	char message[NEW_STR_BUFFER_SIZE];
	sprintf(message, str.c_str(), args.c_str());
	return message;
}

std::string Strings::f(const std::string & str, const std::string & arg1, const std::string & arg2)
{
	char message[NEW_STR_BUFFER_SIZE];
	sprintf(message, str.c_str(), arg1.c_str(), arg2.c_str());
	return message;
}

std::string Strings::f(const std::string & str, const std::string & arg1, const std::string & arg2, const std::string & arg3)
{
	char message[NEW_STR_BUFFER_SIZE];
	sprintf(message, str.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str());
	return message;
}

std::string Strings::f(const std::string & str, const std::string & arg1, const std::string & arg2, const std::string & arg3, const std::string & arg4)
{
	char message[NEW_STR_BUFFER_SIZE];
	sprintf(message, str.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str(), arg4.c_str());
	return message;
}