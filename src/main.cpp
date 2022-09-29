/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trossel <trossel@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:21:50 by trossel           #+#    #+#             */
/*   Updated: 2022/09/20 15:23:05 by trossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Looper.hpp"

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

int main(int argc, char *argv[], char *envp[]) {
    (void) argc;
    (void) argv;
    (void) envp;

	if (argc < 2 || argc > 2)
	{
		std::cerr << "Usage: ./webserv <config file>" << std::endl;
//		exit(1);
	}
//	ConfigParsor parsor("
    Config config;
    Server serv(8080, "0.0.0.0");
    Looper loop;

    createConfig(config);
    loop.addServer(serv);
    loop.setupLoop();
    loop.loop();

    return (0);
}
