#include "Response.hpp"
#include "CommonGatewayInterface.hpp"

/**************************************************************************************/
/*                                  CONSTRUCTOR / DESTRUCTOR                          */
/**************************************************************************************/
Response::Response() {}

Response::Response(const long int socket, const Location *loc, Server *server): _loc(loc), _server(server), _socket(socket) {}

Response::~Response() {}
/**************************************************************************************/
/*                                  TOOLS                                             */
/**************************************************************************************/
void Response::addContentLengthPOST()
{
    _response.append("Content-Length: ");
    _response.append(_request.getHeaders().find("Content-Length")->second);
    _response.append("\r\n");
}

void Response::addBodyToResponse() // TODO: add file to read from (std::string path)
{
    std::string text;
    std::stringstream out;
    std::string loc = _request.getLocation();
    std::string path = _request.getPath();

    if (ft::isDirectory(loc))
    {
        text = createDirectoryListingBody(path, loc);
    }
    else
    {
        try
        {
            text = ft::readFile(loc);
        }
        catch (const std::exception& e)
        {
            _response.append("\r\n\r\n");
            std::cerr << e.what() << std::endl; // TODO: This is very heavy for a common case..
        }
    }
    out << text.size();
    std::string content_len = "Content-Length: " + out.str();
    _response.append(content_len);
    _response.append("\r\n\r\n");
    _response.append(text);
}

void Response::addDate()
{
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char* dt = ctime(&now);
    // convert now to tm struct for UTC
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    _response.append("Date: ");
    _response.append(dt);
}

void Response::addServerHeaderResponse()
{
    _response.append("Server: WetServ/1.0.0\r\n");
}

void Response::addContentType()
{
    _response.append("Content-Type: ");
    std::map<std::string, std::string> tmp = _request.getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
        _response.append(tmp.find("Sec-Fetch-Dest")->second);
    else
        _response.append("NONE");
    _response.append("\r\n");
}

void Response::addHTTPHeader()
{
    checkCode(); // checking if an error code has been parsed in request
    checkPath(); // functions will return the code catched
    writeResponseHeader();
}

void Response::writeResponseHeader()
{
    std::stringstream out;

    out << _request.getStatus();
    _response.append("HTTP/1.1 ");
    if (ft::isOkHTTP(_request.getStatus())) {
        _response.append(out.str());
        _response.append(" OK\r\n");
    }
    else {
        _response.append(out.str());
        _response.append(" KO\r\n");
    }
}

void Response::addErrorBodyToResponse()
{
    std::string body;
    std::stringstream out;

    if (!_loc)
    {
        body.append(ft::craftErrorHTML(_request.getStatus()));
    }
    else
    {
        try
        {
            body.append(ft::readFile(_loc->error_pages.at(_request.getStatus())));
        }
        catch (const std::exception& e)
        {
            body.append(ft::craftErrorHTML(_request.getStatus()));
        }
    }

    _response.append("Content-Length: ");
    _response.append(ft::itoa(body.size()));
    _response.append("\r\n\r\n");
    _response.append(body);
}

/**************************************************************************************/
/*                                  CHECKERS                                          */
/**************************************************************************************/

void    Response::checkCode()
{
    if (_request.getStatus() == 0)
        _request.setStatus(HTTP_OK);
}

void    Response::checkPath()
{
    std::string path = _request.getLocation();

    ft::trimLeft(path, "/");
    if (ft::isDirectory(path) || ft::isFile(path))
        _request.setStatus(HTTP_OK);
    else
        _request.setStatus(NOT_FOUND);
}

bool    Response::secFetchImage()
{
    // This function just checks if the _request is for an image
    std::map<std::string, std::string> tmp = _request.getHeaders();
    if (tmp.find("Sec-Fetch-Dest") != tmp.end())
    {
        if (tmp.find("Sec-Fetch-Dest")->second == "image")
            return (false);
    }
    return (true);
}

/**************************************************************************************/
/*                                  BUILDERS                                          */
/**************************************************************************************/

void Response::buildGetResponse(Request req)
{
    this->_request = req;
    addHTTPHeader();
    addServerHeaderResponse();
    addDate();

    if (0) // CGI or not ?
    {
        std::string path = _request.getLocation();
        if (path[0] == '/')
            ft::trimLeft(path, "/");
        if (ft::isFile(path)) {
            CGI cgi(_request);
            cgi.executeCgi(&_request, _server); //TODO: do smthing with the return !
            // Here we remove HTTP EOF because the CGI we use cannot accept HTML in it.
            // If we send HTML inside the CGI he will TOUPPER the html which is.. shitty ?
            cgi.removeEOFHTTP();
            _response.append(cgi.getRetBody());
            if (ft::isOkHTTP(_request.getStatus()))
                addBodyToResponse();
            else
                addErrorBodyToResponse();
        }
        else
            addErrorBodyToResponse();
        if (VERBOSE) {
            std::cout << "================== CGI ==================" << std::endl;
            if (secFetchImage())
                std::cout << _response << std::endl;
            else
                std::cout << GREEN << "We sent an image" << RESET << std::endl;
            std::cout << "==============================================" << std::endl << std::endl;
        }
    }
    else
    {
        //addContentType(socket); // TODO: Mime if no CGI
        if (ft::isOkHTTP(_request.getStatus()))
            addBodyToResponse();
        else
            addErrorBodyToResponse();
        if (VERBOSE) {
            std::cout << "================== RESPONSE ==================" << std::endl;
            if (secFetchImage())
                std::cout << GREEN << _response << RESET << std::endl;
            else
                std::cout << GREEN << "We sent an image" << RESET << std::endl;
            std::cout << "==============================================" << std::endl << std::endl;
        }
    }
}

void Response::buildPostResponse(Request req)
{
    this->_request = req;
    addHTTPHeader();
    addServerHeaderResponse();
    addDate();
    addContentLengthPOST();
    CGI cgi(_request);
    cgi.executeCgi(&_request, _server); //TODO & ? and do smthing with execute return !
    _response.append(cgi.getRetBody());
    if (VERBOSE) {
        std::cout << "================== CGI ==================" << std::endl;
        if (secFetchImage())
            std::cout << _response << std::endl;
        else
            std::cout << GREEN << "We sent an image" << RESET << std::endl;
        std::cout << "==============================================" << std::endl << std::endl;
    }
}

void Response::buildDeleteResponse(Request req)
{
    this->_request = req;

    std::string path = _request.getLocation();
    if (path[0] == '/')
        ft::trimLeft(path, "/");
    if (ft::isFile(path))
    {
        if (remove(path.c_str()) == 0)
            _request.setStatus(204);
        else
            _request.setStatus(403);
    }
    else
        _request.setStatus(404);
    addHTTPHeader();
    addServerHeaderResponse();
    addContentType();
    addDate();
    if (ft::isOkHTTP(_request.getStatus()))
        addBodyToResponse();
    else
        addErrorBodyToResponse();

    if (VERBOSE) {
        std::cout << "================== RESPONSE ==================" << std::endl;
        if (secFetchImage())
            std::cout << GREEN << _response << RESET << std::endl;
        else
            std::cout << GREEN << "We sent an image" << RESET << std::endl;
        std::cout << "==============================================" << std::endl << std::endl;
    }
}

// GETTERS

std::string Response::getResponse() const { return (this->_response); }
int         Response::respSize() const { return ((int)this->_response.size()); }
long int    Response::getSocket() const { return (this->_socket); }
