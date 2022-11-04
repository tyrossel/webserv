#ifndef POSTRESPONSE_HPP
#define POSTRESPONSE_HPP

#include "ValidResponse.hpp"
#include <string>

class PostResponse : public ValidResponse
{
	public:
		PostResponse(const Location &loc, const Server &server, const Request &req);
		PostResponse(const PostResponse &);
		~PostResponse();

	private:

		void		buildResponse();

};


#endif // POSTRESPONSE_HPP
