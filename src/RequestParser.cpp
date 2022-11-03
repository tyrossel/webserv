#include "RequestParser.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

RequestParser::RequestParser() : _string_request("") {}

RequestParser::RequestParser(const RequestParser &other)
{
	*this = other;
}

RequestParser::~RequestParser() {}

RequestParser &RequestParser::operator=(const RequestParser &other)
{
    if (this != &other)
    {
        this->_request = other._request;
        this->_string_request = other._string_request;
    }
    return (*this);
}

/**************************************************************************************/
/*                                      UTILS                                         */
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

int RequestParser::exitStatus(int exit_status)
{
    _request._status = exit_status;
    return (-1);
}

/**************************************************************************************/
/*                                      CHECKERS                                      */
/**************************************************************************************/

int RequestParser::isValidEncoding(std::string &to_check)
{
    if (to_check != "chunked" && to_check != "compress" && to_check != "deflate" && to_check != "gzip")
        return (-1);
    return (0);
}

int RequestParser::checkMethod(RequestType method)
{
    if (method != Unknown)
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
    for (std::map<std::string, std::string>::iterator it = _request._headers.begin(); it != _request._headers.end(); it++) {
        if (checkWhitespaceBeforeComma(it->second) == -1)
            return (-1);
    }

    /* Check value of Content-Length if existing */
    if (_request._headers.find("Content-Length") != _request._headers.end())
    {
        std::map<std::string, std::string>::iterator it = _request._headers.find("Content-Length");
		try
		{
			_request._body_length = ft::stoi(it->second);
		}
		catch (const std::exception& e)
		{
			_request._body_length = -1;
		}
        if (_request._body_length < 0)
            return (exitStatus(BAD_REQUEST));
    }

    if (_request._headers.find("Host") == _request._headers.end())
        return (exitStatus(BAD_REQUEST));

    /* Check value of Transfer-Encoding if existing */
    size_t      comma = 0, start = 0;
    std::string to_check;

    if (_request._headers.find("Transfer-Encoding") != _request._headers.end())
    {
        std::string encoding = _request._headers.find("Transfer-Encoding")->second;
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

    std::map<std::string, std::string>::iterator it = _request._headers.find(key);
    it->second.append(", ");
    it->second.append(value);

    return 0;
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
        _request._status = BAD_REQUEST; //  server cannot or will not process the request due to something that is perceived to be a client error
        return (-1);
    }
    _request._method = ft::RequestFromString(first_line.substr(start, end));
    if (checkMethod(_request._method) == -1)
        return -1;

    return parsePath(first_line, start, end);
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

    _request._path = first_line.substr(start, end - start);
    if (_request._path.size() > URI_MAX_SIZE)
        return (exitStatus(URI_TOO_LONG));

    /* Substr and Erase with a pos : will copy/erase at pos, until the end */
    if (_request._path.find('?') != std::string::npos)
    {
        _request._query = _request._path.substr(_request._path.find('?') + 1);
        _request._path.erase(_request._path.find('?'));
    }

    if (_request._path[0] != '/')
        return (exitStatus(BAD_REQUEST));

    std::string formated_path = _request.getPath();
    for (size_t pos = formated_path.find("+"); formated_path.find("+") != std::string::npos; pos = formated_path.find("+")) {
        formated_path.replace(pos, 1, " ");
    }
    for (size_t pos = formated_path.find("%20"); formated_path.find("%20") != std::string::npos; pos = formated_path.find("%20")) {
        formated_path.replace(pos, 3, " ");
    }

    _request.setPath(formated_path);

    return (parseVersion(first_line, start, end));
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
    _request._version = first_line.substr(start + 5, 3);

    if (_request._version != "1.1")
    {
        std::cout << "Wrong HTTP version" << std::endl;
        return (exitStatus(HTTP_VERSION_UNSUPPORTED));
    }

    return 0;
}

