#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "../includes/webserv.hpp"

class Handler {
public:
    Handler();
    ~Handler();

    virtual int handle(epoll_event e);
};

#endif // HANDLER_HPP
