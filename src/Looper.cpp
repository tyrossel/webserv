#include "Looper.hpp"
#include "webserv.hpp"
#include "Utils.hpp"
#include <unistd.h>

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Looper::Looper() : _config(), _max_fd(), _timeout(2), _servers() {}

Looper::Looper(const Looper &other) : _config(other._config), _max_fd(other._max_fd), _servers(other._servers)
{
    *this = other;
}

Looper::Looper(const Config &cfg) : _timeout(cfg.getTimeout())
{
	_servers = cfg.getServer();
}

Looper &Looper::operator=(const Looper &rhs)
{
	if (&rhs == this)
		return *this;
	_config = rhs._config;
	_max_fd = rhs._max_fd;
	_timeout = rhs._timeout;
	_servers = rhs._servers;
	// Other variables are not copied.
	return *this;
}

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

    ft::bzero(&buffer, BUFFER_SIZE);

    //TODO : timestamp

    ret = recv(socket, buffer, BUFFER_SIZE-1, 0);
    if (ret <= 0)
    {
        std::cout << strerror(errno) << std::endl;
        if (ret == -1)
            std::cout << RED << "An error occured when reading the request" << RESET << std::endl;
        return -1;
    }
    _raw_request[socket].append(buffer, ret);

    //TODO : remove
    std::cout << MAGENTA << _raw_request[socket] << std::endl;

    size_t headers_end = 0;
    if ((headers_end = _raw_request[socket].find("\r\n\r\n")) != std::string::npos)
    {
        if (_raw_request[socket].find("Content-Length: ") == std::string::npos)
        {
            if (_raw_request[socket].find("Transfer-Encoding: chunked") != std::string::npos)
            {
                std::string end = _raw_request[socket].substr(_raw_request[socket].size() - 5, 5);
                if (end == "0\r\n\r\n")
                    return 0;   // End of chunked body found, time to process the request
                else
                    return 1;   // End of body not found, need to read more
            }
        }

        // Here we want to see if our _raw_request size == Content-Length + Headers size
        headers_end += 4; // We have to add \r\n\r\n (4 char) because find exclude them

        //TODO : update this shit with our function
        size_t len = std::atoi(_raw_request[socket].substr(_raw_request[socket].find("Content-Length: ") + 16, 10).c_str());
        std::cout << CYAN << _raw_request[socket].size() << " || " << len + headers_end;
        if (_raw_request[socket].size() >= len + headers_end)
            return 0;
        else
            return 1;
    }

    return 1;
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
			// Handle connection timeouts
            // select will wait for an event on the set given
            ret = select(_max_fd + 1, &reading_fd_set, &writing_fd_set, NULL, &timeout);

			checkConnectionTimeout();
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

void Looper::checkConnectionTimeout()
{
	if (_timeout < 0)
		return ;
	for (std::map<long int, Server *>::iterator it(_active_servers.begin()); it != _active_servers.end();)
	{
		long int socket = it->first;
		time_t time_now(::time(NULL)), time_last(0);
		::time(&time_now);

		// TODO TYR: Remove, in order to close if not found
		time_last = time_now;

		std::map<long int, std::time_t>::iterator time_it = _last_activity.find(socket);
		if (time_it != _last_activity.end())
			time_last = time_it->second;
		if (time_now - time_last > _timeout)
		{
			FD_CLR(socket, &_active_fd_set);
			_last_activity.erase(socket);
			_active_servers[socket]->close(socket);
			_active_servers.erase(it++);
			it = _active_servers.begin();
		}
		else
			it++;
	}
}

void Looper::sendResponse(fd_set &reading_fd_set, fd_set &writing_fd_set, fd_set &_active_fd_set)
{
    for (std::vector<int>::iterator it = _ready_fd.begin(); it != _ready_fd.end() && RUNNING;)
    {
		int fd(*it);

        if (!FD_ISSET(fd, &writing_fd_set))
        {
			it++;
			continue;
		}

		long ret_val = _active_servers[fd]->send(_response[fd]);

		if (_request[fd].getHeaders()["Connection"] == "close")
		{
			FD_CLR(fd, &_active_fd_set);
			FD_CLR(fd, &reading_fd_set);
			_active_servers[fd]->close(fd);
			_active_servers.erase(fd);
		}
		if (ret_val >= 0) // Comm OK, delete ressources
		{
			_response.erase(fd);
			_request.erase(fd);
		}
		else
		{
			_response.erase(fd);
			_request.erase(fd);
			FD_CLR(fd, &_active_fd_set);
			FD_CLR(fd, &reading_fd_set);
			_active_servers.erase(fd);
		}
		it = _ready_fd.erase(it); // erase the fd from vector when comm is over
    }
}

int Looper::startParsingRequest(int socket)
{
    RequestParser   parser;
    Request         request;

    //TODO : check return of parsing ??
    parser.parseRequest(_raw_request[socket]);
    request = parser.getRequest();

    struct sockaddr_in req_addr;
    socklen_t addr_len = sizeof(req_addr);
    getsockname(socket, (struct sockaddr *)&req_addr, &addr_len);

    const Server *srv = NULL;
    if (request.getStatus() == 0)
        srv = request.FindServer(_servers, req_addr);
    //TODO : Really need this ? Because if parsing fail, no serv will be found
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

    _last_activity[socket] = ::time(NULL);

    return 0;
}

void Looper::requestProcess(fd_set &reading_fd_set)
{
    for (std::map<long, Server *>::iterator it = _active_servers.begin(); it != _active_servers.end() && RUNNING;)
    {
        long socket = it->first;

        if (!FD_ISSET(socket, &reading_fd_set))
		{
			it++;
			continue;
		}

		long ret_val = readFromClient(socket);

		if (ret_val == 0)
		{
			// we store the socket fd into our ready_fd vector since we want to keep the channel open
            startParsingRequest(socket);
            _raw_request.erase(socket);
			_ready_fd.push_back(socket);
			++it;
		}
		else if (ret_val == -1)
		{
			// in case of error, we clear as done previously
			FD_CLR(socket, &_active_fd_set);
			FD_CLR(socket, &reading_fd_set);
			_active_servers[socket]->close(socket);
			_last_activity.erase(socket);
			_active_servers.erase(it++);
		}
        break;
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
