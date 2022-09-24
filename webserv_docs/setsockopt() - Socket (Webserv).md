# setsockopt()
---
~~~cpp
#include <sys/types.h>
#include <sys/socket.h>

int setsockopt(int socket, int level, int option_name,
           const void *option_value, socklen_t option_len);
~~~
- This function shall
	- set the option specified by **option_name** 
	- at the protocol level specified by **level**
	- to the value pointed to by **option_value** 
	- for the socket associated with the fd specified by **socket**
- Some **level** are defined in : _<netinet/in.h>_

- Returns :
	- Success : 0
	- Error : -1
---
### [Source]()
---
## [[Functions - Webserv (CPP)]]
## [[Webserv]]