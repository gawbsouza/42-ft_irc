/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:50:10 by gasouza           #+#    #+#             */
/*   Updated: 2024/06/02 01:11:49 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <string>
# include <sstream>

class Connection
{
    int _id;
    int _fd;
    int _port;
    std::string _address;
    std::string _password;
    
    int _readBufferSize;
    
    std::stringstream _buffer;
    
	bool _closed;

public:

    Connection(int id, int fd, const std::string & address, int port, const std::string & password);
    Connection(const Connection & conn);
    ~Connection(void);

    int getId() const;
    int getFd() const;
    int getPort() const ;
    std::string getAddress() const;
    std::string getPassword() const;
    
    bool isClosed() const;
    void close();
    
    std::string str() const;

    size_t sendMessage(const std::string & msg);
    std::string readMessage();

    Connection & operator=(const Connection & conn);
    bool operator==(const Connection & conn);
    bool operator!=(const Connection & conn);
};

#endif
