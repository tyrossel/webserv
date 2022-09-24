# listen()
---
~~~cpp
#include <sys/socket.h>

int listen(int sockfd, int backlog);
~~~
- listen for connections on a socket
- **marks the socket referred to by sockfd as a passive socket** -> socket that will be used to accept incoming connection requests using [[accept() - Socket (Webserv)]]

- Arguments : 
	- **sockfd** : fd that refers to a socket of type SOCK_STREAM or SOCK_SEQPACKET
	- **backlog** : defines the maximum length to which the queue of pending connections for **sockfd** may grow
		- If a connection request arrives when the queue is full, the client may receive an error
		- If an underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds.

- Returns :
	- Succes : 0
	- Error : -1

---
### [Source](https://man7.org/linux/man-pages/man2/listen.2.html)
---
## [[Socket Functions - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]