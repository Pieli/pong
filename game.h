#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

typedef struct Game {
  SDL_Window* window;
  SDL_Renderer* renderer;
  int wind_w;
  int wind_h;
} Game;


extern Game game;
extern int paused;

void gameInit();
void gameUpdate();
void gameRender();
void gameClean();
void gameInputAction();

void gamePause();
void gameAddScorePlayer1();
void gameAddScorePlayer2();


#endif
