# kevent()
---
~~~cpp
#include <sys/event.h>

int kevent(int	kq, const struct kevent	*changelist, int nchanges, struct	kevent *eventlist, int nevents, const struct timespec *timeout);
~~~
- used to register events with the queue, and **return any pending	events to the user**.  
- Arguments : 
	- **changelist** : is	a pointer to an array of kevent structures, as defined in <sys/event.h>.
		- All changes contained in the changelist are applied before any pending events are read from the queue.  
	- **nchanges** : gives the size of changelist
	- **eventlist** : pointer to an array of kevent structures.  
	- **nevents** :  determines the size of eventlist. 
		- When nevents is zero, **kevent() will return immediately even if there is a timeout specified unlike select**.  
	- **timeout**
		- is a non-NULL pointer, it specifies a maximum interval to wait for an event, which will be interpreted as a struct	timespec.  
		- is a NULL pointer, kevent() waits indefinitely.  To effect a poll, the timeout argument should	be non-NULL, pointing to a zero-valued timespec structure. 
---
- Returns :
	- Success : the number of events placed in the eventlist, up to the value given by nevents.  
	- Error occurs while processing an element of the changelist and there is enough room in the eventlist, then the event will be placed in the eventlist with EV_ERROR set in flags and the system error in data.
	- Otherwise, -1 will be returned, and errno will be set to indicate the error condition.  
	- If the time limit expires, then kevent() returns 0.

---
The kevent	structure is defined as:
~~~cpp
struct kevent {
	uintptr_t	ident;	     /*	identifier for this event */
	short     filter;	     /*	filter for event */
	u_short   flags;	     /*	action flags for kqueue	*/
	u_int     fflags;	     /*	filter flag value */
	int64_t   data;	         /*	filter data value */
	void      *udata;	     /*	opaque user data identifier */
	uint64_t  ext[4];	     /*	extensions */
     };
~~~
- **ident** : Value used to identify this event. Often is a file descriptor.
- **filter** : Identifies the kernel filter used to process this event. 
- **flags** : Actions to perform on the event.
- **fflags** : Filter-specific	flags.
- **data** :  Filter-specific	data value.
- **udata** : Opaque user-defined value passed through the kernel unchanged.
- **ext** : Extended data passed to and from kernel.  
	- ext[0] and ext[1] members use is defined by the filter.  
		- If the filter does not use them, the members are copied unchanged.  
	- The ext[2] and ext[3] members are always passed through the	kernel as-is, making additional context available to application.

---
### [Flags and docs](https://www.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2)
---
## [[Functions - Webserv (CPP)]]
## [[Webserv]]