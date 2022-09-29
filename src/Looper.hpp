#ifndef LOOPER_HPP
#define LOOPER_HPP

#include "Config.hpp"
#include "Server.hpp"

class Looper {
    private:
        Config              _config;
        int                 _max_fd;
        std::vector<Server> _servers;
        fd_set              _active_fd_set;

    public:
        Looper();
        Looper(const Looper &other);
        virtual ~Looper();

        void    setupLoop();
        void    setMaxFd();
        void    addServer(Server &server);
        void    log(std::string message);
        void    loop();
        int     readFromClient(int filedes);
};

#endif // LOOPER_HPP


