# Request Messages
---
- When you write :
	~~~
	www.example.com
	~~~
- Web browser will rewrite and send :
	~~~
	http://www.example.com:80
	~~~
- This what web-browsers send to the servers 
- If server is configured to certain default pages, like default web page where displayed when we visit a folder on the server, some servers use `index.html or public.html`
---
![[http_request_message.png]]
- client sends request messages to the server :
	- Request line, consisting of : 
		- the case-sensitive request method
		- a space
		- the requested URL
		- another space
		- the protocol version,
		- a carriage return
		- a line feed : **GET /index.html HTTP/1.1**
	- zero or more request header fields (at least 1 or more headers in case of HTTP/1.1) each consisting of :
		- the case-insensitive field name
		- a colon, optional leading whitespace
		- the field value, an optional trailing whitespace
		- ending with a carriage return and a line feed : 
		~~~
		Host: www.example.com 
		Accept-Language: en
		~~~
	- an empty line, consisting of a carriage return and a line feed;
	- an optional message body
- In the HTTP/1.1 protocol, **all header fields except Host: hostname are optional**
- A request line containing only the path name is accepted by servers to maintain compatibility with HTTP clients before the HTTP/1.0 specification
---
### [Source](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol)
### [Tuto](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
---
## [[HTTP - Hyper Text Transfer Protocol (Webserv)]]
## [[Webserv]]