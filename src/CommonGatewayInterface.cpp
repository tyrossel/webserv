# include "CommonGatewayInterface.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

CGI::CGI(const Request &request) : _env(), _headers(request.getHeaders()), _req_body(request.getBody()), _cgi_path(""), _file_path(""), _ret_headers(""), _ret_body(""), _cgi_env(NULL) {}

CGI::CGI(const CGI &rhs)
{
    *this = rhs;
}

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
        this->_cgi_path = rhs._cgi_path;
        this->_file_path = rhs._file_path;
        this->_ret_body = rhs._ret_headers;
        this->_ret_body = rhs._ret_body;
        this->_cgi_env = rhs._cgi_env;
    }
    return (*this);
}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

int CGI::returnFail(int ret, std::string error, bool close_pipe, int pip_to_cgi, int pip_from_cgi)
{
    if (close_pipe)
    {
        if (pip_to_cgi != -1)
            close(pip_to_cgi);
        close(pip_from_cgi);
    }
    std::cerr << RED << error << RESET << std::endl;

    return ret;
}

void CGI::exitFail(std::string error, int exit_value)
{
    std::cerr << RED << error << RESET << std::endl;
    exit(exit_value);
}

void CGI::splitContent(std::string &content)
{
    content.erase(0, content.find_first_of("\r\n") + 2);
    _ret_headers = content.substr(0, content.find_first_of("\r\n\r\n"));
    content.erase(0, _ret_headers.size() + 4);
    _ret_body = content;
}

std::string CGI::readContent(int fd)
{
    std::string ret;
    char buf[4096 + 1];
    int readed;

    lseek(fd, 0, SEEK_SET);
    while ((readed = read(fd, buf, 4096)) != 0)
    {
        if (readed == -1)
            return "";
        buf[readed] = '\0';
        ret.insert(ret.length(), buf, readed);
    }
    return ret;
}

int CGI::executeCgi(const Request *request, const Server &server, const Location &loc)
{
    if (setCGIEnvironment(request, server, loc) == -1)
        return INTERNAL_SERVER_ERROR;


    int pip_to_cgi[2];
    int pip_from_cgi[2];

    if (pipe(pip_to_cgi) != 0)
        return returnFail(INTERNAL_SERVER_ERROR, "Pipe Failed for pip_to_cgi");
    if (pipe(pip_from_cgi) != 0)
        return returnFail(INTERNAL_SERVER_ERROR, "Pipe Failed for pip_to_cgi", true, pip_to_cgi[0], pip_to_cgi[1]);

    pid_t pid = fork();

    switch (pid) {
        case -1:
            return returnFail(BAD_GATEWAY, "Fork failed", true, pip_to_cgi[1], pip_from_cgi[0]);

        case 0:
            char        *argv[3];

            if (!(argv[0] = ft::strdup(_cgi_path.c_str())))
                exitFail("Can't duplicate _cgi_path", 42);
            if (!(argv[1] = ft::strdup(_file_path.c_str())))
                exitFail("Can't duplicate _path", 42);
            argv[2] = NULL;

            if (dup2(pip_to_cgi[0], STDIN_FILENO) == -1)
                exitFail("Can't duplicate pip_to_cgi to STDIN_FILENO", 42);
            if (dup2(pip_from_cgi[1], STDOUT_FILENO) == -1)
                exitFail("Can't duplicate pip_from_cgi to STDOUT_FILENO", 42);

            close(pip_from_cgi[0]);
            close(pip_from_cgi[1]);
            close(pip_to_cgi[0]);
            close(pip_to_cgi[1]);

            if (execve(argv[0], argv, _cgi_env) < 0)
                exitFail("Execve failed", 1);
            break;

        default:
            close(pip_to_cgi[0]);
            close(pip_from_cgi[1]);
            if (_req_body.length() && write(pip_to_cgi[1], _req_body.c_str(), _req_body.length()) <= 0)
                return returnFail(INTERNAL_SERVER_ERROR, "Write failed", true, pip_to_cgi[1], pip_from_cgi[0]);
            close(pip_to_cgi[1]);
            pip_to_cgi[1] = -1;

            int status;

            if (waitpid(pid, &status, 0) == -1)
                return returnFail(INTERNAL_SERVER_ERROR, "Waitpid waiting for child process", true, pip_to_cgi[1], pip_from_cgi[0]);
            if (WIFEXITED(status) && WEXITSTATUS(status))
            {
                if (WEXITSTATUS(status) == 42)
                    return returnFail(INTERNAL_SERVER_ERROR, "Child exit with code 42", true, pip_to_cgi[1], pip_from_cgi[0]);
                else
                    return returnFail(BAD_GATEWAY, "Child exit with code " + ft::to_string(WEXITSTATUS(status)), true, pip_to_cgi[1], pip_from_cgi[0]);
            }
            std::string ret = readContent(pip_from_cgi[0]);
            splitContent(ret);
            close(pip_from_cgi[0]);
    }
    return (HTTP_OK);
}

int CGI::setCGIEnvironment(const Request *request, const Server &server, const Location &loc)
{
    _file_path = request->getLocation();
	_cgi_path = loc.cgi_bin;

    if (_headers.find("Auth-Scheme") != _headers.end())
        _env["AUTH_TYPE"] = _headers["Authorization"];

    if (_headers.find("Content-Length") != _headers.end())
        _env["CONTENT_LENGTH"] = _headers["Content-Length"];

    _env["CONTENT_TYPE"] = _headers["Content-Type"];
    _env["FILE_UPLOADS"] = "On";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = _file_path;
    _env["PATH_TRANSLATED"] = _file_path;
    _env["QUERY_STRING"] = request->getQuery();
    _env["REDIRECT_STATUS"] = "0";
    _env["REMOTE_HOST"] = "";
    _env["REQUEST_METHOD"] = ft::RequestToString(request->getMethod());
    _env["REQUEST_URI"] = _file_path;
    _env["SCRIPT_NAME"] = loc.cgi_bin;
    _env["SERVER_NAME"] = server.getAddress();
    _env["SERVER_PORT"] = ft::to_string(server.getPort());
    _env["SERVER_PROTOCOL"] = "HTTP/" + request->getVersion();
    _env["SERVER_SOFTWARE"] = "WetServ/1.0";


    for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
    {
        if (!it->second.empty())
        {
            std::string header = "HTTP_" + ft::toUpper(it->first);
            std::replace(header.begin(), header.end(), '-', '_');
            _env[header] = it->second;
        }
    }

    if (!(_cgi_env = ft::mapToArray(_env)))
        return -1;
    return 0;
}

/**************************************************************************************/
/*                                      GETTERS                                       */
/**************************************************************************************/

std::map<std::string, std::string>  CGI::getEnv() { return (this->_env); }
std::map<std::string, std::string>  CGI::getHeaders() { return (this->_headers); }
std::string                         CGI::getReqBody() { return (this->_req_body); }
std::string                         CGI::getCGIPath() { return (this->_cgi_path); }
std::string                         CGI::getFilePath() { return (this->_file_path); }
std::string                         CGI::getRetHeaders() { return (this->_ret_headers); }
std::string                         CGI::getRetBody() { return (this->_ret_body); }
