/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:47:18 by trossel           #+#    #+#             */
/*   Updated: 2022/10/28 16:36:16 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Config.hpp"
#include "Utils.hpp"
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

Location ConfigParsor::parseLocation(const JsonObject &locObject, const Location &defaultLoc) const
{
	Location loc;

	// Location path
	std::string location_path = locObject.getStringOrDefault("location_path", "");
	ft::trim(location_path);
	if (location_path.empty() && !defaultLoc.path.empty())
		throw std::logic_error("Location error: location_path cannot be empty");
	loc.path = location_path;

	loc.root_dir = locObject.getStringOrDefault("root", "");

	loc.auto_index = locObject.getBoolOrDefault("auto_index", defaultLoc.auto_index);
	loc.directory_listing = locObject.getBoolOrDefault("directory_listing", defaultLoc.directory_listing);

	loc.max_client_body_size = locObject.getIntOrDefault("max_client_body_size", defaultLoc.max_client_body_size);

	// CGI_extensions
	std::vector<std::string> cgi_ext = locObject.getArrayOrEmpty("cgi_extensions").stringValues();
	if (cgi_ext.empty())
		cgi_ext = defaultLoc.cgi_extensions;
	for(std::vector<std::string>::iterator it = cgi_ext.begin();
		it != cgi_ext.end(); it++)
	{
		std::string ext = *it;
		ft::trim(ext);
		loc.cgi_extensions.push_back(ext);
	}

	// Indexes
	std::vector<std::string> indexes = locObject.getArrayOrEmpty("index").stringValues();
	if (indexes.empty())
		indexes = defaultLoc.indexes;
	for (std::vector<std::string>::iterator it = indexes.begin();
		it != indexes.end(); it++)
	{
		std::string index = *it;
		ft::trim(index);
		loc.indexes.push_back(index);
	}

	//error pages
	JsonObject errorsObj = locObject.getObjectOrEmpty("error_pages");
	std::map<std::string, JsonArray> err_map = errorsObj.getAllArrays();
	if (err_map.empty())
		loc.error_pages = defaultLoc.error_pages;
	else
	{
		for(std::map<std::string, JsonArray>::const_iterator it = err_map.begin();
				it != err_map.end(); it++)
		{
			std::vector<int> error_codes = it->second.intValues();
			for (size_t i(0); i < error_codes.size(); i++)
				loc.error_pages[error_codes[i]] = it->first;
		}
	}

	//redirections
	std::vector<JsonObject> redirObj = locObject.getArrayOrEmpty("redirections").ObjectValues();
	if (redirObj.empty())
		loc.redirections = defaultLoc.redirections;
	else
	{
		for(std::vector<JsonObject>::const_iterator it = redirObj.begin();
				it != redirObj.end(); it++)
		{
			std::string old_url = it->getString("old_url");
			std::string new_url = it->getString("new_url");
			ft::trim(old_url);
			ft::trim(new_url);
			if (old_url[0] != '/')
				old_url = loc.path + '/' + old_url;
			if (new_url[0] != '/')
				new_url = loc.path + '/' + new_url;
			loc.redirections[old_url] = new_url;
		}
	}

	std::vector<std::string> disabled_requests = locObject.getArrayOrEmpty("disabled_methods").stringValues();
	for (size_t i(0); i < disabled_requests.size(); i++)
	{
		RequestType type = ft::RequestFromString(disabled_requests[i]);
		loc.disableRequest(type);
	}

	loc.cgi_bin = locObject.getStringOrDefault("cgi_bin", "");
	if (!loc.cgi_bin.empty())
		loc.isCGI = true;
	else
		loc.cgi_bin = defaultLoc.cgi_bin;
    char *tmp = getcwd(NULL, 0);
    if (tmp)
	{
		std::string cwd = tmp;
		free(tmp);
		loc.cgi_bin = (loc.cgi_bin[0] == '/') ? loc.cgi_bin : (cwd + '/' + loc.cgi_bin);
	}

	return loc;
}

Server ConfigParsor::parseServer(const JsonObject &serverObject) const
{
    Server serverCfg;

    serverCfg.addPort(serverObject.getIntOrDefault("port", 80));

	std::string address = serverObject.getStringOrDefault("address", "0.0.0.0");
	if (address == "localhost")
		address = "127.0.0.1";
	serverCfg.addAddress(address);

	std::vector<std::string> hosts = serverObject.getArrayOrEmpty("server_name").stringValues();
	for(std::vector<std::string>::iterator it = hosts.begin();
		it != hosts.end(); it++)
		serverCfg.addName(*it);

	// Default location
	Location defaultLocation = parseLocation(serverObject, Location());
	defaultLocation.path = "/";
	defaultLocation.isCGI = false;
	serverCfg.addLocation("/", defaultLocation);

	// Other locations
	std::vector<JsonObject> locations = serverObject.getArrayOrEmpty("locations").ObjectValues();
	for(std::vector<JsonObject>::iterator it = locations.begin();
		it != locations.end(); it++)
	{
		Location loc = parseLocation(*it, defaultLocation);
		serverCfg.addLocation(loc.path, loc);
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
    std::cout << cfg << std::endl;
	return cfg;
}
