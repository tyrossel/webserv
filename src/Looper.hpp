#ifndef LOOPER_HPP
#define LOOPER_HPP

#include "Config.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"

class Looper {
    private:
<<<<<<< HEAD
        Config                              _config;
        int                                 _max_fd;
        std::vector<Server>                 _servers;
        std::map<long int, Server *>        _active_servers;
        std::vector<int>                    _ready_fd;
        std::map<long int, std::string>     _response;
        std::map<long int, RequestParser>   _request;
=======
        Config                          _config;
        int                             _max_fd;
        std::vector<Server>             _servers;
        std::map<long int, Server *>    _active_servers;
        std::vector<int>                _ready_fd;
        std::map<long int, std::string> _response;
>>>>>>> b0ede9b (function to craft the response are splitted and more organized. They are static for the moment. 👬)


        fd_set                              _active_fd_set;

    public:
        Looper();
        Looper(const Looper &other);
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
<<<<<<< HEAD
        // =================================================================

        // CHECKERS ========================================================
        int     checkCode(RequestParser request);
        int     checkPath(long socket);
        int     checkExtension(RequestParser request);
        // =================================================================

        // RESPONSE CRAFTING ===============================================
        int     buildResponse(long socket);
        void    addStaticBodyResponse(long socket);
        void    addDate(long socket);
        void    addBodyToResponse(long socket);
        int     addHTTPHeader(long socket);
        int     writeResponseHeader(long socket);
        void    addErrorBodyToResponse(long socket);
        // =================================================================
=======
        int     buildResponse(long socket, RequestParser request);
        void    addStaticBodyResponse(std::string &str);
        void    addDate(std::string &str);
        void    addBodyToResponse(std::string &str);
>>>>>>> b0ede9b (function to craft the response are splitted and more organized. They are static for the moment. 👬)


};

#endif // LOOPER_HPP


