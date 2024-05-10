/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 08:21:51 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/10 08:38:26 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include <sstream>
#include <iostream>

Log::Level Log::_level = ALL;

#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_RESET   "\x1b[0m"

std::string currentDateTime() {
    time_t now = time(0);
    tm tstruct = {};
    char buf[80];

    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

void Log::setLevel(const Level level){
    Log::_level = level;
}

Log::Level Log::getLevel(){
    return _level;
}


void Log::debug(const std::string & msg) {
    if (_level > DEBUG) {
        return;
    }
    std::cout << currentDateTime()
              << COLOR_MAGENTA " [DEBUG] " << COLOR_RESET
              << msg << std::endl;
}
void Log::info(const std::string & msg) {
    if (_level > INFO) {
        return;
    }
    std::cout << currentDateTime()
              << COLOR_BLUE << " [INFO] " << COLOR_RESET
              << msg << std::endl;
}

void Log::warning(const std::string & msg) {
    if (_level > WARNING) {
        return;
    }
    std::cout << currentDateTime()
              << COLOR_YELLOW << " [WARNING] " << COLOR_RESET
              << msg << std::endl;
}
void Log::error(const std::string & msg) {
    if (_level > ERROR) {
        return;
    }
    std::cout << currentDateTime()
              << COLOR_RED << " [ERROR] " << COLOR_RESET
              << msg << std::endl;
}