#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define BUFFER_SIZE 4096

/***********************************************************************************
 *                               HTTP RESPONSES                                    *
 ***********************************************************************************/
 /*---------------------------- SUCCESSFUL RESPONSES ------------------------------*/
# define HTTP_OK 200                            // request succeeded
# define HTTP_CREATED 201                       // new resource was created as a result (use after POST or PUT)
# define HTTP_ACCEPTED 202                      // request has been received but not yet acted upon
# define HTTP_NO_CONTENT 204                    // no content to send for this request, but the headers may be useful
# define HTTP_RESET_CONTENT 205                 // tells the user agent to reset the document which sent this request
# define HTTP_PARTIAL_CONTENT 206               // range header is sent from the client to request only part of a resource

/*----------------------------- REDIRECTION MESSAGES ------------------------------*/
# define HTTP_MULTIPLE_CHOICES 300              // request has more than one possible response
# define HTTP_MOVED_PERMANENTLY 301             // URL of the requested resource has been changed permanently. The new URL is given in the response
# define HTTP_FOUND 302                         // URI of requested resource has been changed temporarily
# define HTTP_SEE_OTHER 303                     // response to direct the client to get the requested resource at another URI with a GET request

/*----------------------------- CLIENT ERROR RESPONSES -----------------------------*/
# define BAD_REQUEST 400                        // server cannot or will not process the request due to something that is perceived to be a client error
# define UNAUTHORIZED 401                       // client must authenticate itself to get the requested response
# define FORBIDDEN 403                          // client does not have access rights to the content. Unlike 401 Unauthorized, the client's identity is known to the server
# define NOT_FOUND 404                          // server can not find the requested resource
# define PROXY_AUTHENTICATION 407               // similar to 401 Unauthorized but authentication is needed to be done by a proxy
# define REQUEST_TIMEOUT 408                    // server would like to shut down this unused connection
# define CONFLICT 409                           // request conflicts with the current state of the server
# define LENGTH_REQUIRED 411                    // server rejected the request because the Content-Length header field is not defined and the server requires it
# define PRECONDITION_FAILED 412                // client has indicated preconditions in its headers which the server does not meet
# define URI_TOO_LONG 414                       // URI requested by the client is longer than the server is willing to interpret
# define MEDIA_UNSUPPORTED 415                  // media format of the requested data is not supported by the server
# define RANGE_NOT_SATISFIABLE 416              // range specified by the Range header field in the request cannot be fulfilled (possible that the range is outside the size of the target URI's data)
# define EXPECTATION_FAILED 417                 // expectation indicated by the Expect request header field cannot be met by the server
# define TEAPOT 418                             // server refuses the attempt to brew coffee with a teapot
# define TOO_MANY_REQUESTS 429                  // user has sent too many requests in a given amount of time ("rate limiting")
# define UNAVAILABLE_LEGAL_REASON 451           // user agent requested a resource that cannot legally be provided, such as a web page censored by a government

/*----------------------------- SERVER ERROR RESPONSES -----------------------------*/
# define INTERNAL_SERVER_ERROR 500              // server has encountered a situation it does not know how to handle
# define NOT_IMPLEMENTED 501                    // request method is not supported by the server and cannot be handle. The only methods that servers are required to support (and therefore that must not return this code) are GET and HEAD.
# define BAD_GATEWAY 502                        // server, while working as a gateway to get a response needed to handle the request, got an invalid response
# define SERVICE_UNAVAILABLE 503                // server is not ready to handle the request (server that is down for maintenance or that is overloaded)
# define GATEWAY_TIMEOUT 504                    // server is acting as a gateway and cannot get a response in time
# define HTTP_VERSION_UNSUPPORTED 505           // HTTP version used in the request is not supported by the server
# define NOT_EXTENDED 510                       // Further extensions to the request are required for the server to fulfill it
# define NETWORK_AUTHENTICATION_REQUIRED 511    // client needs to authenticate to gain network access

/***********************************************************************************
*                              CUSTOM DEFINES                                      *
***********************************************************************************/
# define URI_MAX_SIZE 10000

/***********************************************************************************
 *                              LIBRARIES                                          *
 ***********************************************************************************/
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

/***********************************************************************************
 *                              FUNCTIONS                                          *
 ***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <cstring>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#endif // WEBSERV_HPP
