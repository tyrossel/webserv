# select
---
~~~cpp
#include <sys/select.h>

int select(int nfds, fd_set *restrict readfds,
                  fd_set *restrict writefds, fd_set *restrict 
                  exceptfds, struct timeval *restrict timeout);

void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);
~~~
- can **monitor only file descriptors numbers that are less than FD_SETSIZE (1024)** an unreasonably low limit for many modern applications
- All modern applications **should instead use poll(2) or epoll(7), which do not suffer this limitation.**
- allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation
- A file descriptor is considered ready if it is possible to perform a corresponding I/O operation
---
- **FD_ZERO()**
	- This macro clears (removes all file descriptors from) set.
	- It should be employed as the first step in initializing a file descriptor set.

- **FD_SET()**
	- This macro adds the file descriptor fd to set.  Adding a file descriptor that is already present in the set is a no-op, and does not produce an error.

- **FD_CLR()**
	- This macro removes the file descriptor fd from set.
    - Removing a file descriptor that is not present in the set is a no-op, and does not produce an error.

- **FD_ISSET()**
	- select() modifies the contents of the sets according to the rules described below.  
	- After calling select(), the FD_ISSET() macro can be used to test if a file descriptor is still present in a set.  
	- FD_ISSET() returns nonzero if the file descriptor fd is present in set, and zero if it
---
## [[Functions - Webserv (CPP)]]
## [[Webserv]]