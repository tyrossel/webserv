# include "CommonGatewayInterface.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

CGI::CGI(std::map<std::string, std::string> headers, std::string body) : _env(), _headers(headers), _body(body), _cwd(""), _cgi_path(""), _cgi_env() {}

CGI::CGI(const CGI &rhs) : _env(rhs._env), _headers(rhs._headers), _body(rhs._body), _cwd(""), _cgi_path(""), _cgi_env() {}

CGI::~CGI() {}

CGI &CGI::operator=(const CGI &rhs)
{
    if (this != &rhs)
    {
        this->_env = rhs._env;
        this->_headers = rhs._headers;
        this->_body = rhs._body;
        this->_cwd = rhs._cwd;
        this->_cgi_path = rhs._cgi_path;
        this->_cgi_env = rhs._cgi_env;
    }
    return (*this);
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

void CGI::setCGIEnvironment(const RequestParser *request, const Server *server)
{
    char *tmp = getcwd(NULL, 0);
    if (!tmp)
        return ;
    _cwd = tmp;
    free(tmp);
    _cgi_path = _cwd + "/cgi-bin/php-cgi";

    if (_headers.find("Auth-Scheme") != _headers.end())
        _env["AUTH_TYPE"] = _headers["Authorization"];

    if (_headers.find("Content_length") != _headers.end())
        _env["CONTENT_LENGTH"] = _headers["Content-length"];

    _env["CONTENT_TYPE"] = _headers["Content-type"];
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = request->getPath();
    _env["PATH_TRANSLATED"] = request->getPath();
    _env["QUERY_STRING"] = request->getQuery();
    _env["REMOTE_ADDR"] = server->getAddress();
    _env["REMOTE_IDENT"] = _headers["Authorization"];

    //  if request required authentication using the "Basic" mechanism (AUTH_TYPE= "Basic") : value of the REMOTE_USER meta-variable is set to the user-ID supplied
    //  In all other cases the value of this meta-variable is undefined.
    _env["REMOTE_USER"] = _headers["Authorization"];
    _env["REQUEST_METHOD"] = request->getMethod();
    _env["SCRIPT_NAME"] = _cgi_path;

    std::string host = _headers.find("Host")->second;
    _env["SERVER_NAME"] = host.substr(0, host.find(':'));
    _env["SERVER_PORT"] = server->getPort();
    _env["SERVER_PROTOCOL"] = "HTTP/" + request->getVersion();
    _env["SERVER_SOFTWARE"] = "WetServ/1.0";

    // TODO : Check if RequestHeaders have to be put in env

    if (!(_cgi_env = (char **)malloc(sizeof(char *) * (_env.size() + 1))))
        return ;

    int i = 0;
    for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
    {
        std::string tmp = it->first + "=" + it->second;

        // TODO : FREE IF CRASH
        if (!(_cgi_env[i] = ft::strdup(tmp.c_str())))
            return ;
        i++;
    }
    _cgi_env[i] = NULL;
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::map<std::string, std::string>  CGI::getEnv() { return (this->_env); }
std::map<std::string, std::string>  CGI::getHeaders() { return (this->_headers); }
std::string                         CGI::getBody() { return (this->_body); }
