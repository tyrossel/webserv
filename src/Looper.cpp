#include "Looper.hpp"

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/
Looper::Looper() : _config(), _max_fd(), _servers() {}

Looper::Looper(const Looper &other) : _config(other._config), _max_fd(other._max_fd), _servers(other._servers)
{
    *this = other;
}

Looper::~Looper() {}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

void Looper::setupLoop()
{
    for (unsigned int i = 0; i < _servers.size(); i++)
        _servers[i].buildServer();
}

void Looper::log(std::string message)
{
    std::cout << message << std::endl;
}

void Looper::addServer(Server &server)
{
    this->_servers.push_back(server);
}

void Looper::setMaxFd()
{
    _max_fd = 0;
    for (unsigned int i = 0; i < _servers.size(); i++)
    {
        if (_servers[i].getSock() > _max_fd)
            _max_fd = _servers[i].getSock();
    }
    _max_fd += 1;
}

int Looper::readFromClient(int filedes) {
    char buffer[BUFFER_SIZE];
    int nbytes;

//    printf("\n\nFD RECEIVED : %i \n\n", filedes);
    nbytes = recv(filedes, buffer, BUFFER_SIZE, 0);
    if (nbytes < 0) {
        /* Read error. */
        log("recv failed");
        //exit(1);
    } else if (nbytes == 0)
        /* End-of-file. */
        return -1;
    else {
        /* Data read. */
        //write(1, &buffer, strlen(buffer));
        fprintf(stderr, "Server: got message: `%s'\n", buffer);
        return 0;
    }
    return 0;
}

void Looper::loop()
{
    fd_set  read_fd_set;
    socklen_t size;
    sockaddr_in clientname;

    FD_ZERO(&_active_fd_set);
    for (unsigned int i = 0; i < _servers.size(); i++)
        FD_SET(_servers[i].getSock(), &_active_fd_set);

    while (1) {
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = _active_fd_set;
        setMaxFd();
        if (select(_max_fd + 1, &read_fd_set, NULL, NULL, NULL) < 0) {
            log("select failed");
            exit(1);
        }

        /* Service all the sockets with input pending. */
        for (int i = 0; i <= _max_fd; ++i) {
            if (FD_ISSET(i, &read_fd_set)) {
                for (unsigned int j = 0; j < _servers.size(); j++) {
                    if (i == (_servers[j].getSock())) //TODO : THIS IS STATIC FOR 1 SERVER
                    {
//                        printf("\n\nFD CATCHED : %i \n\n", i);

                        /* Connection request on original socket. */
                        int _new;
                        size = sizeof(clientname);

                        //TODO : THIS IS STATIC FOR 1 SERVER
                        _new = accept(_servers[j].getSock(), (struct sockaddr *) &clientname,
                                      (socklen_t * ) & size); // accept can be called like accept(socket, NULL, NULL)
                        if (_new < 0) {
                            log("accept failed");
                            exit(1);
                        }

                        fprintf(stderr, "Server: connect from host %s, port %hd.\n",
                                inet_ntoa(clientname.sin_addr),
                                ntohs(clientname.sin_port));

                        FD_SET(_new, &_active_fd_set);
                    } else {
                        /* Data arriving on an already-connected socket. */
                        if (readFromClient(i) < 0) {
                            close(i);
                            FD_CLR(i, &_active_fd_set);
                        }
                    }
                }
            }
        }
    }
}
