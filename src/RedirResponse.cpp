#include "RedirResponse.hpp"
#include "ErrorResponse.hpp"
#include "ValidResponse.hpp"

/**************************************************************************************/
/*                                  BUILDERS                                          */
/**************************************************************************************/

RedirResponse::RedirResponse(const Location &loc, const Request &req, const Redirection &redir) :
	Response(301), _loc(loc), _req(req), _redir(redir)
{
}

RedirResponse::~RedirResponse()
{
}

std::string	RedirResponse::buildResponse()
{
	if (_redir.status == 0)
		setStatus(HTTP_FOUND);
	else
		setStatus(_redir.status);
	Response::addHTTPHeader();
	if (getStatus() / 100 == 3)
	{
		_response.append("Location: " + _redir.new_url + "\r\n");
		_response.append("Connection: keep-alive\r\n");
		AddErrorBodyToResponse();
	}
	else
	{
		ErrorResponse err(getStatus());
		_response = err.buildResponse();
	}
	if (VERBOSE)
	{
		std::cout << "================== RESPONSE ==================" << std::endl;
		std::cout << YELLOW << ft::timestamp(TIMESTAMP_FORMAT) << RESET << std::endl;
		std::cout << GREEN << _response << RESET << std::endl;
		std::cout << "==============================================" << std::endl << std::endl;
	}
	printLog("Redirection");
	return _response;
}

/**************************************************************************************/
/*                                  GETTERS                                           */
/**************************************************************************************/

void	RedirResponse::printLog(const std::string &title)
{
	if (VERBOSE)
	{
		std::cout << "================== " + title + " ==================" << std::endl;
		std::cout << ft::timestamp(TIMESTAMP_FORMAT) << std::endl;
		std::cout << CYAN << getResponse() << RESET << std::endl;
	}
	else
	{
        if (getStatus() >= BAD_REQUEST)
            std::cout << CYAN << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
        else
            std::cout << CYAN << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
	}
}

void RedirResponse::addBodyToResponse()
{
    std::string text;
    std::stringstream out;
    std::string loc = _req.getLocation();
    std::string path = _req.getPath();

    if (ft::isDirectory(loc))
    {
		if (_loc.auto_index)
			text = createDirectoryListingBody(path, loc);
		else
		{
			setStatus(FORBIDDEN);
			AddErrorBodyToResponse();
			return ;
		}
    }
    else
    {
        try
        {
            text = ft::readFile(loc);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
			ErrorResponse err(FORBIDDEN);
			_response = err.buildResponse();
			return ;
        }
    }
    out << text.size();
    std::string content_len = "Content-Length: " + out.str();
    _response.append(content_len);
    _response.append("\r\n\r\n");
    _response.append(text);
}

void RedirResponse::AddErrorBodyToResponse()
{
    std::string body;
    std::stringstream out;

	try
	{
		body.append(ft::readFile(_loc.error_pages.at(getStatus())));
	}
	catch (const std::exception& e)
	{
		body.append(ft::craftErrorHTML(getStatus()));
	}

    _response.append("Content-Length: ");
    _response.append(ft::itoa(body.size()));
    _response.append("\r\n\r\n");
    _response.append(body);
}
