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
ServerConfig::ServerConfig(int port, std::string host, std::string root, std::string index, std::map<std::string, std::string> location) : _port(port), _host(host), _root(root), _index(index), _location(location) {}
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

const int                                   ServerConfig::getPort() const { return (this->_port) }
const std::string                           ServerConfig::getHost() const { return (this->_host) }
const std::string                           ServerConfig::getRoot() const { return (this->_root) }
const std::string                           ServerConfig::getIndex() const { return (this->_index) }
const std::map<std::string, std::string>    ServerConfig::getLocation() const { return (this->_location) }

// ----------------------------------- CONFIG --------------------------------------- //

void        Config::addServer(const ServerConfig &newServ) const { this->_server.push_back(newServ); }
const       std::vector<ServerConfig> Config::getServer() const { return (this->_server); }
const int   Config::getNbServer() const { return (this->_server.size()); }

/**************************************************************************************/
/*                                NON MEMBER FUNCTIONS                                */
/**************************************************************************************/

std::ostream &Config::operator<<(std::ostream &out, const Config &rhs)
{
    out << "Configuration Servers" << std::endl;
    for (unsigned int i = 0; i < rhs.getNbServer(); i++) {
        out << "Server number : " << i << "\nPort : " << rhs[i].getPort()
        << "\nHost : " << rhs[i].getHost()
        << "\nRoot : " << rhs[i].getRoot()
        << "\nIndex : " << rhs[i].getIndex() << std::endl;
    }
}