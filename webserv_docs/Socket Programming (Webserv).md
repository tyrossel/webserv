# Socket Programming
---
- way of connecting two nodes on a network to communicate with each other
- One socket listen on a particular port at an IP / the other socket reaches out the other to form a connection
- Server = listener socket
- Client = reaches out the server
---
![[socket_programming.png]]
- **socket()** : use to create socket
- **setsocketopt()** : helps in manipulating options for the socket (referred by sockfd). Helps in reuse of address and port.
	- Prevents error such as "address already in use"
- **bind()** : binds the socket to the address and port number specified in _addr_
- **listen()** : puts the server socket in passive mode (wait client to approach the server)
- **accept()** : extract the first connection request on the queue of pending connections for the listining socket (_sockfd_) -> creates a new connected socket -> returns a new fd referring to this socket
	- Client-Server is connected
---
### [[TCP Server-Client (Webserv)]]
---
### [Source]([https://www.geeksforgeeks.org/socket-programming-cc/](https://www.geeksforgeeks.org/socket-programming-cc/ "https://www.geeksforgeeks.org/socket-programming-cc/"))
---
## [[Webserv]]