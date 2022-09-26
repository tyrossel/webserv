#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include "Handler.hpp"

class ServerHandler : Handler {
    private:
        int _port;
        int _fd;

    public:
        ServerHandler(int port);
        ~ServerHandler();

        virtual int handle(epoll_event e);
};

#endif // SERVERHANDLER_HPP
