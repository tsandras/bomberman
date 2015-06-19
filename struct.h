#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct  s_client_request
{
  unsigned int  magic;                  /* Un magic number commun entre le client et le serveur */
  int           x_pos;                  /* La position x souhaitée par le client */
  int           y_pos;                  /* La position y souhaitée par le client */
  int           dir;                    /* La direction souhaitée par le client */
  int           command;                /* Une commande du client (0 : Ne rien faire / 1 : Poser une bombe) */
  int           speed;                  /* La vitesse du joueur */
  int           ckecksum;               /* Un checksum simple */
} t_client_request;

typedef struct s_player_infos
{
  char  connected;
  char  alive;
  int   x_pos;
  int   y_pos;
  int   current_dir;
  int   current_speed;
  int   max_speed;
  int   bombs_left;
  int   bombs_capacity;
  int   frags;
} t_player_infos;

typedef char t_map[100];

typedef struct          s_game
{
    t_player_infos        player_infos[4];
    t_map                 map;
}                       t_game;
