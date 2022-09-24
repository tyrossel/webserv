# accept()
---
~~~cpp
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *restrict addr,
            socklen_t *restrict addrlen);
~~~
- used with connection-based socket types (SOCK_STREAM, SOCK_SEQPACKET)

- Steps : 
	- extracts the first connection request on the queue of pending connections for the listening socket, **sockfd**
	- creates a new connected socket
	- returns a new file descriptor referring to that socket
- newly created socket is not in the listening state
- original socket sockfd is unaffected by this call.

- Arguments : 
	- **sockfd** : socket that has been created with [[socket() - Socket (Webserv)]], bound to a local address with bind, and is listening for connections after a [[listen() - Socket (Webserv)]]
	- **addr** : is a pointer to a sockaddr structure. Structure is filled in with the address of the peer socket
	- **addrlen** : value-result argument -> caller must initialize it to contain the size (in bytes) of the structure pointed to by addr 
		- on return it will contain the actual size of the peer address.

- Returns : 
	- Succes : return a file descriptor for the accepted socket (a nonnegative integer)
	- Error : -1 is returned, errno is set to indicate the error, and addrlen is left unchanged
---
### [Source](https://man7.org/linux/man-pages/man2/accept.2.html)
---
## [[Socket Functions - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]