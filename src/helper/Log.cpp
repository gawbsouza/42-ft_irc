/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 08:21:51 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/18 09:58:33 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include "Colors.hpp"
#include "Strings.hpp"

#include <iostream>

Log::Level Log::_level = ALL;

std::string replaceScapeChars(const std::string & str) {
    std::string copy = str;
    Strings::replace(copy, "\n", "\\n"); // newline | line feed
    Strings::replace(copy, "\r", "\\r"); // carriage return
    Strings::replace(copy, "\f", "\\r"); // form feed
    Strings::replace(copy, "\t", "\\t"); // horizontal tab
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
              << replaceScapeChars(msg) << COLOR_RESET << std::endl;
}

void Log::info(const std::string & msg)
{
    if (_level > INFO) {
        return;
    }
    std::cout << "INF" << "[" + currentDateTime() + "] "
              << replaceScapeChars(msg) << std::endl;
}

void Log::warning(const std::string & msg)
{
    if (_level > WARNING) {
        return;
    }
    std::cout << COLOR_YELLOW << "WAR" << "[" + currentDateTime() + "] "
              << replaceScapeChars(msg) << COLOR_RESET << std::endl;
}

void Log::error(const std::string & msg)
{
    if (_level > ERROR) {
        return;
    }
    std::cout << COLOR_RED << "ERR" << "[" + currentDateTime() + "] "
              << replaceScapeChars(msg)  << COLOR_RESET << std::endl;
}