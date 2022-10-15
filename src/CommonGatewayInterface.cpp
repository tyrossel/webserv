# include "CommonGatewayInterface.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

CGI::CGI(std::map<std::string, std::string> headers, std::string body) : _env(), _headers(headers), _body(body) {}

CGI::CGI(const CGI &rhs) : _env(rhs._env), _headers(rhs._headers), _body(rhs._body) {}

CGI::~CGI() {}

CGI &CGI::operator=(const CGI &rhs)
{
    if (this != &rhs)
    {
        this->_env = rhs._env;
        this->_headers = rhs._headers;
        this->_body = rhs._body;
    }
    return (*this);
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

void CGI::setCGIEnvironment(const RequestParser *request, const Server *server)
{
    if (_headers.find("Auth-Scheme") != _headers.end())
        _env["AUTH_TYPE"] = _headers["Authorization"];

    if (_headers.find("Content_length") != _headers.end())
        _env["CONTENT_LENGTH"] = _headers["Content-length"];

    _env["CONTENT_TYPE"] = _headers["Content-type"];
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = request->getPath();
    _env["PATH_TRANSLATED"] = request->getPath();
    _env["QUERY_STRING"] = request->getQuery();

    _env["REMOTE_ADDR"] = server->getHost();
    _env["REMOTE_IDENT"] = _headers["Authorization"];

    //  if request required authentication using the "Basic" mechanism (AUTH_TYPE= "Basic") : value of the REMOTE_USER meta-variable is set to the user-ID supplied
    //  In all other cases the value of this meta-variable is undefined.
    _env["REMOTE_USER"] = _headers["Authorization"];

    _env["REQUEST_METHOD"] = request->getMethod();

    //_env["SCRIPT_NAME"] = request->getPath();

    std::string host = _headers.find("Host")->second;
    _env["SERVER_NAME"] = host.substr(0, host.find(':'));
    _env["SERVER_PORT"] = server->getPort();
    _env["SERVER_PROTOCOL"] = "HTTP/" + request->getVersion();
    _env["SERVER_SOFTWARE"] = "WetServ/1.0";

    // TODO : Had to transform env to char** array malloced
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::map<std::string, std::string>  CGI::getEnv() { return (this->_env); }
std::map<std::string, std::string>  CGI::getHeaders() { return (this->_headers); }
std::string                         CGI::getBody() { return (this->_body); }
