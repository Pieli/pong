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

void gameInit(void);
void gameUpdate(void);
void gameRender(void);
void gameClean(void);
void gameInputAction(SDL_Event event);

void gamePauseRender(void);
void gameAddScorePlayer1(void);
void gameAddScorePlayer2(void);


#endif
