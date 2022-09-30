#include "RequestParser.hpp"

RequestParser::RequestParser() : _method(), _url(), _version(), _headers(), _body() {}

RequestParser::RequestParser(const RequestParser &other) : _method(other._method), _url(), _version(other._version), _headers(other._headers), _body(other._body) {}

RequestParser::~RequestParser() {}

RequestParser &RequestParser::operator=(const RequestParser &other)
{
    if (this != &other)
    {
        this->_method = other._method;
        this->_url = other._url;
        this->_version = other._version;
        this->_headers = other._headers;
        this->_body = other._body;
    }
    return (*this);
}

std::string                         RequestParser::getNextLine(std::string &str, size_t &i)
{

}

void                                RequestParser::parseRequest(const char *str)
{
    if (!str)
        return;
    std::string request = str;

    int start, end = 0;
    end = request.find(" ");
    _method = request.substr(0, end);

}

std::string                         RequestParser::getMethod() { return (this->_method); }
std::string                         RequestParser::getVersion() { return (this->_version); }
std::string                         RequestParser::getBody() { return (this->_body); }
std::map<std::string, std::string>  RequestParser::getHeaders() { return (this->_headers); }
