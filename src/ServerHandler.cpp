#include "ServerHandler.hpp"

/**************************************************************************************/
/*                            Constructor/Destructor                                  */
/**************************************************************************************/
ServerHandler::ServerHandler(int port) : _port(port)
{
    memset(&addr, 0, sizeof(addr));

    if ((this->_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cerr << ("Failed to create server socket") << std::endl;
        exit(1);
    }

    addr.sin_family = AF_INET;

    // For now, s_addr is on INADDR_ANY which is 0.0.0.0
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(this->_port);

    if (bind(this->_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        std::cerr << ("Failed to bind server socket") << std::endl;
        exit(1);
    }

    if (listen(this->_fd, MAX_PENDING) < 0) {
        std::cerr << ("Failed to listen on server socket") << std::endl;
        exit(1);
    }

    fcntl(this->fd, F_SETFL, O_NONBLOCK);

    IOLoop::getInstance()->addHandler(fd, this, EPOLLIN);
}

ServerHandler::~ServerHandler() {}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/
int ServerHandler::handle(epoll_event e)
{
    sockaddr_in client_addr;
    socklen_t ca_len = sizeof(client_addr);

    int client = accept(fd, (struct sockaddr *) &client_addr, &ca_len);

    if (client < 0) {
        std::cerr << ("Error accepting connection") << std::endl;
        return -1;
    }

    cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << endl;

    //new EchoHandler(client, client_addr);
    new EchoHandler(client);
    return 0;
}
