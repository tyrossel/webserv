#ifndef WEBSERV_COMMONGATEWAYINTERFACE_HPP
#define WEBSERV_COMMONGATEWAYINTERFACE_HPP

#include "webserv.hpp"
#include "RequestParser.hpp"
#include "Server.hpp"

class CGI {
    private:
        std::map<std::string, std::string>  _env;
        std::map<std::string, std::string>  _headers;
        std::string                         _req_body;
        std::string                         _cgi_path;
        std::string                         _file_path;
        std::string                         _ret_headers;
        std::string                         _ret_body;
        char**                              _cgi_env;

    public:
        CGI(const Request &request);
        ~CGI();
        CGI(const CGI &rhs);
        CGI &operator=(const CGI &rhs);

        int         returnFail(int ret, std::string error, bool close_pipe=false, int pip_to_cgi=-1, int pip_from_cgi=-1);
        void        exitFail(std::string error, int exit_value);

        std::string readContent(int fd);
        void        splitContent(std::string &content);
        int         setCGIEnvironment(const Request *request, const Server &server, const Location &loc);
        int         executeCgi(const Request *request, const Server &server, const Location &loc);

        /* GETTERS */
        std::map<std::string, std::string>  getEnv();
        std::map<std::string, std::string>  getHeaders();
        std::string                         getReqBody();
        std::string                         getCwd();
        std::string                         getCGIPath();
        std::string                         getFilePath();
		std::string                         getRetHeaders();
		std::string                         getRetBody();
};

#endif //WEBSERV_COMMONGATEWAYINTERFACE_HPP
