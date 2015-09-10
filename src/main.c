#include "struct.h"
#include "server.h"
#include "lib.h"

int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        fd_t server;
        fd_t clients[PLAYER_COUNT];

        server = listen_on(my_getnbr(argv[1]));
        wait_for_players(server, clients);
        // From here, all the players are connected
    }
}

