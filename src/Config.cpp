#include "Config.hpp"
#include <unistd.h>
#include <vector>
#include <set>

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
Config::Config() : _server() {}

Config::Config(const Config &other) : _server(other._server) {}

Config::~Config() {}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

const std::vector <Server> Config::getServer() const { return (this->_server); }

void Config::addServer(const Server &newServ) { this->_server.push_back(newServ); }

int Config::getNbServer() const { return (this->_server.size()); }

void Config::setValid(bool valid) { this->_isValid = valid; }

bool Config::isValid() const {
    std::set<int> portList;

    if (_server.empty())
	{
		std::cerr << "No server specified" << std::endl;
        return false;
	}
    for (std::vector<Server>::const_iterator it = _server.begin();
         it != _server.end(); it++) {
        if (it->getPort() < 1 || it->getPort() > 65535) {
            std::cerr << "Port is not valid: " << it->getPort() << std::endl;
            return false;
        }
        if (portList.find(it->getPort()) != portList.end()) {
            std::cerr << "Port is already used by a previous server : " <<
                      it->getPort() << std::endl;
            return false;
        }
        portList.insert(it->getPort());

        if (inet_addr(it->getAddress().c_str()) == INADDR_NONE) {
            std::cerr << "Address is not valid: " << it->getAddress() << std::endl;
            return false;
        }

        const std::map <std::string, Location> locations = it->getLocations();
        for (std::map<std::string, Location>::const_iterator it = locations.begin();
             it != locations.end(); it++) {
            const std::string &loc_path = it->first;
            const Location &loc = it->second;

            if (loc_path.empty()) {
                std::cerr << "Location error: location_path cannot be empty" << std::endl;
                return false;
            }
            if (loc_path == "/" && loc.root_dir.empty()) {
                std::cerr << "Server error: cannot have an empty root_dir" << std::endl;
                return false;
            }
            if (loc.max_client_body_size < 0) {
                std::cerr << "Location '" + loc_path + "' error: max_client_body_size must be positive" << std::endl;
                return false;
            }
            if (loc.isCGI || (loc.path == "/" && !loc.cgi_bin.empty())) {
                if (access(loc.cgi_bin.c_str(), F_OK)) {
                    std::cerr << "CGI error: '" << loc.cgi_bin << "' cannot be found" << std::endl;
                    return false;
                }
                if (access(loc.cgi_bin.c_str(), X_OK)) {
                    std::cerr << "CGI error: " << loc.cgi_bin << " cannot be executed" << std::endl;
                    return false;
                }
            }
            if (!loc.isCGI) {
                if (loc.root_dir.empty()) {
                    std::cerr << "Location error: root_dir cannot be empty if not a CGI proxy" << std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}
/**************************************************************************************/
/*                                NON MEMBER FUNCTIONS                                */
/**************************************************************************************/

std::ostream &operator<<(std::ostream &out, const Config &rhs) {
    if (VERBOSE) {
        out << ft::timestamp(TIMESTAMP_FORMAT) << "[Configuration]" << std::endl;
		std::cout << CYAN "Keepalive-timeout:" RESET << rhs.getTimeout() << std::endl;
        for (int i = 0; i < rhs.getNbServer(); i++) {
            out << CYAN << "Server number : " << RESET << i
                << YELLOW << "\nAddress : " << RESET << rhs.getServer()[i].getAddress()
                << YELLOW << "\nPort : " << RESET << rhs.getServer()[i].getPort();

            out << YELLOW << "\nName : " << RESET;
            for (size_t j = 0; j < rhs.getServer()[i].getName().size(); j++)
                out << rhs.getServer()[i].getName()[j] << " ";

            std::map <std::string, Location> location = rhs.getServer()[i].getLocations();
            for (std::map<std::string, Location>::iterator it = location.begin();
                 it != location.end(); it++) {
                out << YELLOW << "\nLocation: " << RESET << it->first << std::endl << it->second;
            }
        }
    }
    else
    {
        for (int i = 0; i < rhs.getNbServer(); i++) {
            out << YELLOW << ft::timestamp(TIMESTAMP_FORMAT) << "Listening on [" << rhs.getServer()[i].getAddress() << ":" << rhs.getServer()[i].getPort() << "]\n" << RESET;
        }
    }

    return (out);
}
