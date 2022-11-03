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
	if (cgi.getRetHeaders().find("Content-Length:") != std::string::npos)
		return ;
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
    std::string extension = ft::getExtension(_req.getPath());
    if (!extension.empty())
    {
        _response.append("Content-Type: ");
        _response.append(ft::getMimeExtension(_mime_map, extension));
        _response.append("\r\n");
    }

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
	std::string path_extension = ft::getExtension(_req.getPath());
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

        addContentType();
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
        _response.append("\r\n");
    else
        AddErrorBodyToResponse();
    printLog("Valid");
}
