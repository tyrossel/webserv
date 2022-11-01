#include "Response.hpp"
#include "CommonGatewayInterface.hpp"
#include "Location.hpp"
#include "Utils.hpp"
#include "webserv.hpp"

/**************************************************************************************/
/*                                  CONSTRUCTOR / DESTRUCTOR                          */
/**************************************************************************************/
Response::Response() : _loc(NULL) {}

Response::Response(const long int socket) : _loc(NULL), _server(NULL), _socket(socket), _status(0) {}

Response::Response(const long int socket, const Location *loc, Server *server, const int status) : _loc(loc), _server(server), _socket(socket)
{
    if (status == 0)
        _status = HTTP_OK;
    else
        _status = status;
}

Response::~Response() {}

/**************************************************************************************/
/*                                  TOOLS                                             */
/**************************************************************************************/

void Response::printLog(bool print_cgi)
{
	if (VERBOSE)
	{
		if (print_cgi)
			std::cout << "==================== CGI ====================" << std::endl;
		else
			std::cout << "================== RESPONSE ==================" << std::endl;
		std::cout << ft::timestamp(TIMESTAMP_FORMAT) << std::endl;
		if (secFetchImage())
			std::cout << GREEN << _response << RESET << std::endl;
		else
			std::cout << GREEN << "We sent an image" << RESET << std::endl;
	}
	else {
        if (getStatus() >= BAD_REQUEST)
            std::cout << RED << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
        else
            std::cout << GREEN << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
	}
}

void Response::addContentLengthPOST(CGI &cgi)
{
    _response.append("Content-Length: ");
    _response.append(ft::to_string(cgi.getRetBody().length()));
    _response.append("\r\n\r\n");
}

void Response::addBodyToResponse()
{
    std::string text;
    std::stringstream out;
    std::string loc = _request.getLocation();
    std::string path = _request.getPath();

    if (ft::isDirectory(loc))
    {
		if (_loc->auto_index)
			text = createDirectoryListingBody(path, loc);
		else
		{
			setStatus(FORBIDDEN);
			addErrorBodyToResponse();
			return ;
		}
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

// Format according to RFC 7231
// Example: Tue, 15 Nov 1994 08:12:31 GMT
void Response::addDate()
{
	std::string timestamp = ft::timestamp("%a, %d %b %Y %T GMT");
    _response.append("Date: " + timestamp + "\r\n");
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

void Response::addHTTPHeader(bool check_path)
{
    if (check_path && _status == HTTP_OK) {
        checkPath(); // functions will return the code catched
    }
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
        _response.append(ft::errorMessage(getStatus()) + "\r\n");
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

void Response::addCGIHeader(CGI &cgi) { _response.append(cgi.getRetHeaders() + "\r\n"); }

/**************************************************************************************/
/*                                  CHECKERS                                          */
/**************************************************************************************/

void Response::checkPath()
{
    std::string path = _request.getLocation();

    if (ft::isDirectory(path) || ft::isFile(path))
        setStatus(HTTP_OK);
    else
        setStatus(NOT_FOUND);
}

bool Response::secFetchImage()
{
    // This function just checks if the _request is for an image
    std::map<std::string, std::string> tmp = _request.getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
    {
        if (tmp.at("Sec-Fetch-Dest") == "image")
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

void Response::buildRedirectionResponse(const Redirection &redir)
{
	if (redir.status == 0)
		setStatus(HTTP_FOUND);
	else
		setStatus(redir.status);
	std::cout << RED "buildRedir: status = " << _status << RESET << std::endl;
    writeResponseHeader();
    addServerHeaderResponse();
    addDate();
	if (ft::isOkHTTP(getStatus()))
	{
		addBodyToResponse();
	}
	else if (_status / 100 == 3)
	{
		std::cout << RED "Redir to FILE" RESET << std::endl;
		_response.append("Location: " + redir.new_url + "\r\n");
		_response.append("Connection: keep-alive\r\n");
		addErrorBodyToResponse();
	}
	else
	{
		std::cout << RED "Redir to ERROR" RESET << std::endl;
		addErrorBodyToResponse();
	}
	if (VERBOSE) {
		std::cout << "================== RESPONSE ==================" << std::endl;
		std::cout << YELLOW << ft::timestamp(TIMESTAMP_FORMAT) << RESET << std::endl;
		std::cout << GREEN << _response << RESET << std::endl;
		std::cout << "==============================================" << std::endl << std::endl;
	}
}

void Response::buildGetResponse(Request req, const Location *loc)
{
    this->_request = req;
    if (useCGI()) // CGI or not ?
    {
        if (ft::isFile(_request.getLocation())) {
            CGI cgi(_request);
            setStatus(cgi.executeCgi(&_request, _server, loc));
            addHTTPHeader();
            addCGIHeader(cgi);
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
        {
            addHTTPHeader();
            addErrorBodyToResponse();
        }
        printLog(true);
    }
    else
    {
        addHTTPHeader();
        //addContentType(socket); // TODO: Mime if no CGI
        if (ft::isOkHTTP(getStatus()))
            addBodyToResponse();
        else
            addErrorBodyToResponse();
        printLog(false);
    }
}

void Response::buildPostResponse(Request req, const Location *loc)
{
    this->_request = req;

    CGI cgi(_request);
    setStatus(cgi.executeCgi(&_request, _server, loc));
    addHTTPHeader();
    addCGIHeader(cgi);
    addContentLengthPOST(cgi);
    _response.append(cgi.getRetBody());
    printLog(true);
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
    printLog(false);
}

void	Response::buildErrorResponse(int errorCode)
{
	//_loc = NULL;
	setStatus(errorCode);
	addHTTPHeader(false);
	addErrorBodyToResponse();
    printLog(false);
}

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/

std::string Response::getResponse() const { return (this->_response); }
int         Response::respSize() const { return ((int)this->_response.size()); }
long int    Response::getSocket() const { return (this->_socket); }
int         Response::getStatus() const { return (this->_status); }
