#ifndef WEBSERV_COMMONGATEWAYINTERFACE_HPP
#define WEBSERV_COMMONGATEWAYINTERFACE_HPP

#include "../includes/webserv.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"

class CGI {
    private:
        std::map<std::string, std::string>  _env;
        std::map<std::string, std::string>  _headers;
        std::string                         _req_body;
        std::string                         _cwd;
        std::string                         _cgi_path;
        std::string                         _file_path;
        std::string                         _ret_body;
        int                                 _fd_file;
        char**                              _cgi_env;

    public:
        CGI(std::map<std::string, std::string> headers, std::string body);
        ~CGI();
        CGI(const CGI &rhs);
        CGI &operator=(const CGI &rhs);

        /* GETTERS */
        std::map<std::string, std::string>  getEnv();
        std::map<std::string, std::string>  getHeaders();
        std::string                         getReqBody();
        std::string                         getCwd();
        std::string                         getCGIPath();
        std::string                         getFilePath();
        std::string                         getRetBody();
        int                                 getFdFile();

        std::string readContent();
        int setCGIEnvironment(const RequestParser *request, const Server *server);
        int executeCgi(const RequestParser *request, const Server *server);
};

#endif //WEBSERV_COMMONGATEWAYINTERFACE_HPP
