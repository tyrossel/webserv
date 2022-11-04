#include "GetResponse.hpp"
#include "Utils.hpp"
#include "ValidResponse.hpp"
#include "CommonGatewayInterface.hpp"
#include "webserv.hpp"


GetResponse::GetResponse(const Location &loc, const Server &server, const Request &req)
	: ValidResponse(loc, server, req)
{
}

GetResponse::GetResponse(const GetResponse &rhs) :
	ValidResponse(rhs)
{
}

GetResponse::~GetResponse() {}

void GetResponse::buildGetBody()
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
	setBody(text);
}

void GetResponse::buildResponse()
{
	std::string loc = _req.getLocation();
    if (useCGI())
    {
        if (ft::isFile(loc) || ft::isDirectory(loc)) {
            CGI cgi(_req);
            setStatus(cgi.executeCgi(&_req, _server, _loc));
            setBody(cgi.getRetBody());
        }
        else
			return setError(NOT_FOUND);
        printLog("CGI Get");
    }
    else
    {
		if (!ft::isDirectory(_req.getLocation()) && !ft::isFile(_req.getLocation()))
		{
			return setError(NOT_FOUND);
		}

        setContentType();
        if (ft::isOkHTTP(getStatus()))
            buildGetBody();
        else
			return setError(getStatus());
        printLog("Valid Get");
    }
}
