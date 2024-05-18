/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:48:25 by gasouza           #+#    #+#             */
/*   Updated: 2024/05/15 20:26:48 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

#include <cstring>
#include <unistd.h>
#include <sstream>

Connection::Connection(int id, int fd, const std::string& address, int port, const std::string& password)
{
    this->_id = id;
    this->_fd = fd;
    this->_port = port;
    this->_address = address;
    this->_password = password;
    this->_closed = false;
    this->_readBufferSize = 1024;
}

Connection::~Connection() {}

int Connection::getId() const { return this->_id; }
int Connection::getFd() const { return this->_fd; }
std::string Connection::getAddress() const { return this->_address; }
std::string Connection::getPassword() const { return this->_password; }
int Connection::getPort() const { return this->_port; }
bool Connection::isClosed() const { return this->_closed; }

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
    ss << "(" << this->_id << ") " << this->_address << ":" << this->_port;
    return ss.str();
}

size_t Connection::sendMessage (const std::string &msg) const {
    if (this->_closed) {
        return -1;
    }
    return write(this->getFd(), msg.c_str(), msg.size());
}

std::string Connection::readMessage()
{
    if (this->_closed) {
        return "";
    }

    int buffSize = this->_readBufferSize;
    char buff[buffSize];

    const size_t bytesRead = read(this->_fd, buff, buffSize);

    if (bytesRead < 1) { // end of file or error
        this->_closed = true;
        return "";
    }

    char resultStr[buffSize + 1];
    char nullByte = '\0';

    strncpy(resultStr, buff, bytesRead);
    resultStr[bytesRead] = nullByte;

    return resultStr;
}