# bind()
---
~~~cpp
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr,
        socklen_t addrlen);
~~~
- bind a name to a socket
- When a socket is created with [[socket() - Socket (Webserv)]]
	- it exists in a name space (address family) but has no address assigned to it
	- bind() assigns the address specified by addr to the socket referred to by the fd **sockfd**
- **addrlen** : specifies the size, in bytes, of the address structure pointed to by addr

- Returns :
	- Succes : 0
	- Error : -1
---
### [Source](https://man7.org/linux/man-pages/man2/bind.2.html)
---
## [[Socket Functions - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]