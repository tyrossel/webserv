#ifndef LOOPER_HPP
#define LOOPER_HPP

#include "Config.hpp"
#include "CommonGatewayInterface.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"
#include "webserv.hpp"

class Looper {
    private:
        Config                              _config;
        int                                 _max_fd;
        std::vector<Server>                 _servers;
        std::map<long int, Server *>        _active_servers;
        std::vector<int>                    _ready_fd;
        std::map<long int, std::string>     _response;
        std::map<long int, RequestParser>   _request;

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

        // CHECKERS ========================================================
        int     checkCode(RequestParser request);
        int     checkPath(long socket);
        int     secFetchImage(long socket);
        RequestType     requestMethod(long socket);
        // =================================================================

        // RESPONSE CRAFTING ===============================================
        int     buildResponse(long socket, const Location &loc);
        int     buildGetResponse(long socket, const Location &loc);
        int     buildPostResponse(long socket, const Location &loc);
        int     buildDeleteResponse(long socket, const Location &loc);

        void    addServerHeaderResponse(long socket);
        void    addContentType(long socket);
        void    addDate(long socket);
        void    addBodyToResponse(long socket);
        int     addHTTPHeader(long socket);
        void    addContentLengthPOST(long socket);

        int     writeResponseHeader(long socket);
        void    addErrorBodyToResponse(long socket);
        // =================================================================
};

#endif // LOOPER_HPP
