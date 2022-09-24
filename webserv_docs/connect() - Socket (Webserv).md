# connect()
---
~~~cpp
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr,
            socklen_t addrlen);
~~~
- initiate a connection on a socket
- connects the socket referred to by the fd **sockfd** to the address specified by **addr**

- Arguments :
	- **sockfd** : fd of the socket
	- **addr** : address of the socket
	- **addrlen** : size of the address

- Returns : 
	- Success : 0
	- Error : -1
---
### [Source](https://man7.org/linux/man-pages/man2/connect.2.html)
---
## [[Socket Functions - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]