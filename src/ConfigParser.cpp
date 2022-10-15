/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:47:18 by trossel           #+#    #+#             */
/*   Updated: 2022/10/15 17:27:32 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Config.hpp"
#include <fstream>
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

const std::string &ConfigParsor::getFilename() const { return
	this->_filename;}


Server ConfigParsor::parseServer(const JsonObject &serverObject) const
{
    Server serverCfg;

	(void)serverObject;

    serverCfg.addPort(serverObject.getInt("port"));

	serverCfg.addAddress(serverObject.getString("address"));

    serverCfg.addRoot(serverObject.getString("root"));

	std::vector<std::string> hosts = serverObject.getArray("server_name").stringValues();
	for(std::vector<std::string>::iterator it = hosts.begin();
		it != hosts.end(); it++)
		serverCfg.addName(*it);

	// Indexes
	std::vector<std::string> indexes = serverObject.getArray("index").stringValues();
	for (std::vector<std::string>::iterator it = indexes.begin();
		it != indexes.end(); it++)
		serverCfg.addIndex(*it);

	// Locations
	std::vector<JsonObject> locations = serverObject.getArray("location").ObjectValues();
	for(std::vector<JsonObject>::iterator it = locations.begin();
		it != locations.end(); it++)
	{
		// TODO: handle CGI config
		std::string location_path = it->getString("location_path");
		std::string root = it->getString("root");
		serverCfg.addLocation(location_path, root);
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
