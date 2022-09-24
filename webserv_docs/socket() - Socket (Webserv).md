# socket()
---
~~~cpp
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
~~~
- create an endpoint for communication
- returns a file descriptor that refers to that endpoint. 
	- fd returned by a successful call will be the lowest-numbered file descriptor not currently open for the process.
- **domain** : specifies a communication domain; this selects the protocol family which will be used for communication.
	- defined in : _<sys/socket.h>_
- **type** : socket has the indicated **type**, which specifies the communication semantics.
- **protocol** :  specifies a particular protocol to be used with the socket.
	- Normally only a single protocol exists to support a particular socket type within a given protocol family, in which case protocol can be specified as 0.

- Returns : 
	- Success : fd for the new socket is returned.  
	- Error : -1 is returned, and errno is set to indicate the error.
---
### [Source](https://man7.org/linux/man-pages/man2/socket.2.html)
---
## [[Socket Functions - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]