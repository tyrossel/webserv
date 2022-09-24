# recv()
---
~~~cpp
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);
~~~
- receive a message from a socket
- used to receive messages from a socket
- used **only on a connected socket**
- difference between recv() and read is the presence of flags. With a zero flags argument, recv() is generally equivalent to read
- If a message is too long to fit in the supplied buffer, excess bytes may be discarded depending on the type of socket the message is received from
- If no messages are available at the socket, the receive calls wait for a message to arrive, unless the socket is nonblocking in which case the value -1 is returned
-  An application can use [[select - Webserv]], [[poll() - Webserv]], or [[epoll - Webserv]] to determine when more data arrives on a socket.

- Arguments :
	- **sockfd** : fd of the sending socket.
	- **buf** : message to send
	- **len** : length of message
	- **flags** : bitwise OR of zero or more of the [following flags](https://man7.org/linux/man-pages/man2/recv.2.html)

- Returns :
	- Success : number of bytes received
	- Error : -1
    - When a stream socket peer has performed an orderly shutdown, the return value will be 0 ("EOF" return)
---
### [Source](https://man7.org/linux/man-pages/man2/recv.2.html)
---
## [[Socket Functions - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]