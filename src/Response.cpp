#include "Response.hpp"
#include "CommonGatewayInterface.hpp"
#include "Location.hpp"
#include "Utils.hpp"
#include "webserv.hpp"

/**************************************************************************************/
/*                                  CONSTRUCTOR / DESTRUCTOR                          */
/**************************************************************************************/
Response::Response(int status)
{
	setStatus(status);
}

Response::~Response() {}

Response::Response(const Response &rhs)
{
	*this = rhs;
}

Response &Response::operator=(const Response &rhs)
{
	_status = rhs._status;
	_response = rhs._response;
	return *this;
}
/**************************************************************************************/
/*                                  TOOLS                                             */
/**************************************************************************************/

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

void Response::addHTTPHeader()
{
    writeResponseHeader();
    addServerHeaderResponse();
    addDate();
}

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/

std::string Response::getResponse() const { return (this->_response); }
int         Response::respSize() const { return ((int)this->_response.size()); }
int         Response::getStatus() const { return (this->_status); }
void		Response::setStatus(int new_status)
{
	if (new_status >= 100 && new_status < 600)
		_status = new_status;
	else
		_status = 500;
}
