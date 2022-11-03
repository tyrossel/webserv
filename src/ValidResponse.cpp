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
	buildResponse();
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
		if (getHeader("Content-Type").substr(0, 5) == "image")
		{
			std::cout << GREEN;
			std::cout << headersToString() << std::endl;
			std::cout << std::endl << "Image body skipped" << RESET << std::endl;
		}
		else
			std::cout << GREEN << to_string() << RESET << std::endl;
	}
	else {
        if (getStatus() >= BAD_REQUEST)
            std::cout << RED << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
        else
            std::cout << GREEN << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
	}
}

void ValidResponse::buildGetBody()
{
    std::string text;
    std::string loc = _req.getLocation();
    std::string path = _req.getPath();

    if (ft::isDirectory(loc))
    {
		if (_loc.auto_index)
			text = createDirectoryListingBody(path, loc);
		else
		{
			return setError(FORBIDDEN);
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
			return setError(FORBIDDEN);
        }
    }
	setBody(text);
}

void ValidResponse::setContentType()
{
    std::string extension = ft::getExtension(_req.getPath());
    if (!extension.empty())
        setHeader("Content-Type", ft::getMimeExtension(_mime_map, extension));
}

void	ValidResponse::setError(int status)
{
	std::map<int, std::string>::const_iterator it = _loc.error_pages.find(status);
	std::string custom_page;

	if (it == _loc.error_pages.end())
		custom_page = it->second;

	ErrorResponse err(status, false, custom_page);
	setBody(err.getBody());
	setStatus(status);
}


/**************************************************************************************/
/*                                  CHECKERS                                          */
/**************************************************************************************/

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
			setError(NOT_IMPLEMENTED);
			break;
    }
	return to_string();
}

void ValidResponse::buildGetResponse()
{
    if (useCGI())
    {
        if (ft::isFile(_req.getLocation())) {
            CGI cgi(_req);
            setStatus(cgi.executeCgi(&_req, _server, _loc));
            setBody(cgi.getRetBody());
        }
        else
			setError(NOT_FOUND);
        printLog("CGI Get");
    }
    else
    {
		if (!ft::isDirectory(_req.getLocation()) && !ft::isFile(_req.getLocation()))
			return setError(NOT_FOUND);

        setContentType();
        if (ft::isOkHTTP(getStatus()))
            buildGetBody();
        else
			setError(getStatus());
        printLog("Valid Get");
    }
}

// TODO: Check status before replying ?
void ValidResponse::buildPostResponse()
{
    if (useCGI()) {
        CGI cgi(_req);

        setStatus(cgi.executeCgi(&_req, _server, _loc));
		setBody(cgi.getRetBody());
        printLog("CGI Post");
    }
    else {
        if (!ft::isFile(_req.getLocation()))
            setError(NOT_FOUND);
        else if (ft::writeFile(_req.getLocation(), _req.getBody()) == -1)
            setError(FORBIDDEN);

        printLog("Valid Post");
    }
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

    setContentType();

    printLog("Valid Delete");
}
