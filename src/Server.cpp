#include "Server.hpp"
#include "Response.hpp"
#include <stdexcept>

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Server::Server()
{
	addLocation("/", Location());
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
	this->_name = rhs._name;
	this->_location = rhs._location;
    this->_addr = rhs._addr;
    return (*this);
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

long Server::createSocket()
{
    long socket;

    socket = accept(_fd, NULL, NULL);
    if (socket == -1)
        std::cout << RED << "Could not create socket for this address : " << _address << ". Socket return : " << socket << RESET << std::endl;
    else
        fcntl(socket, F_SETFL, O_NONBLOCK);
    return (socket);
}

void Server::setAddress()
{
	ft::bzero((char *)&_addr, sizeof(_addr));

    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(_address.c_str());
    _addr.sin_port = htons(_port);
}

int Server::send(int socket, const Response &resp)
{
    long bytes_sent_total = 0;
    long bytes_sent_now = 0;
	std::string response_txt = resp.to_string();

    while (bytes_sent_total < static_cast<long>(response_txt.size()))
    {
        bytes_sent_now = ::send(socket, response_txt.c_str() + bytes_sent_total, response_txt.size() - bytes_sent_total, 0);
        if (bytes_sent_now == -1)
        {
            std::cout << "Error on sockets 🔥. Send failed" << std::endl;
            break;
        }
        bytes_sent_total += bytes_sent_now;
        usleep(100);
    }
    if (bytes_sent_total <= 0)
        return (-1);
    else
        return (bytes_sent_total);
}

int Server::setupListen()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
    {
        std::cerr << "Could not create server." << std::endl;
        return (-1);
    }
	int true_var = true;
	if (setsockopt(_fd,SOL_SOCKET,SO_REUSEADDR,&true_var,sizeof(int)) == -1)
	{
		std::cerr << "setsockopt error" << std::endl;
		return (-1);
	}
    this->setAddress();
    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
    {
        std::cerr << RED << "(╯‵□′)╯︵┻━┻ Could not bind host " <<
			_address << ":" << _port << " ┻━┻ ︵ ＼( °□° ＼)" << RESET << std::endl;
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
    if (socket <= 0)
		return;
	::close(socket);
    std::cout << ft::timestamp(TIMESTAMP_FORMAT) << "Close connection on socket " << socket << std::endl;
}

int Server::buildServer() { return (this->setupListen()); }

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/
int                                     Server::getPort() const { return (this->_port); }
long                                    Server::getFd() const { return (this->_fd); }
std::string							    Server::getAddress() const { return (this->_address); }
std::vector<std::string>                Server::getName() const { return (this->_name); }
const std::map<std::string, Location>   &Server::getLocations() const { return (this->_location); }

/**************************************************************************************/
/*                                  ADDERS                                            */
/**************************************************************************************/
void    Server::addPort(int port) { _port = port; }
void	Server::addAddress(const std::string &address) { _address = address; }
void    Server::addName(const std::string &name) { _name.push_back(name); }
void    Server::addLocation(const std::string &key, const Location &location) { _location[key] = location; }
