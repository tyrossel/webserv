#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include "Location.hpp"
#include <stdexcept>

class Server {
    private:
        int                                 _port; // where the server is listening
		std::string							_address; // IP address where the server is listening
        long                                _fd;
        unsigned int                        _host;
		std::string				            _root;  //🌳🌳🌳🌳🌳🌳🌳🌳🌳
        std::vector<std::string>            _index; //👆👆👆👆👆👆👆👆👆👆
        std::vector<std::string>            _name;  //🏠🏠🏠🏠🏠🏠🏠🏠🏠
		std::string				            _cgiBin;
        std::vector<std::string>            _cgiExtensions; // All those files are sent to the CGI
        std::map<std::string, Location> 	 _location;
        // For the map we will have in [0][0](root) the location followed by the path
        // The next parsed elements will go in the map in a random order and will be called with find

        sockaddr_in                         _addr;

    public:
        Server();
        Server(const Server &other);
		Server &operator=(const Server &rhs);
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
		std::string									getCGIBin() const;
        std::vector<std::string>                    getCGIExtensions() const;
        std::string									getRoot() const;
        std::vector<std::string>                    getIndex() const;
        const std::map<std::string, Location>       &getLocations() const;
        unsigned int                                getHost() const;


    // ADDERS
        void                                        addPort(int port);
		void										addAddress(const std::string &address);
        void                                        addHost(const std::string &host);
		void										setCGIBin(const std::string& bin);
		void										addCGIExtension(const std::string& ext);
        void                                        setRoot(const std::string &root);
        void                                        addIndex(const std::string &index);
        void                                        addLocation(const std::string &key, const Location &location);
        void                                        addHost(const unsigned int &host);
        void                                        addName(const std::string &name);


};

#endif // SERVER_HPP
