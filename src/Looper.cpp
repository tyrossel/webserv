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

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
    size_t i;
    unsigned char *p;
    unsigned char *q;

    i = 0;
    p = (unsigned char *) dst;
    q = (unsigned char *) src;
    while (i < n) {
        p[i] = q[i];
        i++;
    }
    return (dst);

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
        std::cerr << "Could not setup cluster !" << std::endl;
        return (-1);
    }
    else
        return (0);
}

/**************************************************************************************/
/*                                  RESPONSE CRAFTING                                 */
/**************************************************************************************/
void Looper::addStaticBodyResponse(std::string &str)
{
    str += "HTTP/1.1 200 OK\n";
    str += "Server: WetServ/1.0.0\n";
    str += "Transfer-Encoding: identity\n";
    str += "Content-Type: text/html\n";
}

void Looper::addDate(std::string &str)
{
    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    char* dt = ctime(&now);

    std::cout << "The local date and time is: " << dt << std::endl;

    // convert now to tm struct for UTC
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    str += "Date: ";
    str += dt;
}

void Looper::addBodyToResponse(std::string &str)
{
    int i = 0;
    std::stringstream out;
    std::ifstream fs("src/html/index.html");
    //std::ifstream fs(_filename.c_str()); TODO: put path recieved

    if (!fs.good())
    {
        std::cerr << "Error stream file not found" << std::endl;
        return ;
    }
    std::string text;
    text.assign(std::istreambuf_iterator<char>(fs),
                std::istreambuf_iterator<char>());
    fs.close();
    str += "Content-Length: ";
    i = text.size();
    out << i;
    str += out.str();
    str += "\n\n";
    str += text;
}

int Looper::buildResponse(long socket, RequestParser request)
{
    (void)request;
    std::string str;

    _response.insert(std::make_pair<long int, std::string>(socket, ""));
    //addHTTPHeader(str);
    addStaticBodyResponse(str);
    addDate(str);
    addBodyToResponse(str);
    _response[socket] += str;
    std::cout << _response[socket] << std::endl;
    return (1);
}

int Looper::readFromClient(long socket)
{
    char	        buffer[BUFFER_SIZE];
    int		        ret;
    RequestParser   request;

    //TODO : Store request in Looper object
    ret = recv(socket, buffer, BUFFER_SIZE, 0);
    request.parseRequest(buffer);
    buildResponse(socket, request);
    return (ret);
}

/**************************************************************************************/
/*                                  LOOP RELATED                                      */
/**************************************************************************************/
void Looper::loop()
{
    while (1)
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
            ft_memcpy(&reading_fd_set, &_active_fd_set, sizeof(_active_fd_set));
            FD_ZERO(&writing_fd_set);
            // here we set the already active fd's in the writing fd's
            for (std::vector<int>::iterator it = _ready_fd.begin(); it != _ready_fd.end(); it++)
                FD_SET(*it, &writing_fd_set);
            // select will wait for an event on the set given
            ret = select(_max_fd + 1, &reading_fd_set, &writing_fd_set, NULL, &timeout);
        }
        // ret will be greater than 0 if any valid event is catched
        if (ret > 0)
        {
            // Prepare and send the response to the client
            sendResponse(reading_fd_set, writing_fd_set, _active_fd_set);
            // Processing the request received. Reading and parsing the request to prepare the response
            requestProcess(reading_fd_set);
            // Catching the new incoming communications to prepare the channel
            catchCommunication(reading_fd_set, ret);
        }
        else
            selectErrorHandle();
    }
}

void Looper::sendResponse(fd_set &reading_fd_set, fd_set &writing_fd_set, fd_set &_active_fd_set)
{
    for (std::vector<int>::iterator it = _ready_fd.begin(); it != _ready_fd.end(); it++)
    {
        if (FD_ISSET(*it, &writing_fd_set))
        {
            long ret_val = _active_servers[*it]->send(*it, _response);
            if (ret_val == 0)
            {
                _response.erase(*it); // erase the response from map when comm is over
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
    for (std::map<long, Server *>::iterator it = _active_servers.begin(); it != _active_servers.end(); it++)
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
            else if (ret_val == -1)
            {
                // in case of error, we clear as done previously
                FD_CLR(socket, &_active_fd_set);
                FD_CLR(socket, &reading_fd_set);
                _active_servers.erase(socket);
                it = _active_servers.begin();
            }
            ret_val = 0;
            break;
        }
    }
}

void Looper::catchCommunication(fd_set &reading_fd_set, int ret)
{
    for (std::vector<Server>::iterator it = _servers.begin(); ret && it != _servers.end(); it++)
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
                _active_servers.insert(std::make_pair<long int, Server *>(socket, &(*it)));
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
    std::cout << "Select had an issue !" << std::cout;
    for (std::map<long, Server *>::iterator it = _active_servers.begin(); it != _active_servers.end(); it++)
        it->second->close(it->first);
    _active_servers.clear();
    _ready_fd.clear();
    FD_ZERO(&_active_fd_set);
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        FD_SET((*it).getFd(), &_active_fd_set);
}
