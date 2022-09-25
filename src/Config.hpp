#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../includes/webserv.hpp"

class ServerConfig {
    private:
        int                                 _port;  //🐖🐖🐖🐖🐖🐖🐖🐖
        std::vector<std::string>            _host;  //🏠🏠🏠🏠🏠🏠🏠🏠🏠
        std::vector<std::string>            _root;  //🌳🌳🌳🌳🌳🌳🌳🌳🌳
        std::vector<std::string>            _index; //👆👆👆👆👆👆👆👆👆👆
        // For the map we will have in [0][0](root) the location followed by the path
        // The next parsed elements will go in the map in a random order and will be called with find
        std::map<std::string, std::string>  _location;

    public:
        // Constructors / Destructor
        ServerConfig();
        ServerConfig(const ServerConfig &other);
        virtual ~ServerConfig();

        // Getters
        int                                         getPort() const;
        std::vector<std::string>                    getHost() const;
        std::vector<std::string>                    getRoot() const;
        std::vector<std::string>                    getIndex() const;
        std::map<std::string, std::string>          getLocation() const;
        // Member functions
        void        addPort(int port);
        void        addHost(const std::string &host);
        void        addRoot(const std::string &root);
        void        addIndex(const std::string &index);
        void        addLocation(const std::string &key, const std::string &value);
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
        const std::vector<ServerConfig> getServer() const;
        int                             getNbServer( void ) const;

        // Member functions
        void        addServer(const ServerConfig &newServ);

};

std::ostream &operator<<(std::ostream &out, const Config &rhs);

#endif // CONFIG_HPP
