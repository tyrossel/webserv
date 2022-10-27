#include "Looper.hpp"
#include "DirectoryListing.hpp"
#include "Utils.hpp"
#include "webserv.hpp"

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
void Looper::log(std::string message) { std::cout << message << std::endl; }

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
    if (_max_fd == 0)
    {
        std::cerr << "Could not setup looper !" << std::endl;
        return (-1);
    }
    else
        return (0);
}
/**************************************************************************************/
/*                                      CHECKERS                                      */
/**************************************************************************************/
int     Looper::checkCode(RequestParser request)
{
    if (request.getStatus() == 0)
        return HTTP_OK;
    else
        return (request.getStatus());
}

int     Looper::checkPath(long socket)
{
	std::string path = _request[socket].getLocation();

    ft::trimLeft(path, "/");
	if (ft::isDirectory(path))
	{
		return HTTP_OK;
	}
    std::ifstream test(path.c_str());
    if (!test.good())
    {
        _request[socket].setStatus(404);
        return NOT_FOUND;
    }
    else
        return HTTP_OK;
}

int Looper::secFetchImage(long socket)
{
    // This function just checks if the request is for an image
    std::map<std::string, std::string> tmp = _request[socket].getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
    {
        if (tmp.find("Sec-Fetch-Dest")->second == "image")
            return (0);
    }
    return (1);
}

/**************************************************************************************/
/*                                  RESPONSE CRAFTING                                 */
/**************************************************************************************/
void Looper::addErrorBodyToResponse(long socket, const Location &loc)
{
    std::string body;
    std::stringstream out;
	int status = _request[socket].getStatus();

	std::cerr << RED << "Status = " << status << RESET << std::endl;
	try
	{
		body.append(ft::readFile(loc.error_pages.at(status)));
	}
	catch (const std::exception& e)
	{
		// std::cerr << RED << "Custom error file not found" << loc.error_pages.at(status) << RESET << std::endl;
		body.append(ft::craftErrorHTML(_request[socket].getStatus()));
	}

    _response[socket].append("Content-Length: ");
    _response[socket].append(ft::itoa(body.size()));
    _response[socket].append("\r\n\r\n");
    _response[socket].append(body);
}

int Looper::writeResponseHeader(long socket)
{
    if (_request[socket].getStatus() == 0)
        _request[socket].setStatus(200);

    int status = _request[socket].getStatus();
    std::stringstream out;

    out << status;
    _response[socket].append("HTTP/1.1 ");
    if (ft::isOkHTTP(status)) {
        _response[socket].append(out.str());
        _response[socket].append(" OK\r\n");
    }
    else {
        _response[socket].append(out.str());
        _response[socket].append(" KO\r\n");
    }
    return (status);
}

int Looper::addHTTPHeader(long socket)
{
    if (checkCode(_request[socket]) != HTTP_OK) // checking if an error code has been parsed in request
        return writeResponseHeader(socket); // make this return return ret so we can pass it to body
    else if (checkPath(socket) != HTTP_OK) // functions will return the code catched
        return writeResponseHeader(socket);
    return writeResponseHeader(socket);
}

void Looper::addServerHeaderResponse(long socket)
{
    _response[socket].append("Server: WetServ/1.0.0\r\n");
}

void Looper::addContentType(long socket)
{
    _response[socket].append("Content-Type: ");
    std::map<std::string, std::string> tmp = _request[socket].getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
        _response[socket].append(tmp.find("Sec-Fetch-Dest")->second);
    else
        _response[socket].append("NONE");
    _response[socket].append("\r\n");
}

void Looper::addDate(long socket)
{
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    // convert now to tm struct for UTC
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    _response[socket].append("Date: ");
    _response[socket].append(dt);
}

void Looper::addBodyToResponse(long socket) // TODO: add file to read from (std::string path)
{
    std::string text;
    std::stringstream out;
	std::string loc = _request[socket].getLocation();
	std::string path = _request[socket].getPath();

	if (ft::isDirectory(loc))
	{
		text = createDirectoryListingBody(path, loc);
	}
	else
	{
		try
		{
			text = ft::readFile(loc);
		}
		catch (const std::exception& e)
		{
            _response[socket].append("\r\n\r\n");
			std::cerr << e.what() << std::endl;;
		}
	}
    out << text.size();
    std::string content_len = "Content-Length: " + out.str();
    _response[socket].append(content_len);
    _response[socket].append("\r\n\r\n");
    _response[socket].append(text);
}

int Looper::buildDeleteResponse(long socket, const Location &loc)
{
	(void)loc;

    int             ret = 0;

    _response.insert(std::make_pair<long int, std::string>(socket, ""));
    std::string path = _request[socket].getLocation();
    if (path[0] == '/')
        ft::trimLeft(path, "/");
    if (ft::isFile(path))
    {
        if (remove(path.c_str()) == 0)
            _request[socket].setStatus(204);
        else
            _request[socket].setStatus(403);
    }
    else
        _request[socket].setStatus(404);
    ret = addHTTPHeader(socket);
    addServerHeaderResponse(socket);
    addContentType(socket);
    addDate(socket);
    if (ft::isOkHTTP(ret))
        addBodyToResponse(socket);
    else
        addErrorBodyToResponse(socket);
    std::cout << "================== RESPONSE ==================" << std::endl;
    if (secFetchImage(socket))
        std::cout << GREEN << _response[socket] << RESET << std::endl;
    else
        std::cout << GREEN << "We sent an image" << RESET << std::endl;
    std::cout << "==============================================" << std::endl << std::endl;

    return (1);
}

