#ifndef WEBSERV_VALIDRESPONSE_HPP
#define WEBSERV_VALIDRESPONSE_HPP

#include "Location.hpp"
#include "webserv.hpp"
#include "Request.hpp"
#include "DirectoryListing.hpp"
#include "Response.hpp"

class Server;
class CGI;

class ValidResponse : public Response {
    private:
        const Location          &_loc;
        const Server            &_server;
		const Request			&_req;

    public:
        // CONSTRUCTORS ====================================================
        ValidResponse(const Location &loc, const Server &server, const Request &req);
		ValidResponse(const ValidResponse &rhs);
        ~ValidResponse();
		ValidResponse &operator=(const ValidResponse &rhs);
        // =================================================================

		std::string	buildResponse();
        void        printLog(const std::string &title);

	protected:
        // CHECKERS ========================================================
        void     checkPath();
        bool     secFetchImage();
		bool	 useCGI();
        // =================================================================

        // TOOLS ========================================================
        void        addContentLengthCGI(CGI &cgi);
        void        addBodyToResponse();
        void        AddErrorBodyToResponse();
        void        addContentType();
        void        addHTTPHeader(bool checkPath = true);
        // =================================================================

        // BUILDERS ========================================================
		void		buildRedirectionResponse(const Redirection &redir);
        void        buildGetResponse();
        void        buildPostResponse();
        void        buildDeleteResponse();
        // =================================================================

};

#endif
