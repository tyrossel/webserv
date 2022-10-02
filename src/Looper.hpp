#ifndef LOOPER_HPP
#define LOOPER_HPP

#include "Config.hpp"
#include "Server.hpp"

class Looper {
    private:
        Config                          _config;
        int                             _max_fd;
        std::vector<Server>             _servers;
        std::map<long int, Server *>    _active_servers;
        std::vector<int>                _ready_fd;

        fd_set                      _active_fd_set;

    public:
        Looper();
        Looper(const Looper &other);
        virtual ~Looper();

        int     setupLoop();
        void    setMaxFd();
        void    addServer(Server &server);
        void    log(std::string message);
        void    loop();
};

#endif // LOOPER_HPP


