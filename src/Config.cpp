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
Config::Config() : _server() {}
Config::Config(const Config &other) : _server(other._server) {}
Config::~Config() {}

/**************************************************************************************/
/*                                  MEMBER FUNCTIONS                                  */
/**************************************************************************************/

const std::vector<Server>       Config::getServer() const { return (this->_server); }
void                            Config::addServer(const Server &newServ) { this->_server.push_back(newServ); }
int                             Config::getNbServer() const { return (this->_server.size()); }
bool							Config::isValid() const {return this->_isValid;}
void							Config::setValid(bool valid) {this->_isValid = valid;}

/**************************************************************************************/
/*                                NON MEMBER FUNCTIONS                                */
/**************************************************************************************/

std::ostream &operator<<(std::ostream &out, const Config &rhs)
{
    out << "Configuration Servers" << std::endl;
    for (int i = 0; i < rhs.getNbServer(); i++) {
        out << "Server number : " << i
        << "\nPort : " << rhs.getServer()[i].getPort()
        << "\nRoot : " << rhs.getServer()[i].getRoot()[0];

        for (size_t j = 0; j < rhs.getServer()[i].getName().size(); j++)
            out << "\nName : " << rhs.getServer()[i].getName()[j];

        for (size_t j = 0; j < rhs.getServer()[i].getIndex().size(); j++)
            out << "\nIndex : " << rhs.getServer()[i].getIndex()[j];

        std::map<std::string, std::string> location = rhs.getServer()[i].getLocation();
        for (std::map<std::string, std::string>::iterator it = location.begin();
                it != location.end(); it++)
            out << "\nLocation : " << (*it).first << " || " << (*it).second;
    }
    return (out);
}
