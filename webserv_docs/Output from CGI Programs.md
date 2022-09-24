# Output From CGI Programs
---
- Usually, CGI program output goes to the client through the server CGI process
- Our CGI program doesn't have to worry abou protocol-specific headers and such
- We will consider CGI header which is sent by our program and read by the web server
---
## Output

#### CGI generic headers
- output begins with generic headers
- one or more text lines in this format
~~~
 name: value
~~~
- **single blank line signals end of the header**
- After this, server stops parsing our program's header and sends the rest of our data untouched to the client
- if server detects odd header linesm server logs a 500 error and doesn't return any data to client

### Content-type
- this headers reports the type of data our program is running
- is a valid MIME type in the format type/subtype
- Example :
~~~
text/html, text/plain, image/gif, image/jpeg, audio/basic
~~~

### Content-length
- reports the length of the data in bytes, not including the header
- used when we are streaming a binary file like image from CGI program

### Expires
- reports the date on which file should be considered outdated by the client
- GMT format
~~~
Saturday, 12-Nov-94 14:05:51 GMT
~~~

### CGI Specific-headers
- those are special to CGI and make the server act on our program's behalf
#### Location
- reports location of a new file for the server or client to retrieve
- must be a complete URL or virtual path
- if it's a complete URL like `http://myserver/misc/file.html` : server redirects client to the URL
- if it's a virtual path like `/misc/file.html` : **server restart the request** using this new path
~~~
http://myserver/misc/file.html
~~~

#### Status
- status code that is returnd for every HTTP request
- indicate to the client whether the request succeeded or not
![[status_code_cgi.png]]
---
### [Source](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)
---
## [[Common Gateway Interface (CGI)]]
## [[Webserv]]