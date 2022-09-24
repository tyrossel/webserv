# epoll_events
---
## EPOLLIN
- The associated file is available for read operations.

## EPOLLOUT
- The associated file is available for write operations.

## EPOLLRDHUP 
- Stream socket peer closed connection, or shut down writing half of connection.  (This flag is especially useful for writing simple code to detect peer shutdown when using edge-triggered monitoring.)

## EPOLLPRI
- There is an exceptional condition on the file descriptor.

## EPOLLERR
- Error condition happened on the associated file descriptor.  This event is also reported for the write end of a pipe when the read end has been closed.
- epoll_wait will always report for this event; it is not necessary to set it in events when calling epoll_ctl().

## EPOLLHUP
- Hang up happened on the associated file descriptor.
- epoll_wait will always wait for this event; it is not necessary to set it in events when calling epoll_ctl().

## [And more ...](https://man7.org/linux/man-pages/man2/epoll_ctl.2.html)
---
## [[epoll_ctl() - epoll (Webserv)]]
## [[epoll - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]