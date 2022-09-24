# HTTP/1.1
---
- TCP/IP connections are used
	- Port 80 to connection unencrypted
	- Port 443 if encrypted
- **connection could be reused for more than one request/response** (instead of HTTP/1.0)
- Request latency reduce because client does not need to re-negotiate TCP 3-Way-Handskae connection **after the first request has been sent**
- **HTTP pipelining added** : allow client to send multiple requests before waiting for each response
	- never considered really safe because server did not handle pipelined requests properly
	- only **HEAD and GET** requests could be pipelined in a safe mode
	- **Removed in HTTP/2** (exetended the usage of persistent connections by multiplexing many concurrent requests/responses through a single TCP/IP connection)
- Better management of cached resources (better headers)
- Introduce **byte range serving** : client can request only one or more portions (ranges of byte) of a resource / the server usually sends only the requested portion
---
### [Source](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol)
---
## [[HTTP - Hyper Text Transfer Protocol (Webserv)]]
## [[Webserv]]