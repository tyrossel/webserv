#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../includes/webserv.hpp"

class ServerConfig {
    private:
        int                                 _port;  //🐖🐖🐖🐖🐖🐖🐖🐖
        std::string                         _host;  //🏠🏠🏠🏠🏠🏠🏠🏠
        std::string                         _root;  //🌳🌳🌳🌳🌳🌳🌳🌳
        std::string                         _index; //👆👆👆👆👆👆👆👆
        // For the map we will have in [0][0](root) the location followed by the path
        // The next parsed elements will go in the map in a random order and will be called with find
        std::map<std::string, std::string>  _location;

    public:
        // Constructors / Destructor
        ServerConfig();
        ServerConfig(int port, std::string host, std::string root, std::string index, std::map<std::string, std::string> location);
        ServerConfig(const ServerConfig &other);
        virtual ~ServerConfig();

        // Getters
        int                                         getPort() const;
        const std::string                           getHost() const;
        const std::string                           getRoot() const;
        const std::string                           getIndex() const;
        const std::map<std::string, std::string>    getLocation() const;
        // Member functions
};

class Config {
    private:
        std::vector<ServerConfig> _server;

    public:
        // Constructors / Destructor
        Config();
        Config(const Config &other);
        virtual ~Config();

        // Getters
        const       std::vector<ServerConfig> getServer() const;
        int         getNbServer( void ) const;

        // Member functions
        void        addServer(const ServerConfig &newServ);

        friend std::ostream &operator<<(std::ostream &out, const Config &rhs);

};

std::ostream &operator<<(std::ostream &out, const Config &rhs);

#endif // CONFIG_HPP
