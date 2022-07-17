#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>


typedef struct Player
{
  SDL_Rect drect;
  double speed;
} Player;

extern Player player;

void playerInit(Player *player);
void playerUpdate(Player *player);
void playerRender(Player *player);
void playerClean(Player *player);

#endif
