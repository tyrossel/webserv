# send()
---
~~~cpp
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);
~~~
- send a message on a socket
- used only when the socket is in a connected state
- difference between send and write is the presence of flags.  With a zero flags argument, send is equivalent to write
- If the message is too long to pass atomically through the underlying protocol : error EMSGSIZE is returned, and the message is not transmitted.

- Arguments :
	- **sockfd** : fd of the sending socket.
	- **buf** : message to send
	- **len** : length of message
	- **flags** : bitwise OR of zero or more of the [following flags](https://man7.org/linux/man-pages/man2/send.2.html)

- Returns : 
	- Success : number of bytes sent
	- Error : -1 is returned
---
### [Source](https://man7.org/linux/man-pages/man2/send.2.html)
---
## [[Socket Functions - Webserv]]
## [[Functions - Webserv (CPP)]]
## [[Webserv]]