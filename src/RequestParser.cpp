#include "RequestParser.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

RequestParser::RequestParser() : _method(""), _path(""), _version(""), _headers(), _body(""), _body_length(0), _status(0) {}

RequestParser::RequestParser(const RequestParser &other) : _method(other._method), _path(other._path), _version(other._version), _headers(other._headers), _body(other._body), _body_length(other._body_length), _status(other._status) {}

RequestParser::~RequestParser() {}

RequestParser &RequestParser::operator=(const RequestParser &other)
{
    if (this != &other)
    {
        this->_method = other._method;
        this->_path = other._path;
        this->_version = other._version;
        this->_headers = other._headers;
        this->_body = other._body;
        this->_body_length = other._body_length;
        this->_status = other._status;
    }
    return (*this);
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/


void RequestParser::trimWhitespaces(std::string &value)
{
    ft::trimLeft(value, ' ');
    ft::trimRight(value, ' ');
}

int RequestParser::appendHeaderValue(std::string &key, std::string &value)
{
    /* Multiple Content-Length is forbidden */
    if (key == "Content-Length")
    {
        this->_status = BAD_REQUEST;
        return -1;
    }

    std::map<std::string, std::string>::iterator it = this->_headers.find(key);
    it->second.append(", ");
    it->second.append(value);

    return 0;
}

int RequestParser::checkHeaders()
{
    /* Check value of Content-Length if existing */
    if (_headers.find("Content-Length") != _headers.end())
    {
        std::map<std::string, std::string>::iterator it = this->_headers.find("Content-Length");
        this->_body_length = ft::stoi(it->second);
        if (this->_body_length < 0)
        {
            _status = BAD_REQUEST;
            return (-1);
        }
    }

    return 0;
}

int RequestParser::checkWhitespaceBeforeColon(std::string &line, size_t &trunc)
{
    if (line[trunc - 1] == ' ')
    {
        this->_status = BAD_REQUEST;
        return (-1);
    }
    return 0;
}

int RequestParser::checkMethod(std::string &method)
{
    // Rest Connect, Options, Trace and Patch

    if (method == "GET" || method == "HEAD"
        || method == "POST" || method == "PUT"
        || method == "DELETE")
        return 0;
    this->_status = NOT_IMPLEMENTED;
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
        this->_status = BAD_REQUEST;
        return (-1);
    }

    /* Get the 5 next characters. Expect that it will be HTTP/ */
    std::string format = first_line.substr(start, 5);
    if (format != "HTTP/")
    {
        std::cout << "Wrong HTTP format" << std::endl;
        this->_status = BAD_REQUEST;
        return (-1);
    }

    /* Extract version */
    this->_version = first_line.substr(start + 5, 3);

    //TODO : Accept 1.0 ??
    if (this->_version != "1.1")
    {
        std::cout << "Wrong HTTP version" << std::endl;
        this->_status = BAD_REQUEST;
        return (-1);
    }

    return 0;
}

int RequestParser::parsePath(std::string &first_line, size_t &start, size_t &end)
{
    /* Stock the next spaces index in end, and check if no spaces are found */
    start = first_line.find_first_not_of(' ', end);
    if (start == std::string::npos)
    {
        std::cout << "Error Not Found : There is no spaces after URL" << std::endl;
        this->_status = BAD_REQUEST;
        return (-1);
    }
    end = first_line.find_first_of(' ', start);
    this->_path = first_line.substr(start, end - start);
    if (this->_path.size() > URI_MAX_SIZE)
    {
        this->_status = URI_TOO_LONG;
        return (-1);
    }

    return (parseVersion(first_line, start, end));
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
        this->_status = BAD_REQUEST; //  server cannot or will not process the request due to something that is perceived to be a client error
        return (-1);
    }
    this->_method = first_line.substr(start, end);
    if (checkMethod(this->_method) == -1)
        return -1;

    return parsePath(first_line, start, end);
}

int RequestParser::parseHeaders(std::string &request, size_t &index)
{
    std::string line, key, value;
    size_t      trunc = 0, end_spaces = 0;

    line = getNextLine(request, index);
    if (line == "" || line == "\r" || line == "\r\n") {
        this->_status = BAD_REQUEST;
        return (-1);
    }

    while (line != "\r" && line != "")
    {
        /* Check if colon is present */
        trunc = line.find_first_of(":");
        if (trunc == std::string::npos || trunc == 0)
        {
            this->_status = BAD_REQUEST;
            return (-1);
        }

        /* Check if whitespace before colon */
        if (this->checkWhitespaceBeforeColon(line, trunc) == -1)
            return (-1);

        /* Get the key */
        key = line.substr(0, trunc);

//        /* Trim whitespaces before and after value */
//        this->trimWhitespaces(line, trunc, end_spaces);

        /* Insert key and value : append to value if key is already in map */
        value = line.substr(trunc, line.size() - trunc);
        trimWhitespaces(value);
        //TODO : check if whitespaces before coma

        if (this->_headers.find(key) != _headers.end()) {
            if (appendHeaderValue(key, value) == -1)
                return (-1);
        }
        else
                this->_headers[key] = value;

        end_spaces = 0;
        line = getNextLine(request, index);
    }

//    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++) {
//        std::cout << it->first << it->second << std::endl;
//    }

    return (checkHeaders());
}

int RequestParser::parseRequest(const char *str)
{
    if (!str)
    {
        std::cerr << "Wrong buffer send by recv" << std::endl;
        return (-1);
    }

    size_t      index = 0;
    std::string line;

    std::string request(str);

    line = getNextLine(request, index);
    if (this->parseFirstLine(line) == -1)
        return (-1);

    if (this->parseHeaders(request, index) == -1)
        return (-1);

    return (0);
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::map<std::string, std::string>  RequestParser::getHeaders() { return (this->_headers); }
std::string                         RequestParser::getMethod() { return (this->_method); }
std::string                         RequestParser::getPath() { return (this->_path); }
std::string                         RequestParser::getVersion() { return (this->_version); }
std::string                         RequestParser::getBody() { return (this->_body); }
int                                 RequestParser::getStatus() { return (this->_status); }
