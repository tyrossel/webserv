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
    protected:
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

		virtual void		buildResponse() = 0;
        virtual void        printLog(const std::string &title);

	protected:
        // CHECKERS ========================================================
		bool	 useCGI();
        // =================================================================

        // TOOLS ========================================================
        void        addContentLengthCGI(CGI &cgi);
        void        setContentType();
        // =================================================================

        // BUILDERS ========================================================
        void        buildPostResponse();
        void        buildDeleteResponse();
        // =================================================================
		//
		void		setError(int status);

};

#endif
