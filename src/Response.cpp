#include "Response.hpp"
#include "CommonGatewayInterface.hpp"

/**************************************************************************************/
/*                                  CONSTRUCTOR / DESTRUCTOR                          */
/**************************************************************************************/
Response::Response() {}

Response::Response(const long int socket, const Location *loc, Server *server): _loc(loc), _server(server), _socket(socket) {}

Response::~Response() {}
/**************************************************************************************/
/*                                  TOOLS                                             */
/**************************************************************************************/
void Response::addContentLengthPOST()
{
    _response.append("Content-Length: ");
    _response.append(_request.getHeaders().find("Content-Length")->second);
    _response.append("\r\n");
}

void Response::addBodyToResponse() // TODO: add file to read from (std::string path)
{
    std::string text;
    std::stringstream out;
    std::string loc = _request.getLocation();
    std::string path = _request.getPath();

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
            _response.append("\r\n\r\n");
            std::cerr << e.what() << std::endl; // TODO: This is very heavy for a common case..
        }
    }
    out << text.size();
    std::string content_len = "Content-Length: " + out.str();
    _response.append(content_len);
    _response.append("\r\n\r\n");
    _response.append(text);
}

void Response::addDate()
{
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    // convert now to tm struct for UTC
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    _response.append("Date: ");
    _response.append(dt);
}

void Response::addServerHeaderResponse()
{
    _response.append("Server: WetServ/1.0.0\r\n");
}

void Response::addContentType()
{
    _response.append("Content-Type: ");
    std::map<std::string, std::string> tmp = _request.getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
        _response.append(tmp.find("Sec-Fetch-Dest")->second);
    else
        _response.append("NONE");
    _response.append("\r\n");
}

void Response::addHTTPHeader()
{
    checkCode(); // checking if an error code has been parsed in request
    checkPath(); // functions will return the code catched
    writeResponseHeader();
    addServerHeaderResponse();
    addDate();
}

void Response::writeResponseHeader()
{
    std::stringstream out;

    out << getStatus();
    _response.append("HTTP/1.1 ");
    if (ft::isOkHTTP(getStatus())) {
        _response.append(out.str());
        _response.append(" OK\r\n");
    }
    else {
        _response.append(out.str());
        _response.append(" KO\r\n");
    }
}

void Response::addErrorBodyToResponse()
{
    std::string body;
    std::stringstream out;

    if (!_loc)
    {
        body.append(ft::craftErrorHTML(getStatus()));
    }
    else
    {
        try
        {
            body.append(ft::readFile(_loc->error_pages.at(getStatus())));
        }
        catch (const std::exception& e)
        {
            body.append(ft::craftErrorHTML(getStatus()));
        }
    }

    _response.append("Content-Length: ");
    _response.append(ft::itoa(body.size()));
    _response.append("\r\n\r\n");
    _response.append(body);
}

void Response::setStatus(int new_status) { _status = new_status; }

/**************************************************************************************/
/*                                  CHECKERS                                          */
/**************************************************************************************/
void    Response::checkCode()
{
    if (getStatus() == 0)
        setStatus(HTTP_OK);
}

void    Response::checkPath()
{
    std::string path = _request.getLocation();

    if (ft::isDirectory(path) || ft::isFile(path))
        setStatus(HTTP_OK);
    else
        setStatus(NOT_FOUND);
}

bool    Response::secFetchImage()
{
    // This function just checks if the _request is for an image
    std::map<std::string, std::string> tmp = _request.getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
    {
        if (tmp.find("Sec-Fetch-Dest")->second == "image")
            return (false);
    }
    return (true);
}

bool Response::useCGI()
{
	if (_loc->isCGI)
		return true;
	std::string path = _request.getPath();
	size_t slash = path.find_last_of('/');
	if (slash == path.npos)
		slash = 0;
	size_t dot = path.find_last_of('.');
	if (dot == path.npos || dot == path.size() - 1)
		dot = slash;
	std::string path_extension = path.substr(dot + 1);
	for(std::vector<std::string>::const_iterator it = _loc->cgi_extensions.begin();
			it != _loc->cgi_extensions.end(); it++)
	{
		if (path_extension == *it)
		{
			return true;
		}
	}
	return false;
}

/**************************************************************************************/
/*                                  BUILDERS                                          */
/**************************************************************************************/

void Response::buildGetResponse(Request req, const Location *loc)
{
    this->_request = req;

    if (useCGI()) // CGI or not ?
    {
        if (ft::isFile(_request.getLocation())) {
            CGI cgi(_request);
            setStatus(cgi.executeCgi(&_request, _server, loc));
            addHTTPHeader();
            // Here we remove HTTP EOF because the CGI we use cannot accept HTML in it.
            // If we send HTML inside the CGI he will TOUPPER the html which is.. shitty ?
            cgi.removeEOFHTTP();
            _response.append(cgi.getRetBody());
            if (ft::isOkHTTP(getStatus()))
                addBodyToResponse();
            else
                addErrorBodyToResponse();
        }
        else
            addErrorBodyToResponse();
        if (VERBOSE) {
            std::cout << "================== CGI ==================" << std::endl;
            if (secFetchImage())
                std::cout << _response << std::endl;
            else
                std::cout << GREEN << "We sent an image" << RESET << std::endl;
            std::cout << "==============================================" << std::endl << std::endl;
        }
    }
    else
    {
        addHTTPHeader();
        //addContentType(socket); // TODO: Mime if no CGI
        if (ft::isOkHTTP(getStatus()))
            addBodyToResponse();
        else
            addErrorBodyToResponse();
        if (VERBOSE) {
            std::cout << "================== RESPONSE ==================" << std::endl;
            if (secFetchImage())
                std::cout << GREEN << _response << RESET << std::endl;
            else
                std::cout << GREEN << "We sent an image" << RESET << std::endl;
            std::cout << "==============================================" << std::endl << std::endl;
        }
    }
}

void Response::buildPostResponse(Request req, const Location *loc)
{
    this->_request = req;

    CGI cgi(_request);
    setStatus(cgi.executeCgi(&_request, _server, loc));
    addHTTPHeader();
    addContentLengthPOST();
    _response.append(cgi.getRetBody());
    if (VERBOSE) {
        std::cout << "================== CGI ==================" << std::endl;
        if (secFetchImage())
            std::cout << _response << std::endl;
        else
            std::cout << GREEN << "We sent an image" << RESET << std::endl;
        std::cout << "==============================================" << std::endl << std::endl;
    }
}

void Response::buildDeleteResponse(Request req)
{
    this->_request = req;

    std::string path = _request.getLocation();
    if (ft::isFile(path))
    {
        if (remove(path.c_str()) == 0)
            setStatus(HTTP_NO_CONTENT);
        else
            setStatus(FORBIDDEN);
    }
    else
        setStatus(NOT_FOUND);
    addHTTPHeader();
    addContentType();

    if (ft::isOkHTTP(getStatus()))
        addBodyToResponse();
    else
        addErrorBodyToResponse();

    if (VERBOSE) {
        std::cout << "================== RESPONSE ==================" << std::endl;
        if (secFetchImage())
            std::cout << GREEN << _response << RESET << std::endl;
        else
            std::cout << GREEN << "We sent an image" << RESET << std::endl;
        std::cout << "==============================================" << std::endl << std::endl;
    }
}

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/

std::string Response::getResponse() const { return (this->_response); }
int         Response::respSize() const { return ((int)this->_response.size()); }
long int    Response::getSocket() const { return (this->_socket); }
int         Response::getStatus() const { return (this->_status); }
