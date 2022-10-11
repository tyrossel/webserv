/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:21:50 by trossel           #+#    #+#             */
/*   Updated: 2022/10/09 16:58:18 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Looper.hpp"
#include "ConfigParser.hpp"

void createConfig(Config &config) {
    ServerConfig server;

    server.addHost("localhost");
    server.addHost("mywebsite.com");
    server.addIndex("index.html");
    server.addIndex("index.php");
    server.addRoot("/var/www/html");
    server.addPort(8080);
    server.addLocation("location_path", "");
    server.addLocation("root", "subfolder");

    config.addServer(server);
    //std::cout << config << std::endl;
}

Config getConfig(const std::string &filename)
{
	ConfigParsor parsor(filename);
	Config conf = parsor.parse();
	return conf;
}


int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;

	if (argc < 2 || argc > 2)
	{
		std::cerr << "Usage: ./webserv <config file>" << std::endl;
		exit(1);
	}
	Config cfg = getConfig(argv[1]);
	if (!cfg.isValid())
	{
		std::cerr << "Error in config file." << std::endl;
		return 1;
	}
    Server()
    Looper loop;

    //
    // loop.addServer(serv);
    // loop.addServer(serv2);
    // loop.addServer(serv3);
    // loop.addServer(serv4);
    // loop.addServer(serv5);
    // loop.addServer(serv6);
    // loop.addServer(serv7);
    //
    // loop.setupLoop();
    // loop.loop();

    return (0);
}
