#include "RequestParser.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

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

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

int RequestParser::checkMethod(std::string &method)
{
    // Rest Connect, Options, Trace and Patch

    if (method == "GET" || method == "HEAD"
        || method == "POST" || method == "PUT"
        || method == "DELETE")
        return 0;
    return -1;
}

std::string RequestParser::getNextLine(std::string &str, size_t &start)
{
    std::string line;
    size_t      end;

    end = str.find_first_of('\n', start);
    line = str.substr(start, end - start);
    start = end + 1;

    return line;
}

int RequestParser::parseVersion(std::string &first_line, size_t &start, size_t &end)
{
    /* Stock the next ' ' index in end, and check if no ' ' are found */
    start = first_line.find_first_not_of(' ', end);
    if (start == std::string::npos)
    {
        std::cout << "Error Not Found : There is no newline after version HTTP" << std::endl;
        this->_return = BAD_REQUEST;
        return (-1);
    }

    /* Get the 5 next characters. Expect that it will be HTTP/ */
    std::string format = first_line.substr(start, 5);
    if (format != "HTTP/")
    {
        std::cout << "Wrong HTTP format" << std::endl;
        this->_return = BAD_REQUEST;
        return (-1);
    }

    /* Extract version */
    this->_version = first_line.substr(start + 5, 3);
    if (this->_version != "1.0" && this->_version != "1.1")
    {
        std::cout << "Wrong HTTP version" << std::endl;
        this->_return = BAD_REQUEST;
        return (-1);
    }

    return 0;
}

int RequestParser::parseUrl(std::string &first_line, size_t &start, size_t &end)
{
    /* Stock the next spaces index in end, and check if no spaces are found */
    start = first_line.find_first_not_of(' ', end);
    if (start == std::string::npos)
    {
        std::cout << "Error Not Found : There is no spaces after URL" << std::endl;
        this->_return = BAD_REQUEST;
        return (-1);
    }
    end = first_line.find_first_of(' ', start);
    this->_url = first_line.substr(start, end - start);

    return parseVersion(first_line, start, end);
}

int RequestParser::parseFirstLine(std::string &first_line)
{
    size_t      start = 0;
    size_t      end = 0;

    /* Get the spaces after method */
    end = first_line.find_first_of(' ');

    /* If there is no spaces after method, end will be equal to npos */
    if (end == std::string::npos)
    {
        std::cout << "Error Not Found : There is no spaces after method" << std::endl;
        this->_return = BAD_REQUEST; //  server cannot or will not process the request due to something that is perceived to be a client error
        return (-1);
    }
    this->_method = first_line.substr(start, end);
    if (checkMethod(this->_method) == -1)
        return -1;

    return parseUrl(first_line, start, end);
}

int RequestParser::parseRequest(const char *str)
{
    if (!str)
    {
        std::cerr << "Wrong buffer send by recv" << std::endl;
        return -1;
    }

    size_t      index = 0;
    size_t      trunc = 0;

    std::string request(str);
    std::string line, key;

    line = getNextLine(request, index);
    if (this->parseFirstLine(line) == -1)
        return -1;

    while((line = getNextLine(request, index)) != "\r" && line != "")
    {
        trunc = line.find_first_of(":");
        key = line.substr(0, trunc);

        this->_headers[key] = line.substr(trunc, line.size() - trunc);
    }

    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++) {
        std::cout << it->first << it->second << std::endl;
    }

    return 0;
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::string                         RequestParser::getMethod() { return (this->_method); }
std::string                         RequestParser::getUrl() { return (this->_url); }
std::string                         RequestParser::getVersion() { return (this->_version); }
std::string                         RequestParser::getBody() { return (this->_body); }
std::map<std::string, std::string>  RequestParser::getHeaders() { return (this->_headers); }
