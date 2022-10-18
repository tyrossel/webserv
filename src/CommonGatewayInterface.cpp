# include "CommonGatewayInterface.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

CGI::CGI(std::map<std::string, std::string> headers, std::string req_body) : _env(), _headers(headers), _req_body(req_body), _cwd(""), _cgi_path(""), _file_path(""), _ret_body(""), _fd_file(0), _cgi_env() {}

CGI::CGI(const CGI &rhs) : _env(rhs._env), _headers(rhs._headers), _req_body(rhs._req_body), _cwd(""), _cgi_path(""), _file_path(""), _ret_body(""), _fd_file(0), _cgi_env() {}

CGI::~CGI()
{
    if (_cgi_env)
        ft::freeArray(_cgi_env);
}

CGI &CGI::operator=(const CGI &rhs)
{
    if (this != &rhs)
    {
        this->_env = rhs._env;
        this->_headers = rhs._headers;
        this->_req_body = rhs._req_body;
        this->_cwd = rhs._cwd;
        this->_cgi_path = rhs._cgi_path;
        this->_file_path = rhs._file_path;
        this->_ret_body = rhs._ret_body;
        this->_fd_file = rhs._fd_file;
        this->_cgi_env = rhs._cgi_env;
    }
    return (*this);
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

std::string CGI::readContent()
{
    std::string ret;
    char buf[4096 + 1];
    int readed;

    lseek(_fd_file, 0, SEEK_SET);
    while ((readed = read(_fd_file, buf, 4096)) != 0)
    {
        if (readed == -1)
            return "";
        buf[readed] = '\0';
        ret.insert(ret.length(), buf, readed);
    }
    return ret;
}

int CGI::executeCgi(const RequestParser *request, const Server *server)
{
    char *argv[3];

    // TODO : tracking error, free if necessary
    if (setCGIEnvironment(request, server))
        return INTERNAL_SERVER_ERROR;
    if (!(argv[0] = ft::strdup(_cgi_path.c_str())))
        return INTERNAL_SERVER_ERROR;
    if (!(argv[1] = ft::strdup(_file_path.c_str())))
        return INTERNAL_SERVER_ERROR;
    argv[2] = NULL;

    int status, pip[2];

    if (pipe(pip) != 0)
        return INTERNAL_SERVER_ERROR;

    std::string _file_directory = _file_path.substr(0, _file_path.find_last_of('/'));
    _fd_file = open("webserv_cgi", O_CREAT | O_RDWR | O_TRUNC, 00755);

    pid_t pid = fork();

    /* CHILD PART */
    if (pid == 0)
    {
        if (chdir(_file_directory.c_str()) == -1)
            return INTERNAL_SERVER_ERROR;
        close(pip[1]);
        if (dup2(pip[0], 0) == -1)
            return INTERNAL_SERVER_ERROR;
        if (dup2(_fd_file, 1) == -1)
            return INTERNAL_SERVER_ERROR;
        close(pip[0]);
        execve(argv[0], argv, _cgi_env);
        exit(1);
    }
    /* PARENT PART */
    else if (pid > 0) 
    {
        close(pip[0]);
        if (_req_body.length() && write(pip[1], _req_body.c_str(), _req_body.length()) <= 0)
            return INTERNAL_SERVER_ERROR;
        close(pip[1]);

        if (waitpid(pid, &status, 0) == -1)
            return INTERNAL_SERVER_ERROR;
        if (WIFEXITED(status) && WEXITSTATUS(status))
            return BAD_GATEWAY;
    }
    else
    {
        return 502;
    }

    _ret_body = readContent();
    std::cout << BLUE << _ret_body << RESET << std::endl;

    return 200;
}


int CGI::setCGIEnvironment(const RequestParser *request, const Server *server)
{
    char *tmp = getcwd(NULL, 0);
    if (!tmp)
        return 1;
    _cwd = tmp;
    free(tmp);

    _cgi_path = _cwd + "/cgi-bin/php-cgi";
    _file_path = server->getRoot() + request->getPath();
    _cgi_env = NULL;

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
    _env["SERVER_PORT"] = ft::to_string(server->getPort());
    _env["SERVER_PROTOCOL"] = "HTTP/" + request->getVersion();
    _env["SERVER_SOFTWARE"] = "WetServ/1.0";

    // TODO : Check if RequestHeaders have to be put in env

    if (!(_cgi_env = ft::mapToArray(_env)))
        return 1;

    return 0;
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::map<std::string, std::string>  CGI::getEnv() { return (this->_env); }
std::map<std::string, std::string>  CGI::getHeaders() { return (this->_headers); }
std::string                         CGI::getReqBody() { return (this->_req_body); }
std::string                         CGI::getCwd() { return (this->_cwd); }
std::string                         CGI::getCGIPath() { return (this->_cgi_path); }
std::string                         CGI::getFilePath() { return (this->_file_path); }
std::string                         CGI::getRetBody() { return (this->_ret_body); }
int                                 CGI::getFdFile() { return (this->_fd_file); }
