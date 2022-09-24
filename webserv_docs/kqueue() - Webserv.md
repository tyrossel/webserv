# kqueue
---
~~~cpp
#include <sys/event.h>

int kqueue(void);
~~~
- kernel event notification mechanism
- provides a generic method	of notifying the user when an event happens or a condition holds 
- A kevent is identified by the (ident, filter) pair; there may only be **one unique kevent per kqueue.**
- creates a	new kernel event queue and returns a descriptor.  
	- The queue is not inherited by a child	created with fork.
	- if rfork is called without the RFFDG flag, then the descrip-
     tor table is shared, which will allow sharing of the kqueue between two processes.

 - Returns :
	 - Success : creates a new kernel event queue and returns a
     file descriptor.  
     - Error : -1 is returned and errno set.
---
### [Source](https://www.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2)
---
## [[Functions - Webserv (CPP)]]
## [[Webserv]]