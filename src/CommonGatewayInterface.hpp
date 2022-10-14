#ifndef WEBSERV_COMMONGATEWAYINTERFACE_HPP
#define WEBSERV_COMMONGATEWAYINTERFACE_HPP

#include "../includes/webserv.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"

class CGI {
    private:
        Server                              *_server;
        RequestParser                       *_request;
        std::map<std::string, std::string>  _env;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;

    public:
        CGI(Server *server, RequestParser *request, std::map<std::string, std::string> headers, std::string body);
        ~CGI();
        CGI(const CGI &rhs);
        CGI &operator=(const CGI &rhs);

        /* GETTERS */
        std::map<std::string, std::string>  getEnv();
        std::map<std::string, std::string>  getHeaders();
        std::string                         getBody();

        void setCGIEnvironment();
};

#endif //WEBSERV_COMMONGATEWAYINTERFACE_HPP