int RequestParser::parseHeaders(size_t &index)
{
    std::string line, key, value;
    size_t      trunc = 0;

    line = getNextLine(_string_request, index);
    if (line == "" || line == "\r" || line == "\r\n")
        return (exitStatus(BAD_REQUEST));

    while (line != "\r" && line != "")
    {
        /* Check if colon is present */
        trunc = line.find_first_of(":");
        if (trunc == std::string::npos || trunc == 0)
            return (exitStatus(BAD_REQUEST));

        /* Check if whitespace before colon */
        if (checkWhitespaceBeforeColon(line, trunc) == -1)
            return (-1);

        /* Get the key */
        key = line.substr(0, trunc);
        trunc += 1;

        /* Insert key and value : append to value if key is already in map */
        value = line.substr(trunc, line.size() - trunc);
        ft::trim(value, " ");

        if (_request._headers.find(key) != _request._headers.end()) {
            if (appendHeaderValue(key, value) == -1)
                return (-1);
        }
        else
            _request._headers[key] = value;

        line = getNextLine(_string_request, index);
    }

    return (checkHeaders());
}

int RequestParser::parseBody(size_t &index)
{
    if (index != _string_request.size())
    {
        if (_request._headers.find("Transfer-Encoding") != _request._headers.end() && _request._headers["Transfer-Encoding"] == "chunked")
        {
            if (parseChunkedBody(index) == -1)
                return -1;
        }
        else
        {
            /* A server MAY reject a _request that contains a message body but not a Content-Length */
            if (_request._headers.find("Content-Length") == _request._headers.end())
                return (exitStatus(LENGTH_REQUIRED));

            _request._body = _string_request.substr(index, _string_request.size() - index);

            if (_request._body_length > 0 && _request._body_length != (int)_request._body.size())
                return (exitStatus(BAD_REQUEST));
        }
    }

    return (0);
}

int RequestParser::parseChunkedBody(size_t &index)
{
    size_t end_line;
    index = _string_request.find_first_not_of("\r\n", index);

    while ((end_line = _string_request.find("\r\n", index)) != std::string::npos)
    {
        std::string chunk_size = _string_request.substr(index, end_line - index);
        size_t size = ft::hexToInt(chunk_size);
        if (size == 0)
        {
            if (parseTrailer(index) == -1)
                return -1;
            return 0;
        }

        index = _string_request.find_first_not_of("\r\n", end_line);
        end_line = _string_request.find("\r\n", index);
        if (size != end_line - index)
            return (exitStatus(BAD_REQUEST));
        else
            _request._body += _string_request.substr(index, size);

        index += size;
        index = _string_request.find_first_not_of("\r\n", end_line);
    }

    return -1;
}

int RequestParser::parseTrailer(size_t &index)
{
    _string_request.erase(0, index);

    if (_string_request.substr(0, 5).compare("0\r\n\r\n") == 0)
        return 0;
    else if (_string_request.substr(0, 3).compare("0\r\n") == 0)
        _string_request.erase(0, 3);
    else
        return (exitStatus(BAD_REQUEST));

    size_t colon, end_line;
    while ((end_line = _string_request.find("\r\n")) != std::string::npos)
    {
        if ((colon = _string_request.find(':', 0)) != std::string::npos)
        {
            std::string key = _string_request.substr(0, colon);
            std::string value = _string_request.substr(colon + 1, end_line - colon);
            ft::trim(key);
            ft::trim(value);

            _request._headers[key] = value;
        }
        else
            return (exitStatus(BAD_REQUEST));

        _string_request.erase(0, end_line + 2);
        if (_string_request.empty())
            return 0;
    }
    return (exitStatus(BAD_REQUEST));
}

int RequestParser::parseRequest(std::string &request)
{
    size_t      index = 0;
    std::string line;

    _string_request = request;

    line = getNextLine(_string_request, index);
    if (parseFirstLine(line) == -1)
        return (-1);

    if (parseHeaders(index) == -1)
        return (-1);

    if (parseBody(index) == -1)
        return (-1);

    return (0);
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

Request RequestParser::getRequest() { return (this->_request); }
