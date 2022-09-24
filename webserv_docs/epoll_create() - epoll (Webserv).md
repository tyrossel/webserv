# epoll_create()
---
~~~cpp
#include <sys/epoll.h>

int epoll_create(int size);
~~~
- creates a new epoll instance
- **returns a file descriptor referring to the new epoll instance.** 
- This file descriptor is used for all the subsequent calls to the epoll interface.
- When no longer required, the file descriptor returned by epoll_create() **should be closed by using close** 
- When all file descriptors referring to an epoll instance have been closed, the kernel destroys the instance and releases the associated resources for reuse

- Returns : 
	- Success : file descriptor (a nonnegative integer)
	- Error : -1 is returned, and errno is set to indicate the error.
---
### [Source](https://man7.org/linux/man-pages/man2/epoll_create.2.html)
---
## [[epoll - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]