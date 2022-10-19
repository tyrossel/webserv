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
    char        *argv[3];

    if (setCGIEnvironment(request, server))
        return INTERNAL_SERVER_ERROR;

    std::string _path = _cwd + _file_path;

    if (!(argv[0] = ft::strdup(_cgi_path.c_str())))
        return INTERNAL_SERVER_ERROR;
    if (!(argv[1] = ft::strdup(_path.c_str())))
        return INTERNAL_SERVER_ERROR;
    argv[2] = NULL;

    int pip[2];

    if (pipe(pip) != 0)
        return 500;


//    std::ifstream fs(_path.c_str());
//    if (!fs.good())
//    {
//        std::cerr << "Error stream file not found" << std::endl;
//        return 500;
//    }
//    std::string text;
//    text.assign(std::istreambuf_iterator<char>(fs),
//                std::istreambuf_iterator<char>());
//    fs.close();
//    _req_body = text;


    _fd_file = ::open("cgi_tmp", O_CREAT | O_RDWR | O_TRUNC, 00755);
    if (_fd_file == -1)
        std::cout << "OPEN FAILED" << std::endl;

    pid_t pid = fork();

    if (pid == 0) {
        if (chdir(_path.substr(0, _path.find_last_of('/')).c_str()) == -1)
            return 500;
        close(pip[1]);
        if (dup2(pip[0], 0) == -1)
            return 500;
        if (dup2(_fd_file, 1) == -1)
            return 500;
        close(pip[0]);
        execve(argv[0], argv, _cgi_env);
        exit(1);
    }
    else if (pid > 0) {
        close(pip[0]);
        if (_req_body.length() && write(pip[1], _req_body.c_str(), _req_body.length()) <= 0)
            return 500;
        close(pip[1]);

        int status;

        if (waitpid(pid, &status, 0) == -1)
            return 500;
        if (WIFEXITED(status) && WEXITSTATUS(status))
            return 502;
    }
    else {
        return 502;
    }

    char buf[4096 + 1];
    int ret;

    lseek(_fd_file, 0, SEEK_SET);
    while ((ret = read(_fd_file, buf, 4096)) != 0) {
        if (ret == -1) {
            return 500;
        }
        buf[ret] = '\0';
        _ret_body.insert(_ret_body.length(), buf, ret);
    }
    // Here we remove HTTP EOF because the CGI we use cannot accept HTML in it.
    // If we send HTML inside the CGI he will TOUPPER the html which is.. shitty ?
    removeEOFHTTP(_ret_body);

    return (HTTP_OK);
}

void CGI::removeEOFHTTP(std::string &str)
{
    for (int i = 0; i < 2; i++)
    {
        std::string::iterator it = str.end();
        str.erase(--it);
    }
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
    _cgi_env = NULL;

    if (_headers.find("Auth-Scheme") != _headers.end())
        _env["AUTH_TYPE"] = _headers["Authorization"];

    if (_headers.find("Content-Length") != _headers.end())
        _env["CONTENT_LENGTH"] = _headers["Content-Length"];

    _env["CONTENT_TYPE"] = _headers["Content-Type"];
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = _cwd + _file_path;
    _env["REQUEST_URI"] = _cwd + _file_path;
    _env["PATH_TRANSLATED"] = _cwd + _file_path;

    _env["QUERY_STRING"] = request->getQuery();
//    _env["REMOTE_ADDR"] = server->getAddress();
    _env["REMOTE_HOST"] = "";

    //  if request required authentication using the "Basic" mechanism (AUTH_TYPE= "Basic") : value of the REMOTE_USER meta-variable is set to the user-ID supplied
    //  In all other cases the value of this meta-variable is undefined.
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
