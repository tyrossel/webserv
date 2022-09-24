# getsockname()
---
~~~cpp
#include <sys/socket.h>

int getsockname(int sockfd, struct sockaddr *restrict addr,
                socklen_t *restrict addrlen);
~~~
- get socket name
- returns the current address to which the socket **sockfd** is bound, in the buffer pointed to by addr
- **addrlen** should be initialized to indicate the amount of space (in bytes) pointed to by addr
- returned address is truncated if the buffer provided is too small
	- in this case, addrlen will return a value greater than was supplied to the call.

- Arguments : 
	- **sockfd** : fd of the socket
	- **addrr** : point the buffer
	- **addrrlen** : on return it contains the actual size of the socket address.

- Returns :
	- Success : 0
	- Error : -1
---
### [Source](https://man7.org/linux/man-pages/man2/getsockname.2.html)
---
## [[Socket Functions - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]