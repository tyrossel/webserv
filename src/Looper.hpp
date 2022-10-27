#ifndef LOOPER_HPP
#define LOOPER_HPP

#include "Config.hpp"
#include "CommonGatewayInterface.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
#include "webserv.hpp"

class Looper {
    private:
        Config                              _config;
        int                                 _max_fd;
        std::vector<Server>                 _servers;
        std::map<long int, Server *>        _active_servers;
        std::vector<int>                    _ready_fd;
        std::map<long int, Response>        _response;
        std::map<long int, Request>         _request;

        fd_set                              _active_fd_set;

    public:
        Looper();
        Looper(const Looper &other);
        Looper(const Config &cfg);
        virtual ~Looper();

        int     setupLoop();
        int     readFromClient(long socket);
        void    setMaxFd();
        void    addServer(Server &server);
        void    log(std::string message);

        // LOOP RELATED ====================================================
        void    loop();
        void    catchCommunication(fd_set &reading_fd_set, int ret);
        void    requestProcess(fd_set &reading_fd_set);
        void    sendResponse(fd_set &reading_fd_set, fd_set &writing_fd_set, fd_set &_active_fd_set);
        void    selectErrorHandle();
        // =================================================================

        // RESPONSE CRAFTING ===============================================
        int     buildResponse(long socket, const Location *loc);
        // =================================================================
};

#endif // LOOPER_HPP
