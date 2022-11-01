#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include "Location.hpp"
#include <stdexcept>

class Response;

class Server {
    private:
        int                                 _port;
        long                                _fd;
        std::string							_address;
        std::vector<std::string>            _name;
        std::map<std::string, Location>     _location;
        sockaddr_in                         _addr;

    public:
        Server();
        Server(const Server &other);
		Server &operator=(const Server &rhs);
        ~Server();

        long    createSocket();
        void    setAddress();
        int     setupListen();
        int     buildServer();
        void    close(int socket);
        int     send(int socket, const Response &Resp);

        // GETTERS =========================================================
        int                                         getPort() const;
        long                                        getFd() const;
        std::string									getAddress() const;
        std::vector<std::string>                    getName() const;
        const std::map<std::string, Location>       &getLocations() const;
        // =================================================================

        // ADDERS ==========================================================
        void                                        addPort(int port);
		void										addAddress(const std::string &address);
        void                                        addLocation(const std::string &key, const Location &location);
        void                                        addName(const std::string &name);
        // =================================================================

};

#endif // SERVER_HPP
