/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:48:25 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/21 09:00:46 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "../helper/Log.hpp"

#include <cstring>
#include <unistd.h>
#include <sstream>

Connection::Connection(int id, int fd, const std::string & address, int port, const std::string & password)
{
    this->_id = id;
    this->_fd = fd;
    this->_port = port;
    this->_address = address;
    this->_password = password;
    this->_closed = false;
    this->_readBufferSize = 1024;
}

Connection::Connection(const Connection & conn)
{
    *this = conn;
}

Connection::~Connection() {}

Connection & Connection::operator=(const Connection & conn)
{
    if (this != &conn) {
        this->_id = conn._id;
        this->_fd = conn._fd;
        this->_address = conn._address;
        this->_port = conn._port;
        this->_password = conn._password;
        this->_readBufferSize = conn._readBufferSize;
        this->_closed = conn._closed;
    }
    return *this;
}

bool Connection::operator==(const Connection & conn)
{
    return this->_id == conn._id;
}

bool Connection::operator!=(const Connection & conn)
{
    return this->_id != conn._id;
}

int Connection::getId() const 
{ 
    return this->_id; 
}

int Connection::getFd() const 
{ 
    return this->_fd;
}

std::string Connection::getAddress() const
{ 
    return this->_address;
}

std::string Connection::getPassword() const
{ 
    return this->_password;
}

int Connection::getPort() const 
{
    return this->_port;
}

bool Connection::isClosed() const 
{ 
    return this->_closed;
}

void closeFd(int fd) {
    close(fd);
}

void Connection::close() {
    if (this->_closed) {
        return;
    }
    closeFd(this->_fd);
    this->_closed = true; 
}

std::string Connection::str() const 
{
    std::stringstream ss;
    ss << "(" << this->_id << ")[" << this->_address << ":" << this->_port << "]";
    return ss.str();
}

size_t Connection::sendMessage (const std::string &msg)
{
    if (this->isClosed()) {
        return -1;
    }
    
    int written = write(this->getFd(), msg.c_str(), msg.size());
    
    if (written == -1) {
        this->close();
        return 0;
    }
    Log::debug("Sent to " + this->str() + " " + msg);
    return written;
}

std::string Connection::readMessage()
{
    if (this->isClosed()) {
        return "";
    }

    int buffSize = this->_readBufferSize;
    char buff[buffSize];

    const int bytesRead = read(this->_fd, buff, buffSize);

    if (bytesRead < 1) { // end of file or error
        this->_closed = true;
        return "";
    }

    char resultStr[buffSize + 1];
    char nullByte = '\0';

    strncpy(resultStr, buff, bytesRead);
    resultStr[bytesRead] = nullByte;

    Log::debug("Received from " + this->str() + " " + resultStr);
    return resultStr;
}