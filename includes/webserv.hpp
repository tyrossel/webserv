#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/***********************************************************************************
 *                              LIBRARIES                                          *
 ***********************************************************************************/
#include <map>
#include <stack>
#include <vector>
#include <iostream>

/***********************************************************************************
 *                              FUNCTIONS                                          *
 ***********************************************************************************/
//#include <sys/event.h> kqueue and kevent. does not compile for some reasons

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

#endif // WEBSERVHPP