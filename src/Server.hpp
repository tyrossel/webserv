#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"

class Server {
    private:
        int             _port;
        int             _sock;
        unsigned int    _host;
        sockaddr_in     _addr;

    public:
        Server();
        Server(int port, std::string host);
        Server(const Server &other);
        ~Server();

        void    log(std::string message);
        void    createSocket(); //Create and set socket
        void    setAddress();   //Set address
        void    setupSocket(); // Bind and listen socket
        void    buildServer();

        int     getPort();
        int     getSock();

};

#endif // SERVERHPP