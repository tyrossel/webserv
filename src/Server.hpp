#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"

class Server {
    private:
        int                                 _port; // where the server is listening
		std::string							_address; // IP address where the server is listening
        long                                _fd;
        unsigned int                        _host;
        std::vector<std::string>            _root;  //🌳🌳🌳🌳🌳🌳🌳🌳🌳
        std::vector<std::string>            _index; //👆👆👆👆👆👆👆👆👆👆
        std::vector<std::string>            _name;  //🏠🏠🏠🏠🏠🏠🏠🏠🏠
        std::map<std::string, std::string>  _location;
        // For the map we will have in [0][0](root) the location followed by the path
        // The next parsed elements will go in the map in a random order and will be called with find

        sockaddr_in                         _addr;

    public:
        Server();
        Server(int port, std::string host);
        Server(const Server &other);
        ~Server();

        void    log(std::string message);
        long    createSocket(); //Create and set socket
        void    setAddress();   //Set address
        int     setupListen(); // Bind and listen socket
        int     buildServer();
        void    close(int socket);
        int     send(long socket, std::map<long, std::string> response);

        // GETTERS
        long                                        getFd() const;
		std::string									getAddress() const;
        int                                         getPort() const; // Use the const one, remove the not const
        std::vector<std::string>                    getName() const;
        std::vector<std::string>                    getRoot() const;
        std::vector<std::string>                    getIndex() const;
        std::map<std::string, std::string>          getLocation() const;
        unsigned int                                getHost() const;


    // ADDERS
        void                                        addPort(int port);
		void										addAddress(const std::string &address);
        void                                        addHost(const std::string &host);
        void                                        addRoot(const std::string &root);
        void                                        addIndex(const std::string &index);
        void                                        addLocation(const std::string &key, const std::string &value);
        void                                        addHost(const unsigned int &host);
        void                                        addName(const std::string &name);


};

#endif // SERVER_HPP
