/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:47:18 by trossel           #+#    #+#             */
/*   Updated: 2022/10/04 16:09:08 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Json.hpp"
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

Config ConfigParsor::parse() const
{
	Config cfg;
	std::ifstream fs(_filename.c_str());

	if (!fs.good())
	{
		std::cerr << "Config file not found" << std::endl;
		cfg.setValid(false);
		return cfg;
	}
	std::string text;
	text.assign(std::istreambuf_iterator<char>(fs),
			std::istreambuf_iterator<char>());
	fs.close();
	std::cout << "text = " << text << std::endl;
	if (!isValidJsonObject(text))
	{
		std::cerr << "Config is not valid JSON" << std::endl;
		cfg.setValid(false);
		return cfg;
	}
	else
	{
		std::cerr << "Config is valid !" << std::endl;
	}




    ServerConfig server;
    server.addHost("localhost");
    server.addHost("mywebsite.com");
    server.addIndex("index.html");
    server.addIndex("index.php");
    server.addRoot("/var/www/html");
    server.addPort(8080);
    server.addLocation("location_path", "");
    server.addLocation("root", "subfolder");

	cfg.addServer(server);
	cfg.setValid(true);
	return cfg;
}
