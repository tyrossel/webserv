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
        int                     _status;

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
		bool	 useCGI();
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
        void        setStatus(int new_status);
        // =================================================================

        // BUILDERS ========================================================
        void        buildGetResponse(Request req, const Location *loc);
        void        buildPostResponse(Request req, const Location *loc);
        void        buildDeleteResponse(Request req);
        // =================================================================

        // GETTERS =========================================================
        std::string     getResponse() const;
        int             respSize() const;
        long int        getSocket() const;
        int             getStatus() const;
        // =================================================================

};

#endif
