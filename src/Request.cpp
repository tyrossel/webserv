#include "Request.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

Request::Request() : _method(), _path(""), _query(""), _version(""), _headers(), _body(""), _body_length(0), _status(0) {}

Request::Request(const Request &other)
{
    *this = other;
}

Request::~Request() {}

Request &Request::operator=(const Request &other)
{
    if (this != &other)
    {
        this->_method = other._method;
        this->_path = other._path;
        this->_location = other._location;
        this->_query = other._query;
        this->_version = other._version;
        this->_headers = other._headers;
        this->_body = other._body;
        this->_body_length = other._body_length;
        this->_status = other._status;
    }
    return (*this);
}

/**************************************************************************************/
/*                                      MEMBERS                                       */
/**************************************************************************************/

bool matchLocation(const std::string &path, const std::string &loc_path)
{
    // TODO TYR: allow other matching, such as those defined in Nginx:
    // = (has to be totally equal)
    // ~ (case-sensitive)
    // ~* case-insensitive
    return !path.compare(0, loc_path.size(), loc_path);
}

const Location * Request::FindLocation(const Server &server) const
{
    const std::map<std::string, Location> & loc_map = server.getLocations();
    std::string best_loc_index = "/";
    for (std::map<std::string, Location>::const_iterator it = loc_map.begin();
         it != loc_map.end(); it++)
    {
        if (matchLocation(_path, it->first)
            && it->first.size() > best_loc_index.size())
            best_loc_index = it->first;
    }
    return &loc_map.at(best_loc_index);
}

const Server * Request::FindServer(const std::vector<Server> &servers) const
{
    // Splitting of Host header into host and port:
    std::string hostHeader = _headers.at("Host");

    size_t	colon = hostHeader.find_first_of(':');

    std::string requestHost = hostHeader.substr(0, colon);
    std::string requestPortStr = (colon != std::string::npos ? hostHeader.substr(colon + 1) : "80");
    int requestPort = 80;
    if (colon != std::string::npos && colon != hostHeader.size() - 1)
        requestPort = ft::stoi(requestPortStr);
    std::cout << MAGENTA << "Request host=" << requestHost << ", port=" << requestPort << RESET << std::endl;

    for(std::vector<Server>::const_iterator it_srv = servers.begin(); it_srv !=
                                                                      servers.end(); it_srv++)
    {
        // TODO: Check request adress
        if (requestPort != it_srv->getPort())
            continue;

        const std::vector<std::string> &server_names = it_srv->getName();
        if (server_names.empty())
            return &(*it_srv);

        for (std::vector<std::string>::const_iterator it_names = server_names.begin();
             it_names != server_names.end(); it_names++)
        {
            // TODO: Handle wildcards
            if (*it_names == requestHost)
                return &(*it_srv);
        }
    }
    return NULL;
}

bool	Request::isValid(const Location *loc) const
{
    if (!loc)
        return false;

    if (!loc->isRequestAllowed(_method))
    {
        std::cerr << RED << "Method " << ft::RequestToString(_method) << " not allowed in location " << loc->path << RESET << std::endl;
        return false;
    }

    if (loc->max_client_body_size != 0 && _body_length > loc->max_client_body_size)
    {
        std::cerr << RED << "Client body size too big for location " << loc->path << RESET << std::endl;
        return false;
    }

    return true;
}

void	Request::updatePathWithLocation(const Location *loc)
{
    if (!loc)
        return ;

    _location = loc->root_dir + "/" + _path.substr(loc->path.length());

    if (loc->isCGI)
        return ;

    if (!ft::isDirectory(_location))
        return ;

    if (loc->auto_index)
    {
        std::vector<std::string>::const_iterator it = loc->indexes.begin();
        for (;it != loc->indexes.end(); it++)
        {
            std::string new_location = _location + *it;
            std::ifstream test_stream(new_location.c_str());
            if (test_stream.is_open())
            {
                _location = new_location;
                return ;
            }
            test_stream.close();
        }
        // TODO : Handle case if index is not found
    }
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::map<std::string, std::string>  Request::getHeaders() const { return (this->_headers); }
RequestType                         Request::getMethod() const { return (this->_method); }
std::string                         Request::getPath() const { return (this->_path); }
std::string                         Request::getLocation() const { return (this->_location); }
std::string                         Request::getQuery() const { return (this->_query); }
std::string                         Request::getVersion() const { return (this->_version); }
std::string                         Request::getBody() const { return (this->_body); }
int                                 Request::getBodyLength() const { return (this->_body_length); }
int                                 Request::getStatus() const { return (this->_status); }

/**************************************************************************************/
/*                                      SETTERS                                       */
/**************************************************************************************/

void Request::setStatus(int new_status) { this->_status = new_status; }


/**************************************************************************************/
/*                                      NON MEMBERS                                   */
/**************************************************************************************/

std::ostream &operator<<(std::ostream &out, const Request &rhs)
{
    out << "HTTP CLIENT REQUEST : \n"
        << ft::RequestToString(rhs.getMethod()) << ' ' << rhs.getPath() << ' ' << rhs.getVersion() << "\r\n";

    std::map<std::string, std::string> headers = rhs.getHeaders();
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        out << it->first << ": " << it->second << "\r\n";

    out << "\r\n" << rhs.getBody() << std::endl;

    return out;
}
