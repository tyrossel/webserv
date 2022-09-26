# epoll
---
~~~cpp
#include <sys/epoll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
~~~
- performs a similar task to [[poll() - Webserv]] : monitoring multiple file descriptors to see if I/O is possible on any of them.  
- epoll API can be used either as an edge-triggered or a level-triggered interface and scales well to large numbers of watched file descriptors.
- central concept of the epoll API is the epoll instance, an in-kernel data structure which, from a user-space perspective, can be considered as a container for two lists:
	• **interest list** (sometimes also called the epoll set): the set of file descriptors that the process has registered an interest in monitoring.
	• **ready list** : the set of file descriptors that are "ready" for I/O.  The ready list is a subset of the file descriptors in the interest list.  
---
The following system calls are provided to create and manage an epoll instance :
	• epoll_create, epoll_ctl, epoll_wait

---
## [[epoll_create() - epoll (Webserv)]]
## [[epoll_ctl() - epoll (Webserv)]]
## [[epoll_wait() - epoll (Webserv)]]
---
### [Source IMPORTANT](https://man7.org/linux/man-pages/man7/epoll.7.html)
---
## [[Functions - Webserv (CPP)]]
## [[Webserv]]