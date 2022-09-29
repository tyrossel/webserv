/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:21:50 by trossel           #+#    #+#             */
/*   Updated: 2022/09/26 17:01:11 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Looper.hpp"
#include "conf/parse.hpp"

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

//ServerConfig getConfig(const std::string &filename)
//{
//	ConfigParsor parsor(filename);
//	try
//	{
//		ServerConfig conf = parsor.parse();
//		return conf;
//
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << e.what() << std::endl;
//	}
//}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;

	if (argc < 2 || argc > 2)
	{
		std::cerr << "Usage: ./webserv <config file>" << std::endl;
//		exit(1);
	}
//	ConfigParsor parsor("
    Config config;
    createConfig(config);

    Server serv2(2020, "0.0.0.0");
    Server serv3(3030, "0.0.0.0");
    Server serv4(4040, "0.0.0.0");
    Server serv5(5050, "0.0.0.0");
    Server serv6(6060, "0.0.0.0");
    Server serv(8080, "0.0.0.0");



    Looper loop;

    loop.addServer(serv);
    loop.addServer(serv2);
    loop.addServer(serv3);
    loop.addServer(serv4);
    loop.addServer(serv5);
    loop.addServer(serv6);
    loop.setupLoop();
    loop.loop();

    return (0);
}
