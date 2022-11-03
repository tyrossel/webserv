#include "ErrorResponse.hpp"
#include "CommonGatewayInterface.hpp"
#include "Location.hpp"
#include "Response.hpp"
#include "Utils.hpp"
#include "webserv.hpp"

/**************************************************************************************/
/*                                  CONSTRUCTOR / DESTRUCTOR                          */
/**************************************************************************************/
ErrorResponse::ErrorResponse(int status, bool close, const std::string &custom_page) :
	Response(status),
	_custom_file(custom_page),
    _close(close)
{
	buildResponse();
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

std::string	ErrorResponse::buildResponse()
{
    std::string body;

	setHeader("Content-Type", "html");
    if (_close)
		setHeader("Connection", "close");

	try
	{
		body.append(ft::readFile(_custom_file));
	}
	catch (const std::exception& e)
	{
		body.append(ft::craftErrorHTML(getStatus()));
	}
	setBody(body);
    printLog("Error");
	return to_string();
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
