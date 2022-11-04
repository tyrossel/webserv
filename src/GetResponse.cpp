#include "GetResponse.hpp"
#include "ValidResponse.hpp"
#include "CommonGatewayInterface.hpp"


GetResponse::GetResponse(const Location &loc, const Server &server, const Request &req)
	: ValidResponse(loc, server, req)
{
	buildResponse();
}

GetResponse::GetResponse(const GetResponse &rhs) :
	ValidResponse(rhs)
{
}

GetResponse::~GetResponse() {}

void GetResponse::buildResponse()
{
    if (useCGI())
    {
        if (ft::isFile(_req.getLocation())) {
            CGI cgi(_req);
            setStatus(cgi.executeCgi(&_req, _server, _loc));
            setBody(cgi.getRetBody());
        }
        else
			setError(NOT_FOUND);
        printLog("CGI Get");
    }
    else
    {
		if (!ft::isDirectory(_req.getLocation()) && !ft::isFile(_req.getLocation()))
			return setError(NOT_FOUND);

        setContentType();
        if (ft::isOkHTTP(getStatus()))
            buildGetBody();
        else
			setError(getStatus());
        printLog("Valid Get");
    }
}
