#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"

class Server {
    private:
        int             _port;
        long            _fd;
        unsigned int    _host;
        sockaddr_in     _addr;

    public:
        Server();
        Server(int port, std::string host);
        Server(const Server &other);
        ~Server();

        void    log(std::string message);
        long    createSocket(); //Create and set socket
        void    setAddress();   //Set address
        int     setupListen(); // Bind and listen socket
        int     buildServer();
        void    close(int socket);
        int     send(long socket, std::map<long, std::string> response);
        int     getPort();
        long    getFd();

};

#endif // SERVER_HPP
