# Step for CGI Requests
---
### 1. Client sends the request
- client sends the request
- request might be a document, or something else (content of HTML form ...)
- **IF request is for REGULAR DOCUMENT** (such GIF or HTML), server sends document directly back to the client
- **IF request is data intended for external application**, server needs to use CGI to run that (for example, a request to search in database)

### 2. Server creates the CGI process
- Server receive a request that must be handled by external application
- Server creates a copy of itself
- Second process = **CGI process** -> its the process which the CGI program will run
	- This CGI process has all the same communication pathways that the server process has
	- Only purpose is set up communications between CGI program and server
	- Has access to info about CGI request like :
		- which remote host made the request
		- if that host was authorized with server access control or not
		- browser software the remote user is running

### 3. Server assigns variables and opens data paths
- CGI process takes the data the server has about the current request
- puts it into environment variables
- CGI process enables client to pass data to CGI program as standard input
- Also creates data pathways between CGI program and server
	- Server can now send to the program any encoded form data the client submitted
	- external CGI program can send a reply back to client via web server

### 4. CGI program responds to the client 
- CGI program takes the data that the server provides through environment variables, standard input, or command-line arguments
- it processes data, contacts any external services it needs to, and then sends a response to the server by way of the data pathways using **standard output**
- Server takes the program's response and sends it back to the client software
---
### [Source](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)
---
## [[Common Gateway Interface (CGI)]]
## [[Webserv]]