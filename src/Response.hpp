#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "webserv.hpp"

class Response {

    private:

        int                     _status;

	protected:

        std::string             _response;

    public:

        Response(int status = 0);
		Response(const Response &rhs);
        virtual ~Response();
		Response &operator=(const Response &rhs);

        // TOOLS ========================================================
        virtual void		printLog(const std::string &title) = 0;

        // BUILDERS ========================================================
		virtual std::string buildResponse() = 0;

        // GETTERS & SETTERS ===============================================
        std::string			getResponse() const;
        int					respSize() const;
        int					getStatus() const;
        void				setStatus(int new_status);

	protected:

        void				addHTTPHeader();

	private:

        void				addServerHeaderResponse();
        void				addDate();
        void				writeResponseHeader();

};

#endif
