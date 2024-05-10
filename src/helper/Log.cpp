/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 08:21:51 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/10 13:32:21 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include "Colors.hpp"

#include <sstream>
#include <iostream>

Log::Level Log::_level = ALL;

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
    std::cout << currentDateTime()
              << COLOR_GRAY << " [DBG] " << COLOR_RESET
              << msg << std::endl;
}

void Log::info(const std::string & msg)
{
    if (_level > INFO) {
        return;
    }
    std::cout << currentDateTime()
              << COLOR_BLUE << " [INF] " << COLOR_RESET
              << msg << std::endl;
}

void Log::warning(const std::string & msg)
{
    if (_level > WARNING) {
        return;
    }
    std::cout << currentDateTime()
              << COLOR_YELLOW << " [WAR] " << COLOR_RESET
              << msg << std::endl;
}

void Log::error(const std::string & msg)
{
    if (_level > ERROR) {
        return;
    }
    std::cout << currentDateTime()
              << COLOR_RED << " [ERR] " << COLOR_RESET
              << msg << std::endl;
}