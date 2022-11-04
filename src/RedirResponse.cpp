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
		setError(getStatus(), false);
	}
	else
	{
		return setError(getStatus(), true);
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

void RedirResponse::setError(int status, bool print)
{
	std::map<int, std::string>::const_iterator it = _loc.error_pages.find(status);
	std::string custom_page;

	if (it != _loc.error_pages.end())
		custom_page = it->second;

	ErrorResponse err(status, false, &_loc);
	err.setPrint(print);
	err.buildResponse();
	setBody(err.getBody());
	setStatus(status);
}
