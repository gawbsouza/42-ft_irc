/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:18:25 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/10 13:44:20 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include <string>

class Log
{

public:

    typedef enum e_level {
        ALL,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        NONE
    } Level;


    static void setLevel(Level level);
    static Level getLevel();
    
    static void debug(const std::string & msg);
    static void info(const std::string & msg);
    static void warning(const std::string & msg);
    static void error(const std::string & msg);

private:

    static Level _level;
    Log();

};

#endif