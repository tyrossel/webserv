#include "ValidResponse.hpp"
#include "CommonGatewayInterface.hpp"
#include "ErrorResponse.hpp"
#include "Location.hpp"
#include "Utils.hpp"
#include "webserv.hpp"

/**************************************************************************************/
/*                                  CONSTRUCTOR / DESTRUCTOR                          */
/**************************************************************************************/
ValidResponse::ValidResponse(const Location &loc, const Server &server, const Request &req)
	: Response(req.getStatus()), _loc(loc), _server(server), _req(req)
{
}

ValidResponse::ValidResponse(const ValidResponse &rhs) :
	Response(rhs),
	_loc(rhs._loc),
	_server(rhs._server),
	_req(rhs._req)
{
}

ValidResponse::~ValidResponse() {}

/**************************************************************************************/
/*                                  TOOLS                                             */
/**************************************************************************************/

void ValidResponse::printLog(const std::string &title)
{
	if (VERBOSE)
	{
		std::cout << "==================== " + title + " ====================" << std::endl;
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

void ValidResponse::addContentLengthCGI(CGI &cgi)
{
	// TODO : Check if already present ?
    _response.append("Content-Length: ");
    _response.append(ft::to_string(cgi.getRetBody().length()));
    _response.append("\r\n\r\n");
}

void ValidResponse::addBodyToResponse()
{
    std::string text;
    std::stringstream out;
    std::string loc = _req.getLocation();
    std::string path = _req.getPath();

    if (ft::isDirectory(loc))
    {
		if (_loc.auto_index)
			text = createDirectoryListingBody(path, loc);
		else
		{
			setStatus(FORBIDDEN);
			AddErrorBodyToResponse();
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
            std::cerr << e.what() << std::endl;
			ErrorResponse err(FORBIDDEN);
			_response = err.buildResponse();
			return ;
        }
    }
    out << text.size();
    std::string content_len = "Content-Length: " + out.str();
    _response.append(content_len);
    _response.append("\r\n\r\n");
    _response.append(text);
}

void ValidResponse::AddErrorBodyToResponse()
{
    std::string body;
    std::stringstream out;

	try
	{
		body.append(ft::readFile(_loc.error_pages.at(getStatus())));
	}
	catch (const std::exception& e)
	{
		body.append(ft::craftErrorHTML(getStatus()));
	}

    _response.append("Content-Length: ");
    _response.append(ft::itoa(body.size()));
    _response.append("\r\n\r\n");
    _response.append(body);
}

void ValidResponse::addContentType()
{
    _response.append("Content-Type: ");
    std::map<std::string, std::string> tmp = _req.getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
        _response.append(tmp.find("Sec-Fetch-Dest")->second);
    else
        _response.append("NONE");
    _response.append("\r\n");
}

void ValidResponse::addHTTPHeader(bool check_path)
{
    if (check_path && getStatus() == HTTP_OK) {
        checkPath(); // functions will return the code catched
    }
	Response::addHTTPHeader();
}

/**************************************************************************************/
/*                                  CHECKERS                                          */
/**************************************************************************************/

void ValidResponse::checkPath()
{
    std::string path = _req.getLocation();

    if (ft::isDirectory(path) || ft::isFile(path))
        setStatus(HTTP_OK);
    else
        setStatus(NOT_FOUND);
}

bool ValidResponse::secFetchImage()
{
    // This function just checks if the _request is for an image
    std::map<std::string, std::string> tmp = _req.getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
    {
        if (tmp.at("Sec-Fetch-Dest") == "image")
            return (false);
    }
    return (true);
}

bool ValidResponse::useCGI()
{
	if (_loc.isCGI)
		return true;
	std::string path = _req.getPath();
	size_t slash = path.find_last_of('/');
	if (slash == path.npos)
		slash = 0;
	size_t dot = path.find_last_of('.');
	if (dot == path.npos || dot == path.size() - 1)
		return false;
	std::string path_extension = path.substr(dot + 1);
	for(std::vector<std::string>::const_iterator it = _loc.cgi_extensions.begin();
			it != _loc.cgi_extensions.end(); it++)
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

std::string ValidResponse::buildResponse()
{
    switch (_req.getMethod())
	{
        case Get:
            buildGetResponse();
            break;
        case Post:
            buildPostResponse();
            break;
        case Delete:
            buildDeleteResponse();
            break;
        default:
			{
				ErrorResponse err(NOT_IMPLEMENTED);
				_response = err.buildResponse();
				break;
			}
    }
	return _response;
}

void ValidResponse::buildRedirectionResponse(const Redirection &redir)
{
	if (redir.status == 0)
		setStatus(HTTP_FOUND);
	else
		setStatus(redir.status);
	std::cout << RED "buildRedir: status = " << getStatus() << RESET << std::endl;
	Response::addHTTPHeader();
	if (ft::isOkHTTP(getStatus()))
	{
		addBodyToResponse();
	}
	else if (getStatus() / 100 == 3)
	{
		std::cout << RED "Redir to FILE" RESET << std::endl;
		_response.append("Location: " + redir.new_url + "\r\n");
		_response.append("Connection: keep-alive\r\n");
		AddErrorBodyToResponse();
	}
	else
	{
		std::cout << RED "Redir to ERROR" RESET << std::endl;
		AddErrorBodyToResponse();
	}
	if (VERBOSE)
	{
		std::cout << "================== RESPONSE ==================" << std::endl;
		std::cout << YELLOW << ft::timestamp(TIMESTAMP_FORMAT) << RESET << std::endl;
		std::cout << GREEN << _response << RESET << std::endl;
		std::cout << "==============================================" << std::endl << std::endl;
	}
}

void ValidResponse::buildGetResponse()
{
    if (useCGI()) // CGI or not ?
    {
        if (ft::isFile(_req.getLocation())) {
            CGI cgi(_req);
            setStatus(cgi.executeCgi(&_req, _server, _loc));
            addHTTPHeader();
			addContentLengthCGI(cgi);
            _response.append(cgi.getRetBody());
        }
        else
        {
            addHTTPHeader();
            AddErrorBodyToResponse();
        }
        printLog("CGI");
    }
    else
    {
        addHTTPHeader();
        //addContentType(socket); // TODO: Mime if no CGI
        if (ft::isOkHTTP(getStatus()))
            addBodyToResponse();
        else
            AddErrorBodyToResponse();
        printLog("Valid2");
    }
}

// TODO: Check status before replying ?
void ValidResponse::buildPostResponse()
{
    CGI cgi(_req);
    setStatus(cgi.executeCgi(&_req, _server, _loc));
    addHTTPHeader();
    addContentLengthCGI(cgi);
    _response.append(cgi.getRetBody());
    printLog("CGI");
}

void ValidResponse::buildDeleteResponse()
{
    std::string path = _req.getLocation();
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
        AddErrorBodyToResponse();
    printLog("Valid");
}
