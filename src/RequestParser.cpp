#include "RequestParser.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

RequestParser::RequestParser() : _method(""), _path(""), _query(""), _version(""), _headers(), _body(""), _body_length(0), _status(0) {}

RequestParser::RequestParser(const RequestParser &other) : _method(other._method), _path(other._path), _version(other._version), _headers(other._headers), _body(other._body), _body_length(other._body_length), _status(other._status) {}

RequestParser::~RequestParser() {}

RequestParser &RequestParser::operator=(const RequestParser &other)
{
    if (this != &other)
    {
        this->_method = other._method;
        this->_path = other._path;
        this->_path = other._query;
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

/* getNextLine extract the next line, removing \r\n at the end of line */
std::string RequestParser::getNextLine(std::string &str, size_t &start)
{
    std::string line;
    size_t      end;

    end = str.find_first_of('\n', start);
    line = str.substr(start, end - start);
    start = end + 1;
    if (line[line.size() - 1] == '\r')
        ft::popBack(line);

    return line;
}

void RequestParser::trimWhitespaces(std::string &str)
{
    ft::trimLeft(str, " ");
    ft::trimRight(str, " ");
}

int RequestParser::exitStatus(int exit_status)
{
    this->_status = exit_status;
    return (-1);
}

int RequestParser::isValidEncoding(std::string &to_check)
{
    if (to_check != "chunked" && to_check != "compress" && to_check != "deflate" && to_check != "gzip")
        return (-1);
    return (0);
}

/**************************************************************************************/
/*                                      CHECKERS                                      */
/**************************************************************************************/

int RequestParser::checkMethod(std::string &method)
{
    if (method == "GET" || method == "POST"
        || method == "DELETE")
        return 0;
    return (exitStatus(NOT_IMPLEMENTED));
}

int RequestParser::checkWhitespaceBeforeComma(std::string &line)
{
    size_t comma = 0, start = 0;

    while ((comma = line.find_first_of(',', start)) != std::string::npos)
    {
        if (line[comma - 1] == ' ')
            return (exitStatus(BAD_REQUEST));
        start = comma + 1;
    }
    return 0;
}

int RequestParser::checkWhitespaceBeforeColon(std::string &line, size_t &trunc)
{
    if (line[trunc - 1] == ' ')
        return (exitStatus(BAD_REQUEST));
    return 0;
}

int RequestParser::checkHeaders()
{
    /* Check if there is whitespaces before comma */
    for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++) {
        if (checkWhitespaceBeforeComma(it->second) == -1)
            return (-1);
    }

    /* Check value of Content-Length if existing */
    if (_headers.find("Content-Length") != _headers.end())
    {
        std::map<std::string, std::string>::iterator it = this->_headers.find("Content-Length");
        this->_body_length = ft::stoi(it->second);
        if (this->_body_length < 0)
            return (exitStatus(BAD_REQUEST));
    }

    if (_headers.find("Host") == _headers.end())
        return (exitStatus(BAD_REQUEST));

    /* Check value of Transfer-Encoding if existing */
    size_t      comma = 0, start = 0;
    std::string to_check;

    if (_headers.find("Transfer-Encoding") != _headers.end())
    {
        std::string encoding = _headers.find("Transfer-Encoding")->second;
        if (encoding.find_first_of(',') != std::string::npos)
        {
            while ((comma = encoding.find_first_of(',', start)) != std::string::npos)
            {
                ft::skipWhitespaces(encoding, start);
                to_check = encoding.substr(start, comma - start);

                if (isValidEncoding(to_check) == -1)
                    return (exitStatus(BAD_REQUEST));

                start = comma + 1;
            }
        }
        to_check = encoding.substr(start, encoding.size() - start);
        if (isValidEncoding(to_check) == -1)
            return (exitStatus(BAD_REQUEST));
    }

    return 0;
}

/**************************************************************************************/
/*                                      PARSERS                                       */
/**************************************************************************************/

int RequestParser::appendHeaderValue(std::string &key, std::string &value)
{
    /* Multiple Content-Length is forbidden */
    if (key == "Content-Length")
        return (exitStatus(BAD_REQUEST));

    std::map<std::string, std::string>::iterator it = this->_headers.find(key);
    it->second.append(", ");
    it->second.append(value);

    return 0;
}

int RequestParser::parseVersion(std::string &first_line, size_t &start, size_t &end)
{
    /* Stock the next ' ' index in end, and check if no ' ' are found */
    start = first_line.find_first_not_of(' ', end);
    if (start == std::string::npos)
    {
        std::cout << "Error Not Found : There is no newline after version HTTP" << std::endl;
        return (exitStatus(BAD_REQUEST));
    }

    /* Get the 5 next characters. Expect that it will be HTTP/ */
    std::string format = first_line.substr(start, 5);
    if (format != "HTTP/")
    {
        std::cout << "Wrong HTTP format" << std::endl;
        return (exitStatus(BAD_REQUEST));
    }

    /* Extract version */
    this->_version = first_line.substr(start + 5, 3);

    if (this->_version != "1.1")
    {
        std::cout << "Wrong HTTP version" << std::endl;
        return (exitStatus(HTTP_VERSION_UNSUPPORTED));
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
        return (exitStatus(BAD_REQUEST));
    }
    end = first_line.find_first_of(' ', start);

    this->_path = first_line.substr(start, end - start);
    if (this->_path.size() > URI_MAX_SIZE)
        return (exitStatus(URI_TOO_LONG));

    /* Substr and Erase with a pos : will copy/erase at pos, until the end */
    if (_path.find('?') != std::string::npos)
    {
        _query = _path.substr(_path.find('?') + 1);
        _path.erase(_path.find('?'));
    }

    if (_path[0] != '/')
        return (exitStatus(BAD_REQUEST));

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
    size_t      trunc = 0;

    line = getNextLine(request, index);
    if (line == "" || line == "\r" || line == "\r\n")
        return (exitStatus(BAD_REQUEST));

    while (line != "\r" && line != "")
    {
        /* Check if colon is present */
        trunc = line.find_first_of(":");
        if (trunc == std::string::npos || trunc == 0)
            return (exitStatus(BAD_REQUEST));

        /* Check if whitespace before colon */
        if (this->checkWhitespaceBeforeColon(line, trunc) == -1)
            return (-1);

        /* Get the key */
        key = line.substr(0, trunc);
        trunc += 1;

        /* Insert key and value : append to value if key is already in map */
        value = line.substr(trunc, line.size() - trunc);
        trimWhitespaces(value);

        if (this->_headers.find(key) != _headers.end()) {
            if (appendHeaderValue(key, value) == -1)
                return (-1);
        }
        else
            this->_headers[key] = value;

        line = getNextLine(request, index);
    }

    return (checkHeaders());
}

int RequestParser::parseChunkedBody(std::string &request, size_t &index)
{
    size_t end_line;
    index = request.find_first_not_of("\r\n", index);

    //TODO : detect errors
    while ((end_line = request.find_first_of("\r\n", index)) != std::string::npos)
    {
        std::string chunk_size = request.substr(index, end_line - index);
        int size = ft::hexToInt(chunk_size);
        if (size == 0)
            return 0;

        index = request.find_first_not_of("\r\n", end_line);
        _body += request.substr(index, size);

        index += size;
        end_line = request.find_first_of("\r\n", index);
        index = request.find_first_not_of("\r\n", end_line);
    }

    //TODO : body.length() > max_body_size => ERROR
    return -1;
}

int RequestParser::parseBody(std::string &request, size_t &index)
{
    if (index != request.size())
    {
        if (_headers.find("Transfer-Encoding") != _headers.end() && _headers["Transfer-Encoding"] == "chunked")
        {
            if (parseChunkedBody(request, index) == -1)
                return -1;
        }
        else
        {
            /* A server MAY reject a request that contains a message body but not a Content-Length */
            if (_headers.find("Content-Length") == _headers.end())
                return (exitStatus(LENGTH_REQUIRED));

            this->_body = request.substr(index, request.size() - index);

            if (_body_length > 0 && _body_length != (int)_body.size())
                return (exitStatus(BAD_REQUEST));
        }
    }

    return (0);
}

int RequestParser::parseRequest(const char *str)
{
    if (!str)
    {
        std::cerr << "Wrong buffer sent by recv" << std::endl;
        return (-1);
    }

    size_t      index = 0;
    std::string line;

    std::string request(str);

//    TODO : Here is some tests to remove later
    size_t inserted = 0;
    inserted = request.find('\n', 100) + 1;
    std::string to_insert = "Transfer-Encoding: chunked\r\n";
    request.insert(inserted, to_insert);
    request.append("8\r\nMamacita\r\nE\r\nYvan suce bien\r\n0\r\n\r\n");
//    TODO : END OF TEST

    line = getNextLine(request, index);
    if (this->parseFirstLine(line) == -1)
        return (-1);

    if (this->parseHeaders(request, index) == -1)
        return (-1);

    if (this->parseBody(request, index) == -1)
        return (-1);

    return (0);
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::map<std::string, std::string>  RequestParser::getHeaders() const { return (this->_headers); }
std::string                         RequestParser::getMethod() const { return (this->_method); }
std::string                         RequestParser::getPath() const { return (this->_path); }
std::string                         RequestParser::getQuery() const { return (this->_query); }
std::string                         RequestParser::getVersion() const { return (this->_version); }
std::string                         RequestParser::getBody() const { return (this->_body); }
int                                 RequestParser::getBodyLength() const { return (this->_body_length); }
int                                 RequestParser::getStatus() const { return (this->_status); }

/**************************************************************************************/
/*                                      NON MEMBERS                                   */
/**************************************************************************************/

std::ostream &operator<<(std::ostream &out, const RequestParser &rhs)
{
    out << "HTTP CLIENT REQUEST : \n"
        << rhs.getMethod() << ' ' << rhs.getPath() << ' ' << rhs.getVersion() << "\r\n";

    std::map<std::string, std::string> headers = rhs.getHeaders();
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        out << it->first << ": " << it->second << "\r\n";

    out << "\r\n" << rhs.getBody() << std::endl;

    return out;
}
/**************************************************************************************/
/*                                      SETTERS                                       */
/**************************************************************************************/

void RequestParser::setStatus(int new_status) { this->_status = new_status; }
