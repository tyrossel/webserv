#include "PostResponse.hpp"
#include "ValidResponse.hpp"
#include "CommonGatewayInterface.hpp"


PostResponse::PostResponse(const Location &loc, const Server &server, const Request &req)
	: ValidResponse(loc, server, req)
{
}

PostResponse::PostResponse(const PostResponse &rhs) :
	ValidResponse(rhs)
{
}

PostResponse::~PostResponse() {}

void PostResponse::buildResponse()
{
    if (useCGI()) {
        CGI cgi(_req);

        setStatus(cgi.executeCgi(&_req, _server, _loc));
		setBody(cgi.getRetBody());
        printLog("CGI Post");
    }
    else {
        if (!ft::isFile(_req.getLocation()))
            return setError(NOT_FOUND);
        else if (ft::writeFile(_req.getLocation(), _req.getBody()) == -1)
            return setError(FORBIDDEN);

        printLog("Valid Post");
    }
}
