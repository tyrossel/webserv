#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "DirectoryListing.hpp"

class Server;
class CGI;

class Response {
    private:
        Request                 _request;
        const Location          *_loc;
        Server                  *_server;
        long int                _socket;
        std::string             _response;

    public:
        // CONSTRUCTORS ====================================================
        Response();
        Response(const long int socket, const Location *loc, Server *server);
        ~Response();
        // =================================================================

        // CHECKERS ========================================================
        void     checkCode();
        void     checkPath();
        bool     secFetchImage();
        // =================================================================

        // TOOLS ========================================================
        void        addContentLengthPOST();
        void        addBodyToResponse();
        void        addDate();
        void        addServerHeaderResponse();
        void        addContentType();
        void        addHTTPHeader();
        void        writeResponseHeader();
        void        addErrorBodyToResponse();
        // =================================================================

        // BUILDERS ========================================================
        void        buildGetResponse(Request req);
        void        buildPostResponse(Request req);
        void        buildDeleteResponse(Request req);
        // =================================================================

        // GETTERS =========================================================
        std::string     getResponse() const;
        int             respSize() const;
        long int        getSocket() const;
        // =================================================================

};

#endif
