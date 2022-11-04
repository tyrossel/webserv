#include "DeleteResponse.hpp"
#include "ValidResponse.hpp"
#include "CommonGatewayInterface.hpp"


DeleteResponse::DeleteResponse(const Location &loc, const Server &server, const Request &req)
	: ValidResponse(loc, server, req)
{
	buildResponse();
}

DeleteResponse::DeleteResponse(const DeleteResponse &rhs) :
	ValidResponse(rhs)
{
}

DeleteResponse::~DeleteResponse() {}

void DeleteResponse::buildResponse()
{
    std::string path = _req.getLocation();
    if (ft::isFile(path))
    {
        if (remove(path.c_str()) == 0)
            setStatus(HTTP_NO_CONTENT);
        else
            setStatus(FORBIDDEN);
    }
    else
        setStatus(NOT_FOUND);

    setContentType();

    printLog("Valid Delete");
}
