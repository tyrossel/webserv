#include "IOLoop.hpp"

IOLoop::IOLoop() : _epfd(), _events(), _handlers() {
    this->_epfd = epoll_create(EPOLL_EVENTS);

    if (this->_epfd < 0) {
        std::cerr << ("Failed to create epoll") << std::endl;
        exit(1);
    }
}

IOLoop::~IOLoop() {}

static IOLoop *IOLoop::getInstance()
{
    static IOLoop instance;
    return &instance;
}

void IOLoop::start()
{
    while(1)
    {
        int nfds = epoll_wait(this->_epfd, this->_events, MAX_EVENTS, -1 /* Timeout */);

        for (int i = 0; i < nfds; ++i)
        {
            int fd = this->events[i].data.fd;
            Handler *h = handlers[fd];
            h->handle(this->events[i]);
        }
    }
}

void IOLoop::addHandler(int fd, Handler *handler, unsigned int events)
{
    handlers[fd] = handler;
    epoll_event e;
    e.data.fd = fd;
    e.events = events;

    if(epoll_ctl(this->epfd, EPOLL_CTL_ADD, fd, &e) < 0) {
        std::cerr << ("Failed to insert handler to epoll") << std::endl;
    }
}

void IOLoop::modifyHandler(int fd, unsigned int events)
{
    epoll_event e;
    e.data.fd = fd;
    e.events = events;

    if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, fd, &e) == -1) {
        std::cerr << "Delete using epoll_ctl failed" << std::endl;
        exit(1);
    }
}

void IOLoop::removeHandler(int fd)
{
    if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Delete using epoll_ctl failed" << std::endl;
        exit(1);
    }
}
