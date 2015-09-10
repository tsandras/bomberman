#ifndef SERVER_H
#define SERVER_H

typedef int fd_t;

#define WRITE(fd, str) write(fd, str, sizeof(str)) // This should make your life easier
#define LISTEN_QUEUE_SIZE 10
#define PLAYER_COUNT 4
#define DISCONNECTED -1
#define ACCEPT_TIMEOUT_USEC 100000 // 100 ms
#define READ_TIMEOUT_USEC 100000 // 100 ms
#define READ_BUFF_SIZE 1024
#define EXIT_COMMAND "exit"

void fatal(const char * message);
fd_t listen_on(int port);
fd_t accept_one(fd_t server);
void add_client(fd_t * clients, fd_t client);
int dead_client_count(fd_t * clients);
void wait_for_players(fd_t server, fd_t * clients);

#endif