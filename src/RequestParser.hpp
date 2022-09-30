#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "../includes/webserv.hpp"

class RequestParser {
    private:
        std::string                         _method;
        std::string                         _url;
        std::string                         _version;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;
        int                                 _return;

    public:
        RequestParser();
        RequestParser(const RequestParser &other);
        ~RequestParser();

        RequestParser &operator=(const RequestParser &other);

        std::string                         getNextLine(std::string &str, size_t &i);
        int                                 parseFirstLine(std::string &str);
        int                                 parseRequest(const char *request);

        /* GETTERS */
        std::string                         getMethod();
        std::string                         getVersion();
        std::string                         getBody();
        std::map<std::string, std::string>  getHeaders();

};

#endif //REQUESTPARSER_HPP
