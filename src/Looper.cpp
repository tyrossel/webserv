#include "Looper.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Looper::Looper() : _config(), _max_fd(), _servers() {}

Looper::Looper(const Looper &other) : _config(other._config), _max_fd(other._max_fd), _servers(other._servers)
{
    *this = other;
}

Looper::~Looper() {}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

void Looper::setupLoop()
{
    for (unsigned int i = 0; i < _servers.size(); i++)
        _servers[i].buildServer();
}

void Looper::log(std::string message)
{
    std::cout << message << std::endl;
}

void Looper::addServer(Server &server)
{
    this->_servers.push_back(server);
}

void Looper::setMaxFd()
{
    _max_fd = 0;
    for (unsigned int i = 0; i < _servers.size(); i++)
    {
        if (_servers[i].getSock() > _max_fd)
            _max_fd = _servers[i].getSock();
    }
}

void Looper::loop()
{

}
