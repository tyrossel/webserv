#ifndef WEBSERV_ERRORRESPONSE_HPP
#define WEBSERV_ERRORRESPONSE_HPP

#include "Response.hpp"
#include "Location.hpp"
#include "webserv.hpp"
#include <string>

class ErrorResponse : public Response {

	private:

		const Location *		_loc;
		std::string		_custom_file;
        bool            _close;
		bool			_print;

    public:
        // CONSTRUCTORS ====================================================
        ErrorResponse(int status = 500, bool close=false, const Location *loc = NULL);
		ErrorResponse(const ErrorResponse &rhs);
        ~ErrorResponse();
		ErrorResponse &operator=(const ErrorResponse &rhs);
        // =================================================================

        // TOOLS ========================================================

        void		printLog(const std::string &title);
		void		setPrint(bool print = true);

        // BUILDERS ========================================================

		void		buildResponse();

	private:

        void		addBodyToResponse();

        // =================================================================

};

std::ostream &operator<<(std::ostream &os, const ErrorResponse &rhs);

#endif
