#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

class RequestParser {
    private:
        std::string                         _method;
        std::string                         _version;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;

    public:
        RequestParser();
        RequestParser(const RequestParser &other);
        ~RequestParser();

        RequestParser &operator=(const RequestParser &other);

        void                                parseRequest(const char *request);

        /* GETTERS */
        std::string                         getMethod();
        std::string                         getVersion();
        std::string                         getBody();
        std::map<std::string, std::string>  getHeaders();

};

#endif //REQUESTPARSER_HPP
