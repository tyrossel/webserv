# epoll_wait()
---
~~~cpp
#include <sys/epoll.h>

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
~~~
- Arguments : 
	- waits for events on the epoll instance referred to by the file descriptor **epfd**.  
	- **events** : buffer, is used to return information from the ready list about file descriptors in the interest list that have some events available.  
	- Up to **maxevents** are returned by it.
		- This argument must be greater than zero.
	- **timeout** : specifies the number of milliseconds that epoll_wait() will block.  

- A call to epoll_wait() will block until either:
	• a file descriptor delivers an event
	• the call is interrupted by a signal handler
	• the timeout expires.

- Returns :
	- Success : number of file descriptors
		- 0 if no file descriptor became ready during the requested timeout milliseconds.
    - On failure : -1 and errno is set to indicate the error.

- The struct **epoll_event** is defined as:
~~~cpp
typedef union epoll_data {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;    /* Epoll events */
    epoll_data_t data;      /* User data variable */
};
~~~
---
### [Source](https://man7.org/linux/man-pages/man2/epoll_wait.2.html)
---
## [[epoll - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]