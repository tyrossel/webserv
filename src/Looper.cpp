#include "Looper.hpp"
#include "webserv.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Looper::Looper() : _timeout(15)  {}

Looper::Looper(const Looper &other)
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
    _active_servers = rhs._active_servers;
    _ready_fd = rhs._ready_fd;
    _responses = rhs._responses;
    _requests = rhs._requests;
    _last_activity = rhs._last_activity;
    _raw_request = rhs._raw_request;
    _active_fd_set = rhs._active_fd_set;

	return *this;
}

Looper::~Looper()
{
    for (std::map<long int, Server *>::iterator it = _active_servers.begin(); it != _active_servers.end(); it++)
        it->second->close(it->first);
    _servers.clear();
    _active_servers.clear();
    _ready_fd.clear();
    _responses.clear();
    _requests.clear();
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
		std::cout << ft::timestamp(TIMESTAMP_FORMAT) << std::endl;
        std::cout << BLUE << request << RESET;
    }
    else {
        std::cout << YELLOW << "=============================================================\n"
                  << ft::timestamp(TIMESTAMP_FORMAT)
                  << "Connection receive from client on ["
                  <<  addr_str << ":" << ntohs(req_addr.sin_port) << "]\n"
                  << BLUE << ft::timestamp(TIMESTAMP_FORMAT)
                  << "Request [" << ft::RequestToString(request.getMethod())
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

/**************************************************************************************/
/*                                  EXTRACT REQUEST                                   */
/**************************************************************************************/

int Looper::startParsingRequest(int socket)
{
    RequestParser   parser;
    Request         request;

    //TODO : check return of parsing ??
    if (parser.parseRequest(_raw_request[socket]) == -1)
        return parser.getRequest().getStatus();

    request = parser.getRequest();

    struct sockaddr_in req_addr;
    socklen_t addr_len = sizeof(req_addr);
    getsockname(socket, (struct sockaddr *)&req_addr, &addr_len);

    const Server &srv = request.FindServer(_servers, req_addr);
    const Location &loc = request.FindLocation(srv);

    printLog(request, socket);

    if (!request.isValid(loc))
    {
        _requests.insert(std::make_pair<long, Request>(socket, request));
        buildResponse(socket, loc);
    }
    else
    {
        request.updatePathWithLocation(loc);
        _requests.insert(std::make_pair<long, Request>(socket, request));
        buildResponse(socket, loc);
    }

    _last_activity[socket] = ::time(NULL);

    return 0;
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
        if (ret == -1)
            std::cout << RED << "An error occured when reading the request" << RESET << std::endl;
        return -1;
    }
    _raw_request[socket].append(buffer, ret);

    size_t headers_end = _raw_request[socket].find("\r\n\r\n");
	if ((headers_end == std::string::npos && _raw_request.size() > HEADERS_MAX_LENGTH)
			|| headers_end > HEADERS_MAX_LENGTH)
	{
		return HEADERS_TOO_LARGE;
	}
	else if (headers_end != std::string::npos)
    {
        if (_raw_request[socket].find("Content-Length:") == std::string::npos)
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

        size_t len = 0;
		if (_raw_request[socket].find("Content-Length:") != std::string::npos)
			len = std::atoi(_raw_request[socket].substr(_raw_request[socket].find("Content-Length:") + 15, 10).c_str());
        if (_raw_request[socket].size() >= len + headers_end)
            return 0;
        else
            return 1;
    }
	// TODO : What happens when returning 1 ?!?
    return 1;
}

/**************************************************************************************/
/*                                  LOOP RELATED                                      */
/**************************************************************************************/
void Looper::loop()
{
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

		long ret_val = _active_servers[fd]->send(_responses[fd]);

		if (_requests[fd].getHeaders()["Connection"] == "close")
		{
			FD_CLR(fd, &_active_fd_set);
			FD_CLR(fd, &reading_fd_set);
			_active_servers[fd]->close(fd);
			_active_servers.erase(fd);
		}
		if (ret_val >= 0) // Comm OK, delete ressources
		{
			_responses.erase(fd);
			_requests.erase(fd);
		}
		else
		{
			_responses.erase(fd);
			_requests.erase(fd);
			FD_CLR(fd, &_active_fd_set);
			FD_CLR(fd, &reading_fd_set);
			_active_servers.erase(fd);
		}
		it = _ready_fd.erase(it); // erase the fd from vector when comm is over
    }
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

        switch(ret_val) {
			case 0: // Request fully received
            {
                int status = startParsingRequest(socket);
                if (status > 0)
                    buildErrorResponse(socket, status);
                _raw_request.erase(socket);
                // we store the socket fd into our ready_fd vector since we want to keep the channel open
                _ready_fd.push_back(socket);
                ++it;
                break;
            }
			case 1: // Must read again to get the full request
				break;
			case -1: // Communication error
				FD_CLR(socket, &_active_fd_set);
				FD_CLR(socket, &reading_fd_set);
				_active_servers[socket]->close(socket);
				_last_activity.erase(socket);
				_active_servers.erase(it++);
				break;
			default: // Request not conform: Respond an error
                buildErrorResponse(socket, ret_val);
				_raw_request.erase(socket);
				_ready_fd.push_back(socket);
				_last_activity[socket] = ::time(NULL);
				break;
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
        _responses.erase(it->first);
        _requests.erase(it->first);
    }
    _active_servers.clear();
    _ready_fd.clear();

    FD_ZERO(&_active_fd_set);
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        FD_SET((*it).getFd(), &_active_fd_set);
}

/**************************************************************************************/
/*                                  RESPONSE CRAFTING                                 */
/**************************************************************************************/

int Looper::buildResponse(long socket, const Location &loc)
{
    Request &req = _requests[socket];
    Response ret(socket, &loc, _active_servers[socket], req.getStatus());
    const Redirection *redir = loc.findRedirection(req.getPath());
    if (redir)
    {
        ret.buildRedirectionResponse(*redir);
        _responses.insert(std::pair<long int, Response>(socket, ret));
        return 1;
    }
    switch (req.getMethod()) {
        case Get:
            ret.buildGetResponse(req);
            _responses.insert(std::pair<long int, Response>(socket, ret));
            break;
        case Post:
            ret.buildPostResponse(req);
            _responses.insert(std::pair<long int, Response>(socket, ret));
            break;
        case Delete:
            ret.buildDeleteResponse(req);
            _responses.insert(std::pair<long int, Response>(socket, ret));
            break;
        default:
			ret.buildErrorResponse(NOT_IMPLEMENTED);
			break;
    }
    return (1);
}

void Looper::buildErrorResponse(long socket, int status)
{
    Response ret(socket);

    ret.buildErrorResponse(status);
    _responses.insert(std::pair<long int, Response>(socket, ret));
}
