#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <poll.h>
# include <vector>
# include <string>

class Connection
{
    int _id;
    int _fd;
    int _port;
    std::string _address;
    std::string _password;
    
	bool _closed;

public:

    Connection(int id, int fd, int port, const std::string& address, 
			   const std::string& password);
    ~Connection();

    int         getId() const;
    int         getFd() const;
    int         getPort() const ;
    std::string getAddress() const;
    std::string getPassword() const;
    
    bool        isClosed() const;
	void 		setStatus(bool status);

    size_t      sendMessage(const std::string &msg) const;
    std::string readMessage();

    friend bool operator==(const Connection &lhs, const Connection &rhs) {
        return lhs._fd == rhs._fd;
    }

    friend bool operator!=(const Connection &lhs, const Connection &rhs) {
        return !(lhs == rhs);
    }
};

typedef std::vector<Connection> Session;


#endif
