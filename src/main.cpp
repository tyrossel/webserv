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

/*#define TRUE 1
#define FALSE 0

#define PORT    8080
#define MAXMSG  1024

int
make_socket (uint16_t port)
{
    int sock;
    int opt = 1;
    sockaddr_in name;

    *//* Create the socket. *//*
    sock = socket (AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror ("socket");
        exit (EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(sock, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    *//* Give the socket a name. *//*
    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);

    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
        perror ("bind");
        exit (EXIT_FAILURE);
    }

    return sock;
}

int read_from_client(int filedes) {
    char buffer[MAXMSG];
    int nbytes;

    nbytes = recv(filedes, buffer, MAXMSG, 0);
    if (nbytes < 0) {
        *//* Read error. *//*
        perror("read");
        exit(EXIT_FAILURE);
    } else if (nbytes == 0)
        *//* End-of-file. *//*
        return -1;
    else {
        *//* Data read. *//*
        //write(1, &buffer, strlen(buffer));
        fprintf(stderr, "Server: got message: `%s'\n", buffer);
        return 0;
    }
}

*/
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

int main(int argc, char *argv[], char *envp[]) {
    (void) argc;
    (void) argv;
    (void) envp;
    Config config;
    Server serv(8080, "0.0.0.0");
    Looper loop;

    createConfig(config);
    loop.addServer(serv);
    loop.setupLoop();
    loop.loop();

/*    int sock;
    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    socklen_t size;

    *//* Create the socket and set it up to accept connections. *//*
    sock = make_socket(PORT);
    if (listen(sock, 100) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    *//* Initialize the set of active sockets. *//*
    FD_ZERO(&active_fd_set);
    FD_SET(sock, &active_fd_set);

    while (1) {
        *//* Block until input arrives on one or more active sockets. *//*
        read_fd_set = active_fd_set;
        if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        *//* Service all the sockets with input pending. *//*
        for (i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET(i, &read_fd_set))
            {
                if (i == sock)
                {
                    *//* Connection request on original socket. *//*
                    int _new;
                    size = sizeof(clientname);
                    _new = accept(sock,
                                  (struct sockaddr *) &clientname, (socklen_t * ) &size);
                    if (_new < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    fprintf(stderr,
                            "Server: connect from host %s, port %hd.\n",
                            inet_ntoa(clientname.sin_addr),
                            ntohs(clientname.sin_port));
                    FD_SET(_new, &active_fd_set);
                }
                else
                {
                    *//* Data arriving on an already-connected socket. *//*
                    if (read_from_client(i) < 0) {
                        close(i);
                        FD_CLR(i, &active_fd_set);
                    }
                }
            }
    }*/

    return (0);
}
