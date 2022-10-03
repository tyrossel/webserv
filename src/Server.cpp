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
        std::cout << "Could not create socket for host " << _host << std::endl;
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

int     Server::send(long socket)
{
    int ret = 0;
    std::cout << "I am preparing an answer on socket : " << socket << std::endl;
    ret = ::send(socket, "HTTP/1.1 200 OK\nServer: WetServ/1.0.0\nTransfer-Encoding: identity\nContent-Length: 12\nContent-Type: text/plain\n\nHello, world\0", 124, 0);
    std::cout << "send return : " << ret << std::endl;
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
        std::cerr <<"Could not bind port " << std::endl;
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


