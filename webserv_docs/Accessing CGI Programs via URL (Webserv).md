# Accessing CGI program via URL
---
- Server must figure out if it can handle the request itself or if  it must create CGI process
- To determine whether URL that the client requests refers to CGI program : 2 methods 
---
## Methods

### 1. MIME type
- specify a MIME type for CGI programs, then store the CGI programs in same directories as our HTML documents and graphics
- in this method, all file with specified extensions (.cgi, or .exe) runs as CGI program
- Most flexible method
- **CAREFUL : shouldn't put CGI programs in directories into which users an uploa info because users can then write CGI programs that return info such as password or delete file on our machine**

### 2. Specific directory for CGI programs
- more control over who writes CGI programs and who access to them
- have to designate certain virtual paths (URL prefixes) as CGI directories
- then, any URL requesting a file inside those directories is executed as CGI program
---
### [Source](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)
---
## [[Common Gateway Interface (CGI)]]
## [[Webserv]]