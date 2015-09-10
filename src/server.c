#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <netinet/in.h>
#include <stdint.h>

#include "server.h"

void fatal(const char * message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

fd_t listen_on(int port)
{
    fd_t server;
    struct sockaddr_in serv_addr;

    // Initialize server information
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    // Create the server socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1)
        fatal("Could not create a socket for the server");
    // Bind it to the information and listen
    if (bind(server, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        fatal("Error binding the server socket");
    if (listen(server, LISTEN_QUEUE_SIZE) == -1)
        fatal("Error listening for connections");
    return (server);
}

fd_t accept_one(fd_t server)
{
    fd_t client;
    fd_set read_fds;
    struct timeval accept_timeout = { 0, ACCEPT_TIMEOUT_USEC };

    // Non blocking accept
    client = DISCONNECTED;
    FD_ZERO(&read_fds);
    FD_SET(server, &read_fds);
    if (select(server + 1, &read_fds, 0, 0, &accept_timeout) > 0)
    {
        client = accept(server, 0, 0);
        if (client == -1)
            fatal("Error while accepting a client");
        printf("new client num: %d\n", client);
    }
    return (client);
}

void add_client(fd_t * clients, fd_t client)
{
    int i;

    for (i = 0; i < PLAYER_COUNT; ++i)
        if (clients[i] == DISCONNECTED)
        {
            clients[i] = client;
            break ;
        }
}

int dead_client_count(fd_t * clients)
{
    int i, max_fd, dead;
    fd_set read_fds;
    struct timeval read_timeout = { 0, READ_TIMEOUT_USEC };

    // Filling the read set
    FD_ZERO(&read_fds);
    max_fd = 0;
    for (i = 0; i < PLAYER_COUNT; ++i)
        if (clients[i] != DISCONNECTED)
        {
            FD_SET(clients[i], &read_fds);
            if (clients[i] > max_fd)
                max_fd = clients[i];
        }

    // Count dead clients (Either disconnect or exit request)
    dead = 0;
    if (select(max_fd + 1, &read_fds, 0, 0, &read_timeout) > 0)
    {
        for (i = 0; i < PLAYER_COUNT; ++i)
        {
            if (clients[i] != DISCONNECTED && FD_ISSET(clients[i], &read_fds))
            {
                int read_size;
                char buff[READ_BUFF_SIZE + 1] = { 0 };

                read_size = read(clients[i], &buff, READ_BUFF_SIZE);
                if (read_size <= 0 || !strcmp(buff, EXIT_COMMAND))
                {
                    printf("lost client num: %d\n", clients[i]);
                    close(clients[i]);
                    clients[i] = DISCONNECTED;
                    ++dead;
                }
            }
        }
    }
    return (dead);
}

void wait_for_players(fd_t server, fd_t * clients)
{
    int client_count = 0;

    // Marking everyone as disconnected
    memset(clients, DISCONNECTED, sizeof(fd_t) * PLAYER_COUNT);
    // Waiting for everyone
    while (client_count < PLAYER_COUNT)
    {
        // Trying to accept a new client
        fd_t new_client = accept_one(server);
        if (new_client != DISCONNECTED)
        {
            add_client(clients, new_client);
            ++client_count;
        }
        // Checking for clients who left or got disconnected
        client_count -= dead_client_count(clients);
    }
}