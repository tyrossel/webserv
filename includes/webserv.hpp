#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/***********************************************************************************
 *                               COLORS                                            *
 ***********************************************************************************/
# define RESET       "\033[0m"
# define BLACK       "\033[30m"              /* Black */
# define RED         "\033[31m"              /* Red */
# define GREEN       "\033[32m"              /* Green */
# define YELLOW      "\033[33m"              /* Yellow */
# define BLUE        "\033[34m"              /* Blue */
# define MAGENTA     "\033[35m"              /* Magenta */
# define CYAN        "\033[36m"              /* Cyan */
# define WHITE       "\033[37m"              /* White */
# define BOLDBLACK   "\033[1m\033[30m"       /* Bold Black */

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
# define HTTP_FOUND 302                         // URI of requested resource has been changed temporarily. Client will send a GET request
# define HTTP_SEE_OTHER 303                     // response to direct the client to get the requested resource at another URI with a GET request
# define HTTP_NOT_MODIFIED 304                  // response to direct the client to get the requested resource from the cache
# define HTTP_TEMPORARY_REDIRECT 307            // URI of requested resource has been changed temporarily. Client should send the same request
# define HTTP_PERMANENT_REDIRECT 308            // URI of requested resource has been changed permanently. Client should send the same request

/*----------------------------- CLIENT ERROR RESPONSES -----------------------------*/
# define BAD_REQUEST 400                        // server cannot or will not process the request due to something that is perceived to be a client error
# define UNAUTHORIZED 401                       // client must authenticate itself to get the requested response
# define FORBIDDEN 403                          // client does not have access rights to the content. Unlike 401 Unauthorized, the client's identity is known to the server
# define NOT_FOUND 404                          // server can not find the requested resource
# define METHOD_NOT_ALLOWED 405                 // server can not deal with this HTTP method
# define PROXY_AUTHENTICATION 407               // similar to 401 Unauthorized but authentication is needed to be done by a proxy
# define REQUEST_TIMEOUT 408                    // server would like to shut down this unused connection
# define CONFLICT 409                           // request conflicts with the current state of the server
# define LENGTH_REQUIRED 411                    // server rejected the request because the Content-Length header field is not defined and the server requires it
# define PRECONDITION_FAILED 412                // client has indicated preconditions in its headers which the server does not meet
# define PAYLOAD_TOO_LARGE 413					// The request is larger than the server is willing or able to process.
# define URI_TOO_LONG 414                       // URI requested by the client is longer than the server is willing to interpret
# define MEDIA_UNSUPPORTED 415                  // media format of the requested data is not supported by the server
# define RANGE_NOT_SATISFIABLE 416              // range specified by the Range header field in the request cannot be fulfilled (possible that the range is outside the size of the target URI's data)
# define EXPECTATION_FAILED 417                 // expectation indicated by the Expect request header field cannot be met by the server
# define TEAPOT 418                             // server refuses the attempt to brew coffee with a teapot
# define TOO_MANY_REQUESTS 429                  // user has sent too many requests in a given amount of time ("rate limiting")
# define HEADERS_TOO_LARGE 431					// he server is unwilling to process the request because either an individual header field, or all the header fields collectively, are too large
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
# define BUFFER_SIZE 4096
# define URI_MAX_SIZE 10000
# define HEADERS_MAX_LENGTH 1000

enum RequestType
{
	Get,
	Post,
	Delete,
	Unknown,
	LastRequestType
};

/***********************************************************************************
 *                              LIBRARIES                                          *
 ***********************************************************************************/
#include <map>
#include <algorithm>
#include <stack>
#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <fstream>
#include <sstream>
#include <csignal>

/***********************************************************************************
 *                              FUNCTIONS                                          *
 ***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <cstring>
#include <fcntl.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctime>
#include <utility>

/***********************************************************************************
 *                             GLOBAL VARIABLE                                     *
 ***********************************************************************************/
extern int RUNNING;

#define VERBOSE 0
#define TIMESTAMP_FORMAT "[%T] "

#include "../src/Utils.hpp"

#endif // WEBSERV_HPP
