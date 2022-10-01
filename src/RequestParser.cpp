#include "RequestParser.hpp"

RequestParser::RequestParser() : _method(""), _url(""), _version(""), _headers(), _body(""), _return(0) {}

RequestParser::RequestParser(const RequestParser &other) : _method(other._method), _url(other._url), _version(other._version), _headers(other._headers), _body(other._body), _return(other._return) {}

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
        this->_return = other._return;
    }
    return (*this);
}

std::string RequestParser::getNextLine(std::string &str, size_t &i)
{
    (void)str;
    (void)i;
    return NULL;
}

int RequestParser::parseVersion(std::string &full_line, size_t &start, size_t &end)
{
    /* Stock the next \n index in end, and check if no \n are found */
    start = full_line.find_first_not_of(' ', end);
    if (start == std::string::npos)
    {
        std::cout << "Error Not Found : There is no newline after version HTTP" << std::endl;
        this->_return = 404;
        return (-1);
    }

    std::string format = full_line.substr(start, 5);
    if (format != "HTTP/")
    {
        std::cout << "Wrong HTTP format" << std::endl;
        this->_return = 404;
        return (-1);
    }

    this->_version = full_line.substr(start + 5, 3);
    if (this->_version != "1.0" && this->_version != "1.1")
    {
        std::cout << "Wrong HTTP version" << std::endl;
        this->_return = 404;
        return (-1);
    }

    return 0;
}

int RequestParser::parseUrl(std::string &full_line, size_t &start, size_t &end)
{
    /* Stock the next spaces index in end, and check if no spaces are found */
    start = full_line.find_first_not_of(' ', end);
    if (start == std::string::npos)
    {
        std::cout << "Error Not Found : There is no spaces after URL" << std::endl;
        this->_return = 404;
        return (-1);
    }
    end = full_line.find_first_of(' ', start);
    this->_url = full_line.substr(start, end - start);

    return parseVersion(full_line, start, end);
}

int RequestParser::parseFirstLine(std::string &str)
{
    size_t      start = 0;
    size_t      end = 0;
    std::string full_line;

    /* Extract the first line */
    end = str.find_first_of('\n');
    full_line = str.substr(start, end);

    /* Get the spaces after method */
    end = full_line.find_first_of(' ');

    /* If there is no spaces after method, end will be equal to npos */
    if (end == std::string::npos)
    {
        std::cout << "Error Not Found : There is no spaces after method" << std::endl;
        this->_return = 404;
        return (-1);
    }
    this->_method = full_line.substr(start, end);

    return parseUrl(full_line, start, end);
}

int    RequestParser::parseRequest(const char *str)
{
    if (!str)
        return -1;
    std::string request(str);

    this->parseFirstLine(request);

    return 0;
}

std::string                         RequestParser::getMethod() { return (this->_method); }
std::string                         RequestParser::getVersion() { return (this->_version); }
std::string                         RequestParser::getBody() { return (this->_body); }
std::map<std::string, std::string>  RequestParser::getHeaders() { return (this->_headers); }
