#include "Config.hpp"
#include <vector>

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
void							Config::setValid(bool valid) {this->_isValid = valid;}

bool							Config::isValid() const
{
	if (_server.empty())
		return false;
	for (std::vector<Server>::const_iterator it = _server.begin();
			it != _server.end(); it++)
	{
		if (it->getPort() < 1 || it->getPort() > 65535)
		{
			std::cerr << "Port is not valid: " << it->getPort() << std::endl;
			return false;
		}

		std::vector<std::string> names = it->getName();
		if (names.empty())
			return false;
		// for (std::vector<std::string>::const_iterator it(names.begin());
		// it != names.end(); it++)
		// {
			// if (!ft::is_url(*it))
			// 	return false;
		// }
		if (it->getRoot().empty())
			return false;

		const std::map<std::string, Location> locations = it->getLocations();
		for(std::map<std::string, Location>::const_iterator it = locations.begin();
				it != locations.end(); it++)
		{
			const std::string &loc_path = it->first;
			const Location &loc = it->second;

			if (loc_path == "/" && loc.root_dir.empty())
				throw std::logic_error("Server error: cannot have an empty root_dir");
			if (loc.isCGI && !loc.root_dir.empty())
				throw std::logic_error("Location error: Cannot have a root and a cgi_bin");
			// TODO TYR: Check if CGI, that cgi_bin is set and executable
		}
	}
	return true;
}
/**************************************************************************************/
/*                                NON MEMBER FUNCTIONS                                */
/**************************************************************************************/

std::ostream &operator<<(std::ostream &out, const Config &rhs)
{
    out << "Configuration Servers" << std::endl;
    for (int i = 0; i < rhs.getNbServer(); i++) {
        out << YELLOW << "Server number : " << RESET << i
        << YELLOW << "\nPort : " << RESET << rhs.getServer()[i].getPort();

		out << YELLOW << "\nName : " << RESET;
        for (size_t j = 0; j < rhs.getServer()[i].getName().size(); j++)
            out << rhs.getServer()[i].getName()[j] << " ";

        std::map<std::string, Location> location = rhs.getServer()[i].getLocations();
        for (std::map<std::string, Location>::iterator it = location.begin();
                it != location.end(); it++)
		{
			out << YELLOW << "\nLocation: " << RESET << it->first << std::endl << it->second;
		}
    }
    return (out);
}
