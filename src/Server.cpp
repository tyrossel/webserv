#include "Server.hpp"
#include <stdexcept>

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Server::Server() : _port(), _fd(), _host(), _name(), _location(), _addr()
{
	addLocation("", Location());
}

Server::Server(const Server &other)
{
    *this = other;
}


Server::~Server() {}

/**************************************************************************************/
/*                                 OPERATOR OVERLOAD                                  */
/**************************************************************************************/
Server &Server::operator=(const Server &rhs)
{
	if (&rhs == this)
		return *this;
    this->_port = rhs._port;
	this->_address = rhs._address;
    this->_fd = rhs._fd;
    this->_host = rhs._host;
	this->_name = rhs._name;
	this->_location = rhs._location;
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
    std::cout << YELLOW << "SENDING " << ret << " BYTES TO " << socket << RESET << std::endl;
    if (ret <= 0)
        return (-1);
    else
        return (ret);
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
}

int Server::buildServer() { return (this->setupListen()); }

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/
int                                 Server::getPort() const { return (this->_port); }
std::string							Server::getAddress() const { return (this->_address); }
std::vector<std::string>            Server::getName() const { return (this->_name); }
long                                Server::getFd() const { return (this->_fd); }
unsigned int                        Server::getHost() const { return (this->_host); }
const std::map<std::string, Location> &Server::getLocations() const { return (this->_location); }

// TODO TYR: Remove
std::string				            Server::getRoot() const { return (this->_location.at("").root_dir); }
std::vector<std::string>            Server::getIndex() const { return (this->_location.at("").indexes); }
std::string							Server::getCGIBin() const { return (this->_location.at("").cgi_bin); }
std::vector<std::string>            Server::getCGIExtensions() const { return (this->_location.at("").cgi_extensions); }

/**************************************************************************************/
/*                                  ADDERS                                            */
/**************************************************************************************/
void    Server::addPort(int port) { _port = port; }
void	Server::addAddress(const std::string &address) { _address = address; }
void    Server::addName(const std::string &name) { _name.push_back(name); }
void    Server::addLocation(const std::string &key, const Location &location) { _location[key] = location; }
void    Server::addHost(const unsigned int &host) { _host = host; }

