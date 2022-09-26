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

#include "Config.hpp"
#include "IOLoop.hpp"
#include "ServerHandler.hpp"
#include "EchoHandler.hpp"

void createConfig(Config  &config)
{
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
    std::cout << config << std::endl;
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;
    Config  config;
    IOLoop looper;

    createConfig(config);
    ServerHandler(config.getServer()[0].getPort());

    looper.start();

	return (0);
}
