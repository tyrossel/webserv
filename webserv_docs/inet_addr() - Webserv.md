# inet_addr()
---
~~~cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

in_addr_t inet_addr(const char *cp);
~~~
- converts the Internet host address **cp** from IPv4 numbers-and-dots notation into binary data in network byte order.
- IPV4 -----> Binary data

- Returns :
	- Success : converted address returned
	- Error : -1
		- **IMPORTANT** : -1 is a valid address (255.255.255.255)
---
### [Source](https://linux.die.net/man/3/inet_addr)
---
## [[Functions - Webserv (CPP)]]
## [[Webserv]]