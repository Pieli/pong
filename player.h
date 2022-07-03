#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>


typedef struct Player
{
  SDL_Rect drect;
} Player;


void playerInit();
void playerUpdate();
void playerRender();
void playerClean();

#endif
