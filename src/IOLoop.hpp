#ifndef IOLOOP_HPP
#define IOLOOP_HPP

#include "Handler.hpp"

class IOLoop {
    private:
        int         _epfd;
        epoll_event *_events;
        Handler      _handlers[10];

    public:
        IOLoop();
        ~IOLoop();

        static IOLoop   *getInstance();
        void            start();

        void            addHandler(int fd, Handler *handler, unsigned int events);
        void            modifyHandler(int fd, unsigned int events);
        void            removeHandler(int fd);
};

#endif // IOLOOP_HPP
