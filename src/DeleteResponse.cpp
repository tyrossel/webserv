#include "DeleteResponse.hpp"
#include "ValidResponse.hpp"
#include "CommonGatewayInterface.hpp"


DeleteResponse::DeleteResponse(const Location &loc, const Server &server, const Request &req)
	: ValidResponse(loc, server, req)
{
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
            return setError(HTTP_NO_CONTENT);
        else
            return setError(FORBIDDEN);
    }
    else
        return setError(NOT_FOUND);

    setContentType();

    printLog("Valid Delete");
}
