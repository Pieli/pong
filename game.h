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
extern int winner;

void gameInit(void);
void gameUpdate(void);
void gameRender(void);
void gameClean(void);
void gameInputAction(SDL_Event event);

int gameCheckWinner(void);
void gameWinnerScreenRender(int player_num);

void gamePauseRender(void);
void gameAddScorePlayer1(void);
void gameAddScorePlayer2(void);
void gameResetScores(void);


#endif
