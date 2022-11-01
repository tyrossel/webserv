#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "Location.hpp"
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
        Response(const long int socket);
        Response(const long int socket, const Location *loc, Server *server, int status);
        ~Response();
        // =================================================================

        // CHECKERS ========================================================
        void     checkPath();
        bool     secFetchImage();
		bool	 useCGI();
        // =================================================================

        // TOOLS ========================================================
        void        printLog(bool print_cgi);
        void        addContentLengthPOST(CGI &cgi);
        void        addBodyToResponse();
        void        addDate();
        void        addServerHeaderResponse();
        void        addContentType();
        void        addHTTPHeader(bool checkPath = true);
        void        writeResponseHeader();
        void        addErrorBodyToResponse();
        void        setStatus(int new_status);
        void        addCGIHeader(CGI &cgi);
        // =================================================================

        // BUILDERS ========================================================
		void		buildRedirectionResponse(const Redirection &redir);
        void        buildGetResponse(Request req, const Location *loc);
        void        buildPostResponse(Request req, const Location *loc);
        void        buildDeleteResponse(Request req);
		void		buildErrorResponse(int errorCode);
        // =================================================================

        // GETTERS =========================================================
        std::string     getResponse() const;
        int             respSize() const;
        long int        getSocket() const;
        int             getStatus() const;
        // =================================================================

};

#endif
