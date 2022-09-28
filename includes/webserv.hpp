#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define BUFFER_SIZE 4096

/***********************************************************************************
 *                              LIBRARIES                                          *
 ***********************************************************************************/
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <string>

/***********************************************************************************
 *                              FUNCTIONS                                          *
 ***********************************************************************************/
//#include <sys/event.h> //kqueue and kevent. does not compile for some reasons

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
