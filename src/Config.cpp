#include "Config.hpp"

/** HOW TO TREAT LOCATION PATH
 * The modifier in the location block is optional. Having a modifier in the location block will allow NGINX to treat a URL differently. Few most common modifiers are:
 *      none: If no modifiers are present in a location block then the requested URI will be matched against the beginning of the requested URI.
 *      =: The equal sign is used to match a location block exactly against a requested URI.
 *      ~: The tilde sign is used for case-sensitive regular expression match against a requested URI.
 *      ~*: The tilde followed by asterisk sign is used for case insensitive regular expression match against a requested URI.
 *      ^~: The carat followed by tilde sign is used to perform longest nonregular expression match against the requested URI. If the requested URI hits such a location block, no further matching will takes place.
 */

/**************************************************************************************/
/*                          CONSTRUCTORS / DESTRUCTORS                                */
/**************************************************************************************/

// ------------------------------- SERVER CONFIG ------------------------------------ //

ServerConfig::ServerConfig() : _port(), _host(), _root(), _index(), _location() {}
ServerConfig::ServerConfig(const ServerConfig &other) : _port(other._port), _host(other._host), _root(other._root), _index(other._index), _location(other._location) {}
ServerConfig::~ServerConfig() {}

// ----------------------------------- CONFIG --------------------------------------- //

Config::Config() : _server() {}
Config::Config(const Config &other) : _server(other._server) {}
Config::~Config() {}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

// ------------------------------- SERVER CONFIG ------------------------------------ //

int                                 ServerConfig::getPort() const { return (this->_port); }
std::vector<std::string>            ServerConfig::getHost() const { return (this->_host); }
std::vector<std::string>            ServerConfig::getRoot() const { return (this->_root); }
std::vector<std::string>            ServerConfig::getIndex() const { return (this->_index); }
std::map<std::string, std::string>  ServerConfig::getLocation() const { return (this->_location); }

void    ServerConfig::addPort(int port) { _port = port; }
void    ServerConfig::addHost(const std::string &host) { _host.push_back(host); }
void    ServerConfig::addRoot(const std::string &root) { _root.push_back(root); }
void    ServerConfig::addIndex(const std::string &index) { _index.push_back(index); }

// ----------------------------------- CONFIG --------------------------------------- //

void        Config::addServer(const ServerConfig &newServ) { this->_server.push_back(newServ); }
const       std::vector<ServerConfig> Config::getServer() const { return (this->_server); }
int         Config::getNbServer() const { return (this->_server.size()); }

/**************************************************************************************/
/*                                NON MEMBER FUNCTIONS                                */
/**************************************************************************************/

std::ostream &operator<<(std::ostream &out, const Config &rhs)
{
    out << "Configuration Servers" << std::endl;
    for (int i = 0; i < rhs.getNbServer(); i++) {
        // TODO: double for loop to catch multiple vector datas if there is some :)
        out << "Server number : " << i
        << "\nPort : " << rhs._server[i].getPort()
        << "\nHost : " << rhs._server[i].getHost()[0]
        << "\nRoot : " << rhs._server[i].getRoot()[0]
        << "\nIndex : " << rhs._server[i].getIndex()[0] << std::endl;
    }
    return (out);
}