void Looper::addContentLengthPOST(long socket)
{
    _response[socket].append("Content-Length: ");
    _response[socket].append(_request[socket].getHeaders().find("Content-Length")->second);
    _response[socket].append("\r\n");
}

int Looper::buildPostResponse(long socket, const Location &loc)
{
	(void)loc;
    int             ret = 0;

    _response.insert(std::make_pair<long int, std::string>(socket, ""));
    ret += addHTTPHeader(socket);
    addServerHeaderResponse(socket);
    addDate(socket);
    addContentLengthPOST(socket);
    CGI cgi(_request[socket].getHeaders(), _request[socket].getBody());
    ret = cgi.executeCgi(&_request[socket], _active_servers[socket]);
    _response[socket].append(cgi.getRetBody());
    if (VERBOSE) {
        std::cout << "================== CGI ==================" << std::endl;
        if (secFetchImage(socket))
            std::cout << _response[socket] << std::endl;
        else
            std::cout << GREEN << "We sent an image" << RESET << std::endl;
        std::cout << "==============================================" << std::endl << std::endl;
    }
    return (1);
}

int Looper::buildGetResponse(long socket, const Location &loc)
{
	(void)loc;
    int             ret = 0;

    _response.insert(std::make_pair<long int, std::string>(socket, ""));
    ret = addHTTPHeader(socket);
    addServerHeaderResponse(socket);
    addDate(socket);

    if (0) // CGI or not ?
    {
        CGI cgi(_request[socket].getHeaders(), _request[socket].getBody());
        ret = cgi.executeCgi(&_request[socket], _active_servers[socket]);
        // Here we remove HTTP EOF because the CGI we use cannot accept HTML in it.
        // If we send HTML inside the CGI he will TOUPPER the html which is.. shitty ?
        cgi.removeEOFHTTP();
        _response[socket].append(cgi.getRetBody());
        if (ft::isOkHTTP(ret))
            addBodyToResponse(socket);
        else
            addErrorBodyToResponse(socket);
        if (VERBOSE) {
            std::cout << "================== CGI ==================" << std::endl;
            if (secFetchImage(socket))
                std::cout << _response[socket] << std::endl;
            else
                std::cout << GREEN << "We sent an image" << RESET << std::endl;
            std::cout << "==============================================" << std::endl << std::endl;
        }
    }
    else
    {
        //addContentType(socket); // TODO: Mime if no CGI
        if (ft::isOkHTTP(ret))
            addBodyToResponse(socket);
        else
            addErrorBodyToResponse(socket);
        if (VERBOSE) {
            std::cout << "================== RESPONSE ==================" << std::endl;
            if (secFetchImage(socket))
                std::cout << GREEN << _response[socket] << RESET << std::endl;
            else
                std::cout << GREEN << "We sent an image" << RESET << std::endl;
            std::cout << "==============================================" << std::endl << std::endl;
        }
    }
    return (ret);
}

int Looper::buildResponse(long socket, const Location &loc)
{
    // TODO : Map with func pointers later
    RequestType req_type = ft::RequestFromString(_request[socket].getMethod());

    switch (req_type) {
        case Get:
            buildGetResponse(socket, loc);
            break;
        case Post:
            buildPostResponse(socket, loc);
            break;
        case Delete:
            buildDeleteResponse(socket, loc);
            break;
        default:
            std::cout << RED << _request[socket].getMethod() << " is not a method that the server threats." << RESET << std::endl;
    }
    return (1);
}

int Looper::readFromClient(long socket)
{
    char	        buffer[BUFFER_SIZE];
    int		        ret;
    RequestParser   request;

    ft::bzero(&buffer, BUFFER_SIZE);

	// TODO: Create a loop to read the client request
    ret = recv(socket, buffer, BUFFER_SIZE, 0);
    if (ret > 0) {
        request.parseRequest(buffer);

        if (VERBOSE) {
            std::cout << "================== REQUEST ==================" << std::endl;
            std::cout << BLUE << request << RESET;
            std::cout << "==============================================" << std::endl;
        }

		struct sockaddr_in req_addr;
		socklen_t addr_len = sizeof(req_addr);
		getsockname(socket, (struct sockaddr *)&req_addr, &addr_len);
		// char *addr_str = inet_ntoa(req_addr.sin_addr);
		// std::cout << MAGENTA << "Request adresse: " << RESET << addr_str << std::endl;
		// std::cout << MAGENTA << "Request port: " << RESET << req_addr.sin_port << std::endl;

		const Server &srv = request.FindServer(_servers);
		// TODO TYR: Check if no server corresponds
		const Location &loc = request.FindLocation(srv);

		if(!request.isValid(loc))
			return (-1);

		request.updatePathWithLocation(loc);

        _request.insert(std::make_pair<long, RequestParser>(socket, request));
        buildResponse(socket, loc);
    }
    return (ret);
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
            long ret_val = _active_servers[*it]->send(*it, _response);
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
                close(socket);
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
