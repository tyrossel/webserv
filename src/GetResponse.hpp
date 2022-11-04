#ifndef GETRESPONSE_HPP
#define GETRESPONSE_HPP

#include "ValidResponse.hpp"
#include <string>

class GetResponse : public ValidResponse
{
	public:
		GetResponse(const Location &loc, const Server &server, const Request &req);
		GetResponse(const GetResponse &);
		~GetResponse();

	private:

		void		buildResponse();
		void		buildGetBody();

};


#endif // GETRESPONSE_HPP
