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
    _mime_map = ft::createMimeMap();
}

Response::~Response() {}

Response::Response(const Response &rhs)
{
	*this = rhs;
}

Response &Response::operator=(const Response &rhs)
{
	_status = rhs._status;
	return *this;
}

/**************************************************************************************/
/*                                  TOOLS                                             */
/**************************************************************************************/

void Response::writeResponseHeader(std::string &txt) const
{
    std::stringstream out;

    out << getStatus();
    txt.append("HTTP/1.1 ");
    if (ft::isOkHTTP(getStatus())) {
        txt.append(out.str());
        txt.append(" OK\r\n");
    }
    else {
        txt.append(out.str());
        txt.append(ft::errorMessage(getStatus()) + "\r\n");
    }
}


std::string		Response::headersToString() const
{
	std::string txt;
	std::string timestamp = ft::timestamp("%a, %d %b %Y %T GMT");

	writeResponseHeader(txt);
    txt.append("Server: WetServ/1.0.0\r\n");
    txt.append("Date: " + timestamp + "\r\n");

	for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
			it != _headers.end(); it++)
	{
		txt.append(it->first + ": " + it->second + "\r\n");
	}
    std::stringstream out;
    out << _body.size();
    std::string content_len = "Content-Length: " + out.str();
    txt.append(content_len);
	return txt;
}

std::string Response::to_string() const
{
	std::string txt;

	txt.append(headersToString());
	txt.append("\r\n\r\n");
	txt.append(_body);
	return txt;
}

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/
void	Response::setHeader(const std::string &key, const std::string &value)
{
	this->_headers[key] = value;
}

std::string	Response::getHeader(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find(key);
	if (it == _headers.end())
		return "";
	return it->second;
}

std::string	Response::getBody() const
{
	return _body;
}

void	Response::setBody(const std::string &str)
{
	this->_body = str;
}

int         Response::getStatus() const { return (this->_status); }
void		Response::setStatus(int new_status)
{
	if (new_status >= 100 && new_status < 600)
		_status = new_status;
	else
		_status = 500;
}
