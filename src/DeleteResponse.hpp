#ifndef DELETERESPONSE_HPP
#define DELETERESPONSE_HPP

#include "ValidResponse.hpp"
#include <string>

class DeleteResponse : public ValidResponse
{
	public:
		DeleteResponse(const Location &loc, const Server &server, const Request &req);
		DeleteResponse(const DeleteResponse &);
		~DeleteResponse();

	private:

		void		buildResponse();

};


#endif // DELETERESPONSE_HPP
