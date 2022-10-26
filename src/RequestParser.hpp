#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "../includes/webserv.hpp"

class Server;
struct Location;

class RequestParser {
private:
    std::string                         _method;
    std::string                         _path;     // According to client
	std::string							_location; // Where the file is physically located
    std::string                         _query;
    std::string                         _version;
    std::map<std::string, std::string>  _headers;
    std::string                         _body;
    std::string                         _request;
    int                                 _body_length;
    int                                 _status;

public:
    RequestParser();
    RequestParser(const RequestParser &other);
    ~RequestParser();

    RequestParser &operator=(const RequestParser &other);

    std::string                         getNextLine(std::string &str, size_t &i);

    void                                trimWhitespaces(std::string &str);
    int                                 exitStatus(int exit_status);
    int                                 isValidEncoding(std::string &to_check);

    int                                 checkMethod(std::string &method);
    int                                 checkWhitespaceBeforeComma(std::string &line);
    int                                 checkWhitespaceBeforeColon(std::string &line, size_t &trunc);
    int                                 checkHeaders();

    int                                 appendHeaderValue(std::string &key, std::string &value);
    int                                 parsePath(std::string &first_line, size_t &start, size_t &end);
    int                                 parseVersion(std::string &first_line, size_t &start, size_t &end);
    int                                 parseFirstLine(std::string &first_line);
    int                                 parseHeaders(size_t &index);
    int                                 parseTrailer(size_t &index);
    int                                 parseChunkedBody(size_t &index);
    int                                 parseBody(size_t &index);
    int                                 parseRequest(const char *request);

    const Location &                    FindLocation(const Server &server) const;
    const Server &                      FindServer(const std::vector<Server> &servers) const;
	bool								isValid(const Location &loc) const;
    void								updatePathWithLocation(const Location &loc);

    /* GETTERS */
    std::map<std::string, std::string>  getHeaders() const;
    std::string                         getMethod() const;
    std::string                         getPath() const;
    std::string                         getLocation() const;
    std::string                         getQuery() const;
    std::string                         getVersion() const;
    std::string                         getBody() const;
    int                                 getBodyLength() const;
    int                                 getStatus() const;

    /* SETTERS */
    void                                setStatus(int new_status);

};

std::ostream &operator<<(std::ostream &out, const RequestParser &rhs);

#endif //REQUESTPARSER_HPP
