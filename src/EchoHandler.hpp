#ifndef ECHOHANDLER_HPP
#define ECHOHANDLER_HPP

#include "Handler.hpp"

class EchoHandler: Handler {
    private:
        int         _fd;
        size_t      _received;
        std::string _buffer;

    public:
        EchoHandler(int fd_client);
        ~EchoHandler();

        virtual int handle(epoll_event e);
};
#endif // ECHOHANDLER_HPP
