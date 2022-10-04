#include "Server.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Server::Server() : _port(), _fd(), _host(), _addr() {}

Server::Server(int port, std::string host) : _port(port), _fd(), _addr()
{
    // TODO : HOST will have to be converted to u_int32 to be sent to htonl
    (void)host;
    _host = htonl(INADDR_ANY);
}

Server::Server(const Server &other)
{
    *this = other;
}

Server::~Server() {}

/**************************************************************************************/
/*                                 OPERATOR OVERLOAD                                  */
/**************************************************************************************/
/*Server &operator=(const Server &rhs)
{
    this->_port = rhs._port;
    this->_fd = rhs._fd;
    this->_host = rhs._host;
    this->_addr = rhs._addr;
    return (*this);
}*/

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

void Server::log(std::string message)
{
    std::cout << message << std::endl;
}

long    Server::createSocket()
{
    long socket;

    socket = accept(_fd, NULL, NULL);
    if (socket == -1)
        std::cout << RED << "Could not create socket for host " << _host << RESET << std::endl;
    else
        fcntl(socket, F_SETFL, O_NONBLOCK);
    return (socket);
}

void    Server::setAddress()
{
    memset((char *)&_addr, 0, sizeof(_addr)); //TODO : import our memset

    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = htonl(_host);
    _addr.sin_port = htons(_port);
}

int     Server::send(long socket, std::map<long, std::string> response)
{
    int ret = 0;
    if (response[socket].c_str())
        ret = ::send(socket, response[socket].c_str(), response[socket].size(), 0);
    else
        std::cout << "Error on sockets 🔥" << std::endl;
    (void)ret;
    // TODO: Better handling with ret
    return (0);
}

int    Server::setupListen()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
    {
        std::cerr << "Could not create server." << std::endl;
        return (-1);
    }
    this->setAddress();
    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
    {
        std::cerr << RED << "(╯‵□′)╯︵┻━┻ Could not bind port ┻━┻ ︵ ＼( °□° ＼)" << RESET << std::endl;
        return (-1);
    }
    if (listen(_fd, 1000) == -1)
    {
        std::cerr << "Could not listen."<< std::endl;
        return (-1);
    }
    return (0);
}

void Server::close(int socket)
{
    if (socket > 0)
        ::close(socket);
    //we might want to erase the request here too
}

int Server::buildServer() { return (this->setupListen()); }

int Server::getPort() { return (this->_port); }

long Server::getFd() { return (this->_fd); }


