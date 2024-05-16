/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 08:21:51 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/16 20:19:40 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include "Colors.hpp"

#include <iostream>

Log::Level Log::_level = ALL;

std::string replaceNewLineChar(const std::string & str) {
    std::string copy = str;
    size_t findAt = copy.find("\n");
    while (findAt != std::string::npos) {
        copy.replace(findAt, 1, "\\n");
        findAt = copy.find("\n");
    }
    return copy;
}

std::string currentDateTime()
{
    time_t now = time(0);
    tm tstruct = {};
    char buf[80];

    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

void Log::setLevel(const Level level) {
    Log::_level = level;
}

Log::Level Log::getLevel() {
    return _level;
}

void Log::debug(const std::string & msg)
{
    if (_level > DEBUG) {
        return;
    }
    std::cout << COLOR_GRAY << "DBG" << "[" + currentDateTime() + "] "
              << replaceNewLineChar(msg) << COLOR_RESET << std::endl;
}

void Log::info(const std::string & msg)
{
    if (_level > INFO) {
        return;
    }
    std::cout << "INF" << "[" + currentDateTime() + "] "
              << replaceNewLineChar(msg) << std::endl;
}

void Log::warning(const std::string & msg)
{
    if (_level > WARNING) {
        return;
    }
    std::cout << COLOR_YELLOW << "WAR" << "[" + currentDateTime() + "] "
              << replaceNewLineChar(msg) << COLOR_RESET << std::endl;
}

void Log::error(const std::string & msg)
{
    if (_level > ERROR) {
        return;
    }
    std::cout << COLOR_RED << "ERR" << "[" + currentDateTime() + "] "
              << replaceNewLineChar(msg)  << COLOR_RESET << std::endl;
}