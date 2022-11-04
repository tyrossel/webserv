#include "RedirResponse.hpp"
#include "ErrorResponse.hpp"
#include "ValidResponse.hpp"

/**************************************************************************************/
/*                                  BUILDERS                                          */
/**************************************************************************************/

RedirResponse::RedirResponse(const Location &loc, const Request &req, const Redirection &redir) :
	Response(301), _loc(loc), _req(req), _redir(redir)
{
	buildResponse();
}

RedirResponse::~RedirResponse()
{
}

void	RedirResponse::buildResponse()
{
	if (_redir.status == 0)
		setStatus(HTTP_FOUND);
	else
		setStatus(_redir.status);
	if (getStatus() / 100 == 3)
	{
		setHeader("Location", _redir.new_url);
		setHeader("Connection", "keep-alive");
		setError(getStatus());
	}
	else
	{
		setError(getStatus());
	}

	printLog("Redirection");
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
		std::cout << CYAN << to_string() << RESET << std::endl;
	}
	else
	{
        if (getStatus() >= BAD_REQUEST)
            std::cout << CYAN << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
        else
            std::cout << CYAN << ft::timestamp(TIMESTAMP_FORMAT) << "Response code [" << getStatus() << "]" << RESET << std::endl;
	}
}

void RedirResponse::buildBody()
{
    std::string text;
    std::string loc = _req.getLocation();
    std::string path = _req.getPath();

    if (ft::isDirectory(loc))
    {
		if (_loc.auto_index)
			text = createDirectoryListingBody(path, loc);
		else
		{
			return setError(FORBIDDEN);
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
			return setError(FORBIDDEN);
        }
    }
}

void RedirResponse::setError(int status)
{
	std::map<int, std::string>::const_iterator it = _loc.error_pages.find(status);
	std::string custom_page;

	if (it == _loc.error_pages.end())
		custom_page = it->second;

	ErrorResponse err(status, false, &_loc);
	setBody(err.getBody());
	setStatus(status);
}
