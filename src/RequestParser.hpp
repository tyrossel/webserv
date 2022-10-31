#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "Request.hpp"
#include "webserv.hpp"

class RequestParser {
    private:
        Request     _request;
        std::string _string_request;

        // UTILS ===================================================
        std::string     getNextLine(std::string &str, size_t &start);
        int             exitStatus(int exit_status);
        // =========================================================

        // CHECKERS ================================================
        int             isValidEncoding(std::string &to_check);
        int             checkMethod(RequestType method);
        int             checkWhitespaceBeforeComma(std::string &line);
        int             checkWhitespaceBeforeColon(std::string &line, size_t &trunc);
        int             checkHeaders();
        // =========================================================

        // PARSERS =================================================
        int             appendHeaderValue(std::string &key, std::string &value);
        int             parseFirstLine(std::string &first_line);
        int             parsePath(std::string &first_line, size_t &start, size_t &end);
        int             parseVersion(std::string &first_line, size_t &start, size_t &end);
        int             parseHeaders(size_t &index);
        int             parseBody(size_t &index);
        int             parseChunkedBody(size_t &index);
        int             parseTrailer(size_t &index);
        // =========================================================

    public:
        RequestParser();
        RequestParser(const RequestParser &other);
        ~RequestParser();

        RequestParser &operator=(const RequestParser &other);

        int                 parseRequest(std::string &request);

        Request             getRequest();
};


#endif //REQUESTPARSER_HPP
