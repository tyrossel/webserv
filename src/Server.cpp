#include "Server.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Server::Server() : _port(), _sock(), _host(), _addr() {}

Server::Server(int port, std::string host) : _port(port), _sock(), _addr()
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
Server &operator=(const Server &rhs)
{
    this->_port = rhs._port;
    this->_sock = rhs._sock;
    this->_host = rhs._host;
    this->_addr = rhs._addr;
    return (*this);
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

void Server::log(std::string message)
{
    std::cout << message << std::endl;
}

void    Server::createSocket()
{
    int opt = 1;

    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock < 0)
    {
        log("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        log("setsockopt failed");
        exit(EXIT_FAILURE);
    }
}

void    Server::setAddress()
{
    memset((char *)&_addr, 0, sizeof(_addr)); //TODO : import our memset

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = htonl(INADDR_ANY); //TODO : host here ?
}

void    Server::setupSocket()
{
    if (bind (_sock, (struct sockaddr *) &_addr, sizeof (_addr)) < 0)
    {
        log("bind");
        exit (EXIT_FAILURE);
    }

    if (listen(_sock, 100) < 0) {
        log("listen");
        exit(EXIT_FAILURE);
    }
}

void Server::buildServer()
{
    this->createSocket();
    this->setAddress();
    this->setupSocket();
}

int Server::getPort() { return (this->_port); }

int Server::getSock() { return (this->_sock); }
