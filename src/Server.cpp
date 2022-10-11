#include "Server.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Server::Server() : _port(), _fd(), _host(), _root(), _index(), _name(), _location(), _addr() {}

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
    std::cout << YELLOW << "SENDING " << ret << " BYTES TO " << socket << std::endl;
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
    // TODO: we might want to erase the request here too
}

int Server::buildServer() { return (this->setupListen()); }

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/
int                                 Server::getPort() const { return (this->_port); }
std::vector<std::string>            Server::getName() const { return (this->_name); }
std::vector<std::string>            Server::getRoot() const { return (this->_root); }
std::vector<std::string>            Server::getIndex() const { return (this->_index); }
std::map<std::string, std::string>  Server::getLocation() const { return (this->_location); }
long                                Server::getFd() const { return (this->_fd); }
unsigned int                        Server::getHost() const { return (this->_host); }

/**************************************************************************************/
/*                                  ADDERS                                            */
/**************************************************************************************/
void    Server::addPort(int port) { _port = port; }
void    Server::addName(const std::string &name) { _name.push_back(name); }
void    Server::addRoot(const std::string &root) { _root.push_back(root); }
void    Server::addIndex(const std::string &index) { _index.push_back(index); }
void    Server::addLocation(const std::string &key, const std::string &value) { _location[key] = value; }
void    Server::addHost(const unsigned int &host) { _host = host; }

