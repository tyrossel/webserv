# TCP Server-Client
---
- TCP suited for applications that require high reliability and transmission time relatively less critical
- USed by HHTP, HTTPs, FTP, SMPT ...
- TCP rearrange data packets in the order specified
- Guarantee that data transferred remains intact and arrives in same order in which it was set
- TCP does congestion control
- Does error checking and error recovery
---
![[tcp_server-client.png]]
Few steps : 
	- Use socket() to create socket
	- Use bind() to bind socket to server address
	- Use listen() put server on passive mode, waiting for client
	- Use accept(), connection with client is working, transferring data
	- Go back to **Listen step**
- See [[Socket Programming (Webserv)]]
---
### [Source](https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/)
---
## [[Client-Server Communication (Webserv)]]
## [[Socket Programming (Webserv)]]
## [[Webserv]]