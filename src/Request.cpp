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

bool Request::matchLocation(const std::string &path, const std::string &loc_path) const
{
    return !path.compare(0, loc_path.size(), loc_path);
}

const Location & Request::FindLocation(const Server &server) const
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
    return loc_map.at(best_loc_index);
}

const Server & Request::FindServer(const std::vector<Server> &servers, struct sockaddr_in req_addr) const
{
    // Splitting of Host header into host and port:
    std::string hostHeader = _headers.at("Host");

    size_t	colon = hostHeader.find_first_of(':');
    std::string requestHost = hostHeader.substr(0, colon);
    int requestPort = ntohs(req_addr.sin_port);
	const Server *firstServ = NULL;

    for(std::vector<Server>::const_iterator it_srv = servers.begin(); it_srv !=
                                                                      servers.end(); it_srv++)
    {
        if (requestPort != it_srv->getPort())
            continue;
		if (it_srv->getAddress() != "0.0.0.0" && inet_addr(it_srv->getAddress().c_str()) != req_addr.sin_addr.s_addr)
			continue;
		if (!firstServ)
			firstServ = &(*it_srv);

        const std::vector<std::string> &server_names = it_srv->getName();
        if (server_names.empty())
            return *it_srv;

        for (std::vector<std::string>::const_iterator it_names = server_names.begin();
             it_names != server_names.end(); it_names++)
        {
            if (*it_names == requestHost)
                return *it_srv;
        }
    }
    return *firstServ;
}

bool Request::isValid(const Location &loc)
{
    if (!loc.requests_allowed[_method])
    {
        setStatus(METHOD_NOT_ALLOWED);
        return false;
    }

    if (loc.max_client_body_size != 0 && _body_length > loc.max_client_body_size)
    {
        setStatus(PAYLOAD_TOO_LARGE);
        return false;
    }

	setStatus(HTTP_OK);
    return true;
}

void Request::updatePathWithLocation(const Location &loc)
{
    char *tmp = getcwd(NULL, 0);
    if (!tmp)
        return ;
    std::string cwd = tmp;
    _location = _path.substr(loc.path.length());
    if (_location[0] != '/')
        _location = '/' + _location;
    _location = loc.root_dir + _location;
    _location = (_location[0] == '/') ? _location : (cwd + '/' + _location);
    free(tmp);

    if (loc.isCGI)
        return ;

    if (!ft::isDirectory(_location))
        return ;

	// Automatic redirect to index
    std::vector<std::string>::const_iterator it = loc.indexes.begin();
	for (;it != loc.indexes.end(); it++)
	{
		std::string new_location = _location + *it;
		std::ifstream test_stream(new_location.c_str());
		if (test_stream.is_open())
		{
			_location = new_location;
			break ;
		}
		test_stream.close();
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
long                                Request::getBodyLength() const { return (this->_body_length); }
int                                 Request::getStatus() const { return (this->_status); }

/**************************************************************************************/
/*                                      SETTERS                                       */
/**************************************************************************************/

void Request::setStatus(int new_status) { this->_status = new_status; }
void Request::setPath(std::string new_path) { this->_path = new_path; }

/**************************************************************************************/
/*                                      NON MEMBERS                                   */
/**************************************************************************************/

std::ostream &operator<<(std::ostream &out, const Request &rhs)
{
    out << ft::RequestToString(rhs.getMethod()) << ' ' << rhs.getPath() << ' ' << rhs.getVersion() << "\r\n";

    std::map<std::string, std::string> headers = rhs.getHeaders();
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
        out << it->first << ": " << it->second << "\r\n";
    }

	if (rhs.getBody().size() > 0 && headers.find("Sec-Fetch-Dest") != headers.end()
		&& (headers.at("Sec-Fetch-Dest") == "image" || headers.at("Sec-Fetch-Dest") == "document"))
	{
		out << "\r\n<Skipped image/document body of length " << rhs.getBody().size() << ">" << std::endl;
	}
	else if (!(rhs.getBody().empty()))
        out << "\r\n" << rhs.getBody() << std::endl;

    return out;
}
