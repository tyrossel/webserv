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

	ErrorResponse err(status, false, &_loc);
	err.buildResponse();
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
