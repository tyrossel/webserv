#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "webserv.hpp"

class Response {

    private:

        int                     _status;

	protected:

        std::map<std::string, std::string>  _mime_map;
        std::string                         _body;

		std::map<std::string, std::string>  _headers;

    public:

        Response(int status = 0);
		Response(const Response &rhs);
        virtual ~Response();
		Response &operator=(const Response &rhs);

        // TOOLS ========================================================
        virtual void		printLog(const std::string &title) = 0;

        // BUILDERS ========================================================
		virtual void buildResponse() = 0;


		std::string	to_string() const;

        // GETTERS & SETTERS ===============================================

		std::string			getHeader(const std::string &key) const;
        int					getStatus() const;
        std::string			getBody() const;
        int					respSize() const;

	protected:
		std::string			headersToString() const;
        void				setBody(const std::string &str);
        void				setStatus(int new_status);
		void				setHeader(const std::string &key, const std::string &value);
		void				setError(int status);

	private:

        void				writeResponseHeader(std::string &txt) const;

};

#endif
