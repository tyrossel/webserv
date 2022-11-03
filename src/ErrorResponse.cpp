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
{}

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
	return *this;
}

/**************************************************************************************/
/*                                  TOOLS                                             */
/**************************************************************************************/

void ErrorResponse::addBodyToResponse()
{
    std::string body;
    std::stringstream out;

	try
	{
		body.append(ft::readFile(_custom_file));
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

/**************************************************************************************/
/*                                  BUILDERS                                          */
/**************************************************************************************/

std::string	ErrorResponse::buildResponse()
{
	addHTTPHeader();
    _response.append("Content-Type: html\r\n");
    if (_close)
        _response.append("Connection: close\r\n");
	addBodyToResponse();
    printLog("Error");
	return _response;
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
		std::cout << GREEN << getResponse() << RESET << std::endl;
	}
	else
	{
        if (getStatus() >= BAD_REQUEST)
            std::cout << RED << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
        else
            std::cout << GREEN << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
	}
}
