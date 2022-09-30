#include "RequestParser.hpp"

RequestParser::RequestParser() : _method(), _version(), _headers(), _body() {}

RequestParser::RequestParser(const RequestParser &other) : _method(other._method), _version(other._version), _headers(other._headers), _body(other._body) {}

RequestParser::~RequestParser() {}

RequestParser &RequestParser::operator=(const RequestParser &other)
{
    if (this != &other)
    {
        this->_method = other._method;
        this->_version = other._version;
        this->_headers = other._headers;
        this->_body = other._body;
    }
    return (*this);
}

void                                RequestParser::parseRequest(const char *str)
{
    if (!str)
        return;
    std::string request = str;


}

std::string                         RequestParser::getMethod() { return (this->_method); }
std::string                         RequestParser::getVersion() { return (this->_version); }
std::string                         RequestParser::getBody() { return (this->_body); }
std::map<std::string, std::string>  RequestParser::getHeaders() { return (this->_headers); }
