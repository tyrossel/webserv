#include "ErrorResponse.hpp"
#include "CommonGatewayInterface.hpp"
#include "Location.hpp"
#include "Response.hpp"
#include "Utils.hpp"
#include "webserv.hpp"

/**************************************************************************************/
/*                                  CONSTRUCTOR / DESTRUCTOR                          */
/**************************************************************************************/
ErrorResponse::ErrorResponse(int status, bool close, const Location *loc) :
	Response(status),
	_loc(loc),
    _close(close),
	_print(true)
{
}

ErrorResponse::~ErrorResponse() {}

ErrorResponse::ErrorResponse(const ErrorResponse &rhs) :
	Response(rhs)
{
	*this = rhs;
}

ErrorResponse &ErrorResponse::operator=(const ErrorResponse &rhs)
{
	Response::operator=(rhs);
	_custom_file = rhs._custom_file;
	_close = rhs._close;
	return *this;
}

/**************************************************************************************/
/*                                  BUILDERS                                          */
/**************************************************************************************/

void	ErrorResponse::buildResponse()
{
    std::string body;
	std::string custom_file;
	if (_loc)
	{
		std::map<int, std::string>::const_iterator it = _loc->error_pages.find(getStatus());
		if (it != _loc->error_pages.end())
			custom_file = it->second;
	}

	setHeader("Content-Type", "html");
    if (_close)
		setHeader("Connection", "close");

	try
	{
		body.append(ft::readFile(custom_file));
	}
	catch (const std::exception& e)
	{
		body.append(ft::craftErrorHTML(getStatus()));
	}
	setBody(body);
	if (_print)
		printLog("Error");
}

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/

void	ErrorResponse::printLog(const std::string &title)
{
	if (VERBOSE)
	{
		std::cout << "================== " + title + " ==================" << std::endl;
		std::cout << ft::timestamp(TIMESTAMP_FORMAT) << std::endl;
		std::cout << GREEN << to_string() << RESET << std::endl;
	}
	else
	{
        if (getStatus() >= BAD_REQUEST)
            std::cout << RED << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
        else
            std::cout << GREEN << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
	}
}

void	ErrorResponse::setPrint(bool print)
{
	this->_print = print;
}
