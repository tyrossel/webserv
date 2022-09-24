# Request Methods
---
- HTTP defines methods to indicate the desired action to be performed
- There is the list : 
	- **GET : Fetch a URL**
		- requests that the target resourcec transfer a representation of its state. Should only retrieve data and have no other effect. Prefered over POST. 
	- **HEAD : Fetch information about a URL**
		- requests that the target resource transfer a representation of its state, as GET, but **without the representation data enclosed in the response body**
	- **POST : Send form data to a URL and get a response back**
		- requests that the target resource process the representation enclosed in the request according to the semantics of the target resource. Use for posting a message on internet forum or subscribing to a mailing list
	- **PUT : Store to an URL**
		- requests that the target resource create or update its state with the state defined by the representation eclosed in the request. Differ from POST because client specifies the target location on the server
	- **DELETE : Delete a URL GET and POST (forms) are commonly used**
		- requests that the target resource delete its state
	- **CONNECT** :  requests that the intermediary establish a TCP/IP tunnel to the origin server identified by the request target. Use to secure connections through one or more HTTP prxies with TLS
	- **OPTIONS, TRACE, PATCH ...**
---
### [Source](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol)
---
## [[HTTP - Hyper Text Transfer Protocol (Webserv)]]
## [[Webserv]]