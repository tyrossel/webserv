#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include "Location.hpp"
#include <stdexcept>

class Response;

class Server {
    private:
        int                                 _port; // where the server is listening
		std::string							_address; // IP address where the server is listening
        long                                _fd;
        unsigned int                        _host;
        std::vector<std::string>            _name;  //🏠🏠🏠🏠🏠🏠🏠🏠🏠
        std::map<std::string, Location>     _location;
        sockaddr_in                         _addr;

    public:
        Server();
        Server(const Server &other);
		Server &operator=(const Server &rhs);
        ~Server();

        void    log(std::string message);
        long    createSocket(); //Create and set socket
        void    setAddress();   //Set address
        int     setupListen(); // Bind and listen socket
        int     buildServer();
        void    close(int socket);
        int     send(const Response &Resp);

        // GETTERS
        long                                        getFd() const;
		std::string									getAddress() const;
        int                                         getPort() const; // Use the const one, remove the not const
        std::vector<std::string>                    getName() const;
		std::string									getCGIBin() const;
        std::vector<std::string>                    getCGIExtensions() const;
        std::string									getRoot() const;
        std::vector<std::string>                    getIndex() const;
        const std::map<std::string, Location>       &getLocations() const;
        unsigned int                                getHost() const;


        // ADDERS
        void                                        addPort(int port);
		void										addAddress(const std::string &address);
        void                                        addLocation(const std::string &key, const Location &location);
        void                                        addHost(const unsigned int &host);
        void                                        addName(const std::string &name);


};

#endif // SERVER_HPP
