#ifndef REDIRRESPONSE_HPP
#define REDIRRESPONSE_HPP

#include "webserv.hpp"
#include "ValidResponse.hpp"

class RedirResponse : public Response {
    private:
		const Location			&_loc;
		const Request 			&_req;
		const Redirection		&_redir;

    public:
        // CONSTRUCTORS ====================================================
        RedirResponse(const Location &loc, const Request &req, const Redirection &redir);
		RedirResponse(const RedirResponse &rhs);
        ~RedirResponse();
		RedirResponse &operator=(const RedirResponse &rhs);
        // =================================================================

		std::string	buildResponse();
        void        printLog(const std::string &title);

	private:

        void        addBodyToResponse();
        void        AddErrorBodyToResponse();

};

#endif // !REDIRRESPONSE_HPP
