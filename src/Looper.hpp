#ifndef LOOPER_HPP
#define LOOPER_HPP

#include "Config.hpp"
#include "CommonGatewayInterface.hpp"
#include "Response.hpp"
#include "webserv.hpp"
#include <ctime>

class Looper {
    private:
        Config                              _config;
        int                                 _max_fd;
		time_t								_timeout;
        std::vector<Server>                 _servers;
        std::map<long int, Server *>        _active_servers;
        std::vector<int>                    _ready_fd;
        std::map<int, Response *>        _responses;
        std::map<int, Request>         _requests;
		std::map<int, std::time_t>		_last_activity;
		std::map<int, std::string>		_raw_request;

        fd_set                              _active_fd_set;

    public:
        Looper();
        Looper(const Looper &other);
        Looper(const Config &cfg);
		Looper &operator=(const Looper &rhs);
        ~Looper();

        // MEMBER FUNCTIONS ================================================
        void    printLog(const Request &request, int socket);
        void    addServer(Server &server);
        void    setMaxFd();
        int     setupLoop();
        void    checkConnectionTimeout();
        bool    isInTimeout(int socket);
        // =================================================================

        // EXTRACT REQUEST =================================================
        int     startParsingRequest(int socket);
        int     readFromClient(long socket);

        // LOOP RELATED ====================================================
        void    loop();
        void    sendResponse(fd_set &reading_fd_set, fd_set &writing_fd_set, fd_set &_active_fd_set);
        void    catchCommunication(fd_set &reading_fd_set, int ret);
        void    requestProcess(fd_set &reading_fd_set);
        void    selectErrorHandle();
        // =================================================================

        // RESPONSE CRAFTING ===============================================
        int     buildResponse(long socket, const Location &loc);
        void    buildErrorResponse(long socket, int status, bool close=false);
        // =================================================================
};

#endif // LOOPER_HPP
