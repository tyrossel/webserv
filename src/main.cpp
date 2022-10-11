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
    Looper loop(cfg);
    loop.setupLoop();
    loop.loop();

    return (0);
}
