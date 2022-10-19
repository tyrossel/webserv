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
    pid_t		pid;
    int			saveStdin;
    int			saveStdout;
    std::string	_ret_body;
//    char        *argv[3];

    if (setCGIEnvironment(request, server))
        return INTERNAL_SERVER_ERROR;
//    if (!(argv[0] = ft::strdup(_cgi_path.c_str())))
//        return INTERNAL_SERVER_ERROR;
//    if (!(argv[1] = ft::strdup(_file_path.c_str())))
//        return INTERNAL_SERVER_ERROR;
//    argv[2] = NULL;

    // SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
    saveStdin = dup(STDIN_FILENO);
    saveStdout = dup(STDOUT_FILENO);

    FILE	*fIn = tmpfile();
    FILE	*fOut = tmpfile();
    long	fdIn = fileno(fIn);
    long	fdOut = fileno(fOut);
    int		ret = 1;

    write(fdIn, _req_body.c_str(), _req_body.size());
    lseek(fdIn, 0, SEEK_SET);

    pid = fork();

    if (pid == -1)
    {
        std::cerr << RED << "Fork crashed." << RESET << std::endl;
        return INTERNAL_SERVER_ERROR;
    }
    else if (!pid)
    {
        char * const * nll = NULL;

        dup2(fdIn, STDIN_FILENO);
        dup2(fdOut, STDOUT_FILENO);
        execve(_cgi_path.c_str(), nll, _cgi_env);
        std::cerr << RED << "Execve crashed." << RESET << std::endl;
        write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
    }
    else
    {
        char	buffer[4096] = {0};

        waitpid(-1, NULL, 0);
        lseek(fdOut, 0, SEEK_SET);

        ret = 1;
        while (ret > 0)
        {
            memset(buffer, 0, 4096);
            ret = read(fdOut, buffer, 4096 - 1);
            _ret_body += buffer;
        }
    }

    dup2(saveStdin, STDIN_FILENO);
    dup2(saveStdout, STDOUT_FILENO);
    fclose(fIn);
    fclose(fOut);
    close(fdIn);
    close(fdOut);
    close(saveStdin);
    close(saveStdout);

    if (!pid)
        exit(0);

    std::cout << "---------------------CGI-------------------------" << std::endl;
    std::cout << BLUE << _ret_body << RESET << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;

    return (HTTP_OK);
}


int CGI::setCGIEnvironment(const RequestParser *request, const Server *server)
{
    char *tmp = getcwd(NULL, 0);

    if (!tmp)
        return 1;
    _cwd = tmp;
    free(tmp);

    _cgi_path = _cwd + "/cgi-bin/ubuntu_cgi_tester";
    _file_path = server->getRoot() + request->getPath();
    std::cout << "File path : " << _file_path << std::endl;
    _cgi_env = NULL;

    if (_headers.find("Auth-Scheme") != _headers.end())
        _env["AUTH_TYPE"] = _headers["Authorization"];

    if (_headers.find("Content-Length") != _headers.end())
        _env["CONTENT_LENGTH"] = _headers["Content-Length"];

    if (_headers.find("Content-Type") != _headers.end())
        _env["CONTENT_TYPE"] = _headers["Content-Type"];

    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = _file_path;
    _env["PATH_TRANSLATED"] = _file_path;
    std::cout << _env["PATH_INFO"] << std::endl;

    _env["QUERY_STRING"] = request->getQuery();
//    _env["REMOTE_ADDR"] = server->getAddress();
    _env["REMOTE_HOST"] = "";

    //  if request required authentication using the "Basic" mechanism (AUTH_TYPE= "Basic") : value of the REMOTE_USER meta-variable is set to the user-ID supplied
    //  In all other cases the value of this meta-variable is undefined.
//    _env["REMOTE_USER"] = _headers["Authorization"];
    _env["REQUEST_METHOD"] = request->getMethod();
    _env["SCRIPT_NAME"] = _cgi_path;

    _env["SERVER_NAME"] = server->getAddress();
    _env["SERVER_PORT"] = ft::to_string(server->getPort());
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
        return 1;

//    int i = 0;
//    while (_cgi_env[i])
//        printf("%s\n", _cgi_env[i++]);

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
