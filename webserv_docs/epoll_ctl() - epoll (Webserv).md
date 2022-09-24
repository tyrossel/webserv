# epoll_ctl()
---
~~~cpp
#include <sys/epoll.h>

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
~~~
- used to **add, modify, or remove entries in the interest list of the epoll instance** referred to by the file descriptor epfd.  
- It requests that the operation op be performed for the target file descriptor, fd.
- The struct epoll_event is defined as:
~~~cpp

typedef union epoll_data {
    void        *ptr;
	int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;      /* Epoll events */
    epoll_data_t data;        /* User data variable */
};
~~~
-   **data member** : specifies data that the kernel should save and then return (via epoll_wait when this file descriptor becomes ready)
-   **events member** : structure is a bit mask composed by zero or more of the following available event types
	- [[epoll_events - epoll_ctl (Webserv)]]

- Returns :
	- Success : returns zero.  
	- Error : -1 and errno is set to indicate the error.
---
### [Source](https://man7.org/linux/man-pages/man2/epoll_ctl.2.html)
---
## [[epoll - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]