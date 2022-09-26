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

void createConfig()
{
    ServerConfig server;
    Config  config;

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

void socketBinding()
{
    int server_fd, new_socket, valread = 0;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    const char* hello = "Wallow from server";
    valread += 0; // just to mute warning about valread not used

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        if ((new_socket
                     = accept(server_fd, (struct sockaddr *) &address,
                              (socklen_t * ) & addrlen))
            < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
        // closing the connected socket
        close(new_socket);
    }
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
}

void runServer()
{
    //socketInit(); : we set all the socket dependent struct and prepare a data structure holding it
    //socketBinding(); : we bind the socket with the datas aquiered before.
    //while (1) { : maybe we could catch a signal for CTRL-C to stop this, or a STOP arg
    //requestCatcher() : Catch the request and stock the request in struct data
    //requestParser() : Parse the request and store the datas parsed
    //requestTreatment() : Execute the different actions according to the request parsed
    // }
    // serverShutdown : Frees all the requiered datas/elements for the shutdown and shutsdown the serverfd
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;

    createConfig();
    runServer();

	return (0);
}
