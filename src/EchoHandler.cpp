#include "EchoHandler.hpp"

/**************************************************************************************/
/*                            Constructor/Destructor                                  */
/**************************************************************************************/
EchoHandler::EchoHandler(int fd_client) : _fd(fd_client) {}

~EchoHandler::EchoHandler() {}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/
int EchoHandler::handle(epoll_event e)
{
    if (e.events & EPOLLHUP) {
        IOLoop::getInstance()->removeHandler(_fd);
        return -1;
    }

    if (e.events & EPOLLERR) {
        return -1;
    }

    if (e.events & EPOLLOUT) {
        if (received > 0) {
            cout << "Writing: " << _buffer << endl;
            if (send(_fd, _buffer, _received, 0) != _received) {
                std::cerr << ("Error writing to socket") << std::endl;
            }
        }

        IOLoop::getInstance()->modifyHandler(_fd, EPOLLIN);
    }

    if (e.events & EPOLLIN) {
        if ((_received = recv(_fd, _buffer, BUFFER_SIZE, 0)) < 0) {
            std::cerr << ("Error reading from socket") << std::endl;
        } else if(_received > 0) {
            _buffer[_received] = 0;
            cout << "Reading: " << _buffer << endl;
        }

        if (received > 0) {
            IOLoop::getInstance()->modifyHandler(_fd, EPOLLOUT);
        } else {
            IOLoop::getInstance()->removeHandler(_fd);
        }
    }
    return 0;
}

