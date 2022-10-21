/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:47:18 by trossel           #+#    #+#             */
/*   Updated: 2022/10/21 12:35:23 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Config.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

ConfigParsor::ConfigParsor(const std::string &filename)
	: _filename(filename)
{
}

ConfigParsor::~ConfigParsor()
{
}

ConfigParsor::ConfigParsor(const ConfigParsor &rhs)
{
	*this = rhs;
}

ConfigParsor &ConfigParsor::operator=(const ConfigParsor &rhs)
{
	(void)rhs;
	return (*this);
}

const std::string &ConfigParsor::getFilename() const { return this->_filename;}

Location ConfigParsor::parseLocation(const JsonObject &locObject) const
{
	std::map<std::string, RequestType> requests_map;
	requests_map["GET"] = Get;
	requests_map["POST"] = Post;
	requests_map["DELETE"] = Delete;
	requests_map["PUT"] = Put;
	requests_map["HEAD"] = Head;
	requests_map["PATCH"] = Patch;

	Location loc;

	loc.max_client_body_size = locObject.getIntOrDefault("max_client_body_size", 0);

	std::vector<std::string> disabled_requests = locObject.getArrayOrEmpty("disabled_methods").stringValues();
	for (size_t i(0); i < disabled_requests.size(); i++)
	{
		RequestType type = requests_map[disabled_requests[i]];
		loc.disableRequest(type);
	}

	loc.root_dir = locObject.getStringOrDefault("root", "");
	loc.cgi_bin = locObject.getStringOrDefault("cgi_path", "");
	if (!loc.cgi_bin.empty() && !loc.root_dir.empty())
		throw std::logic_error("Cannot have a root and a cgi_bin in a location");
	else if (!loc.cgi_bin.empty())
		loc.isCGI = true;

	return loc;
}

Server ConfigParsor::parseServer(const JsonObject &serverObject) const
{
    Server serverCfg;

    serverCfg.addPort(serverObject.getInt("port"));

	serverCfg.addAddress(serverObject.getString("address"));

	serverCfg.setRoot(serverObject.getString("root"));

	serverCfg.setCGIBin(serverObject.getString("cgi_bin"));

	std::vector<std::string> cgi_ext = serverObject.getArray("cgi_extensions").stringValues();
	for(std::vector<std::string>::iterator it = cgi_ext.begin();
		it != cgi_ext.end(); it++)
		serverCfg.addCGIExtension(*it);


	std::vector<std::string> hosts = serverObject.getArray("server_name").stringValues();
	for(std::vector<std::string>::iterator it = hosts.begin();
		it != hosts.end(); it++)
		serverCfg.addName(*it);

	// Indexes
	std::vector<std::string> indexes = serverObject.getArray("index").stringValues();
	for (std::vector<std::string>::iterator it = indexes.begin();
		it != indexes.end(); it++)
		serverCfg.addIndex(*it);

	// Default location
	Location defaultLocation = parseLocation(serverObject);
	defaultLocation.isCGI = false;
	serverCfg.addLocation("", defaultLocation);

	// Other locations
	std::vector<JsonObject> locations = serverObject.getArray("locations").ObjectValues();
	for(std::vector<JsonObject>::iterator it = locations.begin();
		it != locations.end(); it++)
	{
		std::string location_path = it->getString("location_path");
		ft::trim(location_path);
		if (location_path.empty())
			throw std::logic_error("Location error: location_path cannot be empty");
		serverCfg.addLocation(location_path, parseLocation(*it));
	}
	return serverCfg;
}

Config ConfigParsor::parse() const
{
	Config cfg;
	JsonObject json;

	cfg.setValid(false);
	try
	{
		json.parseFromFile(this->_filename);

		JsonArray serversArray = json.getArray("servers");
		std::vector<JsonObject> servers = serversArray.ObjectValues();

		std::vector<JsonObject>::const_iterator it;
		for(it = servers.begin(); it != servers.end(); it++)
			cfg.addServer(parseServer(*it));

		cfg.setValid(true);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Config = " << cfg << std::endl;
	return cfg;
}
