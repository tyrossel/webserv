#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include "Server.hpp"
#include "Location.hpp"
#include "webserv.hpp"

class RequestParser;
class Server;
struct Location;

class Request {
    private:
        RequestType                         _method;
        std::string                         _path;
        std::string							_location;
        std::string                         _query;
        std::string                         _version;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;
        long                                _body_length;
        int                                 _status;
        friend class                        RequestParser;

    public:
        Request();
        Request(const Request &other);
        ~Request();
        Request &operator=(const Request &other);

        bool                                matchLocation(const std::string &path, const std::string &loc_path) const;
        const Location &                    FindLocation(const Server &server) const;
        const Server &						FindServer(const std::vector<Server> &servers, struct sockaddr_in req_addr) const;
        bool								isValid(const Location &loc);
        void								updatePathWithLocation(const Location &loc);

        /* GETTERS */
        std::map<std::string, std::string>  getHeaders() const;
        RequestType                         getMethod() const;
        std::string                         getPath() const;
        std::string                         getLocation() const;
        std::string                         getQuery() const;
        std::string                         getVersion() const;
        std::string                         getBody() const;
        long                                getBodyLength() const;
        int                                 getStatus() const;

        /* SETTERS */
        void                                setStatus(int new_status);
        void                                setPath(std::string new_path);
};

std::ostream &operator<<(std::ostream &out, const Request &rhs);

#endif //WEBSERV_REQUEST_HPP
