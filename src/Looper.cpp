#include "Looper.hpp"
#include "webserv.hpp"
#include "Utils.hpp"
#include <unistd.h>

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Looper::Looper() : _config(), _max_fd(), _servers() {}

Looper::Looper(const Looper &other) : _config(other._config), _max_fd(other._max_fd), _servers(other._servers)
{
    *this = other;
}

Looper::Looper(const Config &cfg) { _servers = cfg.getServer(); }

Looper::~Looper()
{
    for (std::map<long int, Server *>::iterator it = _active_servers.begin(); it != _active_servers.end(); it++)
        it->second->close(it->first);
    _servers.clear();
    _active_servers.clear();
    _ready_fd.clear();
    _response.clear();
    _request.clear();
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/
void Looper::printLog(const Request &request, int socket)
{
    struct sockaddr_in req_addr;
    socklen_t	req_len = sizeof(req_addr);
    getsockname(socket, (struct sockaddr *)&req_addr, &req_len);
    char *addr_str = inet_ntoa(req_addr.sin_addr);

    if (VERBOSE) {
        std::cout << "================== REQUEST ==================" << std::endl;
        std::cout << BLUE << request << RESET;
    }
    else {
        std::cout << GREEN << "===================================================\n"
                  << "Connection receive from client on ["
                  <<  addr_str << ":" << ntohs(req_addr.sin_port) << "]\n"
                  << BLUE << "Request [" << ft::RequestToString(request.getMethod())
                  << " " << request.getPath() << " HTTP " << request.getVersion() << "]"
                  << RESET << std::endl;
    }
}

void Looper::addServer(Server &server) { this->_servers.push_back(server); }

void Looper::setMaxFd()
{
    _max_fd = 0;
    for (unsigned int i = 0; i < _servers.size(); i++)
    {
        if (_servers[i].getFd() > _max_fd)
            _max_fd = _servers[i].getFd();
    }
}

int Looper::setupLoop()
{
    long		fd;

    // this is for one single listen. we will see when parsing is done for more
    FD_ZERO(&_active_fd_set);
    _max_fd = 0;

    for (unsigned int i = 0; i < _servers.size(); i++)
    {
        if (_servers[i].buildServer() != -1)
        {
            fd = _servers[i].getFd();
            FD_SET(fd, &_active_fd_set);
            if (fd > _max_fd)
                _max_fd = fd;
        }
    }
    return _max_fd == 0 ? -1 : 0;
}

/**************************************************************************************/
/*                                  RESPONSE CRAFTING                                 */
/**************************************************************************************/

int Looper::buildResponse(long socket, const Location *loc)
{
	Request &req = _request[socket];
    Response ret(socket, loc, _active_servers[socket]);
	const Redirection *redir = loc->findRedirection(req.getPath());
	if (redir)
	{
		ret.buildRedirectionResponse(*redir);
		_response.insert(std::pair<long int, Response>(socket, ret));
		return 1;
	}
    switch (req.getMethod()) {
        case Get:
			// TODO: Remove loc as param
            ret.buildGetResponse(req, loc);
            _response.insert(std::pair<long int, Response>(socket, ret));
            break;
        case Post:
            ret.buildPostResponse(req, loc);
            _response.insert(std::pair<long int, Response>(socket, ret));
            break;
        case Delete:
            ret.buildDeleteResponse(req);
            _response.insert(std::pair<long int, Response>(socket, ret));
            break;
        default:
            std::cout << RED << req.getMethod() << " is not a method that the server threats." << RESET << std::endl;
    }
    (void)ret;
    return (1);
}

int Looper::readFromClient(long socket)
{
    char	        buffer[BUFFER_SIZE];
    int		        ret;
    std::string     string_buffer;
    RequestParser   parser;
    Request         request;

    ft::bzero(&buffer, BUFFER_SIZE);

    while ((ret = recv(socket, buffer, BUFFER_SIZE-1, 0)) > 0)
    {
        string_buffer += buffer;
        ft::bzero(&buffer, BUFFER_SIZE);
    }

    if (!string_buffer.empty()) {
        parser.parseRequest(string_buffer);
        request = parser.getRequest();

		struct sockaddr_in req_addr;
		socklen_t addr_len = sizeof(req_addr);
		getsockname(socket, (struct sockaddr *)&req_addr, &addr_len);
		const Server *srv = NULL;
        if (request.getStatus() == 0)
            srv = request.FindServer(_servers, req_addr);
		if (!srv)
		{
			std::cerr << RED << "No corresponding server was found" << RESET << std::endl;
			return -1;
		}

		// TODO TYR: Check if no server corresponds
		const Location *loc = NULL;
		if (request.getStatus() == 0)
			loc = request.FindLocation(*srv);

		if(!request.isValid(loc))
			return (-1);

        printLog(request, socket);

		request.updatePathWithLocation(loc);

        _request.insert(std::make_pair<long, Request>(socket, request));
        buildResponse(socket, loc);
    }
    return (string_buffer.size());
}

/**************************************************************************************/
/*                                  LOOP RELATED                                      */
/**************************************************************************************/
void Looper::loop()
{
    ft::setupSignals();
    while (RUNNING)
    {
        fd_set		    reading_fd_set;
        fd_set		    writing_fd_set;
        struct timeval  timeout;
        int				ret = 0;

        while (ret == 0)
        {
            // Setting the timeout for select
            timeout.tv_sec  = 3;
            timeout.tv_usec = 0;
            // Copying the content for the reading set into the active set
            ft::memcpy(&reading_fd_set, &_active_fd_set, sizeof(_active_fd_set));
            FD_ZERO(&writing_fd_set);
            // here we set the already active fd's in the writing fd's
            for (std::vector<int>::iterator it = _ready_fd.begin(); it != _ready_fd.end(); it++)
                FD_SET(*it, &writing_fd_set);
            // select will wait for an event on the set given
            ret = select(_max_fd + 1, &reading_fd_set, &writing_fd_set, NULL, &timeout);
        }
        // ret will be greater than 0 if any valid event is catched
        if (ret > 0 && RUNNING)
        {
            // Prepare and send the response to the client
            sendResponse(reading_fd_set, writing_fd_set, _active_fd_set);
            // Processing the request received. Reading and parsing the request to prepare the response
            requestProcess(reading_fd_set);
            // Catching the new incoming communications to prepare the channel
            catchCommunication(reading_fd_set, ret);
        }
        else
        {
            if (RUNNING)
                selectErrorHandle();
        }
    }
}

void Looper::sendResponse(fd_set &reading_fd_set, fd_set &writing_fd_set, fd_set &_active_fd_set)
{
    for (std::vector<int>::iterator it = _ready_fd.begin(); it != _ready_fd.end() && RUNNING; it++)
    {
        if (FD_ISSET(*it, &writing_fd_set))
        {
            long ret_val = _active_servers[*it]->send(_response[*it]);
            if (ret_val >= 0)
            {
                _response.erase(*it); // erase the response from map when comm is over
                _request.erase(*it); // erase the socket from the map
                _ready_fd.erase(it); // erase the fd from vector when comm is over
            }
            else
            {
                // Here we will remove the fd we catched in the error
                // and clear all communication and all open channels.
                // The fd and the active_server will be removed too
                FD_CLR(*it, &_active_fd_set);
                FD_CLR(*it, &reading_fd_set);
                _active_servers.erase(*it);
                _ready_fd.erase(it);
            }
            ret_val = 0;
            break;
        }
    }
}

void Looper::requestProcess(fd_set &reading_fd_set)
{
    for (std::map<long, Server *>::iterator it = _active_servers.begin(); it != _active_servers.end() && RUNNING; it++)
    {
        long socket = it->first;

        if (FD_ISSET(socket, &reading_fd_set))
        {
            long ret_val = readFromClient(socket);

            if (ret_val > 0)
            {
                // we store the socket fd into our ready_fd vector since we want to keep the channel open
                _ready_fd.push_back(socket);
            }
            else if (ret_val <= 0)
            {
                // in case of error, we clear as done previously
                FD_CLR(socket, &_active_fd_set);
                FD_CLR(socket, &reading_fd_set);
                _active_servers.erase(socket);
                // close(socket);
                it = _active_servers.begin();
            }
            ret_val = 0;
            break;
        }
    }
}

void Looper::catchCommunication(fd_set &reading_fd_set, int ret)
{
    for (std::vector<Server>::iterator it = _servers.begin(); ret && it != _servers.end() && RUNNING; it++)
    {
        long fd = (*it).getFd();

        if (FD_ISSET(fd, &reading_fd_set))
        {
            // Creating a socket with accept
            long socket = (*it).createSocket();

            // We check if the socket has been created
            if (socket != -1)
            {
                // if it has a valid socket fd, we enter it into our active_fd_set
                // and we add it into the _active_servers map with his fd as a key for the sock
                FD_SET(socket, &_active_fd_set);
                _active_servers.insert(std::make_pair(socket, &(*it)));
                // Setting max_fd if the new fd from the socket is greater
                if (socket > _max_fd)
                    _max_fd = socket;
            }
            ret = 0;
            break;
        }
    }
}

void Looper::selectErrorHandle()
{
    // An issue with select has been caught
    // In this case we will close every opened sockets, clear the active_servers map and the read_fd's
    // FD_ZERO to clear the active_fd_set and re set all the sockets to restart the server
    std::cout << "Select had an issue !" << std::endl;
    for (std::map<long, Server *>::iterator it = _active_servers.begin(); it != _active_servers.end(); it++) {
        it->second->close(it->first);
        _response.erase(it->first);
        _request.erase(it->first);
    }
    _active_servers.clear();
    _ready_fd.clear();

    FD_ZERO(&_active_fd_set);
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        FD_SET((*it).getFd(), &_active_fd_set);
}
