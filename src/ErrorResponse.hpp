#ifndef WEBSERV_ERRORRESPONSE_HPP
#define WEBSERV_ERRORRESPONSE_HPP

#include "Response.hpp"
#include "webserv.hpp"
#include <string>

class ErrorResponse : public Response {

	private:

		std::string		_custom_file;

    public:
        // CONSTRUCTORS ====================================================
        ErrorResponse(int status = 500, const std::string &custom_file = "");
		ErrorResponse(const ErrorResponse &rhs);
        ~ErrorResponse();
		ErrorResponse &operator=(const ErrorResponse &rhs);
        // =================================================================

        // TOOLS ========================================================

        void		printLog(const std::string &title);

        // BUILDERS ========================================================

		std::string buildResponse();

	private:

        void		addBodyToResponse();

        // =================================================================

};

std::ostream &operator<<(std::ostream &os, const ErrorResponse &rhs);

#endif
