
#include "Connection.hpp"

#include <cstring>
#include <unistd.h>

Connection::Connection(int id, int fd, int port, const std::string& address, 
			   		   const std::string& password)
{
    this->_id = id;
    this->_fd = fd;
    this->_port = port;
    this->_address = address;
    this->_password = password;
    this->_closed = false;
}

Connection::~Connection() {}

int Connection::getId() const { return this->_id; }
int Connection::getFd() const { return this->_fd; }
std::string Connection::getAddress() const { return this->_address; }
std::string Connection::getPassword() const { return this->_password; }
int Connection::getPort() const { return this->_port; }
bool Connection::isClosed() const { return this->_closed; }
void Connection::setStatus(bool status) { this->_closed = status; }


size_t Connection::sendMessage (const std::string &msg) const {
    if (this->_closed) {
        return -1;
    }

    // TODO: close if error? ex: -1
    return write(this->getFd(), msg.c_str(), msg.size());
}

std::string Connection::readMessage()
{
    char buff[1024];

    const size_t readed = read(this->_fd, buff, 1024);

    // TODO: if < 0 than close connection
    if (readed > 1024) {
        this->_closed = true;
        return "";
    }

    char str[1024];
    strncpy(str, buff, readed);
    str[readed] = '\0';

    return str;
}