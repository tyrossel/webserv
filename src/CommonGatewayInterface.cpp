# include "CommonGatewayInterface.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

CGI::CGI(Server *server, RequestParser *request, std::map<std::string, std::string> headers, std::string body) : _server(server), _request(request), _env(), _headers(headers), _body(body) {}

CGI::CGI(const CGI &rhs) : _server(rhs._server), _request(rhs._request), _env(rhs._env), _headers(rhs._headers), _body(rhs._body) {}

CGI::~CGI() {}

CGI &CGI::operator=(const CGI &rhs)
{
    if (this != &rhs)
    {
        this->_server = rhs._server;
        this->_request = rhs._request;
        this->_env = rhs._env;
        this->_headers = rhs._headers;
        this->_body = rhs._body;
    }
    return (*this);
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

void CGI::setCGIEnvironment() {
    //_env["AUTH_TYPE"] == ???????
    // _env["REMOTE_IDENT"] & _env["REMOTE_USER"] see later

    if (_headers.find("Content_length") != _headers.end())
        _env["CONTENT_LENGTH"] = _headers["Content_length"];
    _env["CONTENT_TYPE"] = _headers["Content_type"];
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";

    // _env["PATH_INFO"] and _env["PATH_TRANSLATED"] will be set using path of RequestClient but reformat

    _env["QUERY_STRING"] = _request->getQuery();

    // _env["REMOTE_ADDR"] : ipv4 or ipv6 of the client sending the request
    // _env["REMOTE_HOST"] : SHOULD be set so ..... no

    _env["REQUEST_METHOD"] = _request->getMethod();

    // _env["SCRIPT_NAME"] : set to a URI path which could identify the CGI script

    // _env["SERVER_NAME"] : MUST be set to the name of the server host to which the client request is directed

    _env["SERVER_PORT"] = _server->getPort();

    _env["SERVER_PROTOCOL"] = "HTTP/" + _request->getVersion();

    _env["SERVER_SOFTWARE"] = "WetServHugeTataz/1.0";

    // Had to transform env to char** array malloced
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::map<std::string, std::string>  CGI::getEnv() { return (this->_env); }
std::map<std::string, std::string>  CGI::getHeaders() { return (this->_headers); }
std::string                         CGI::getBody() { return (this->_body); }
