#include <SDL2/SDL.h>

#include "font.h"
#include "game.h"
#include "line.h"

Game game;

int paused = 0;

Scene scene_player_1;
Scene scene_player_2;
Scene paused_text;

unsigned int score_p_1 = 0;
unsigned int score_p_2 = 0;

const double POS_SCORE_P_1_X = (1.0 / 3);
const double POS_SCORE_P_1_Y = (1.0 / 3);

const double POS_SCORE_P_2_X = (2.0 / 3);
const double POS_SCORE_P_2_Y = (1.0 / 3);

void
gameInit()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("error during init of SDL: %s\n", SDL_GetError());
    return;
  }

  game.wind_w = 1600;
  game.wind_h = 1000;

  game.window = SDL_CreateWindow("pong game",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 game.wind_w,
                                 game.wind_h,
                                 SDL_WINDOW_OPENGL);

  game.renderer = SDL_CreateRenderer(
    game.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  lineInit();
  fontInit();

  fontUpdate(&scene_player_1, "0", POS_SCORE_P_1_X, POS_SCORE_P_1_Y);
  fontUpdate(&scene_player_2, "0", POS_SCORE_P_2_X, POS_SCORE_P_2_Y);
}

void
gameUpdate()
{
}

void
gameRender()
{
  // set color to white
  SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);

  // clears the screen
  if (SDL_RenderClear(game.renderer) != 0) {
    SDL_Log("error during RenderClear: %s\n", SDL_GetError());
    exit(1);
  }

  lineRender();
  fontRender(&scene_player_1);
  fontRender(&scene_player_2);
}

void
gameClean()
{
  lineClean();
  fontClean(&scene_player_1);
  fontClean(&scene_player_2);

  SDL_DestroyRenderer(game.renderer);
  SDL_DestroyWindow(game.window);
}

void
gameInputAction(SDL_Event event)
{
  if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
    game.wind_w = event.window.data1;
    game.wind_h = event.window.data2;
  }

  if (event.key.type == SDL_KEYDOWN &&
      event.key.keysym.scancode == SDL_SCANCODE_P) {
    if (paused)
      paused = 0;
    else
      paused = 1;
  }
}

void
gamePause()
{
  // render font text
}

void
gameAddScorePlayer1()
{
  char num[12];
  sprintf(num, "%d", ++score_p_1);
  fontUpdate(&scene_player_1, num, POS_SCORE_P_1_X, POS_SCORE_P_1_Y);
}

void
gameAddScorePlayer2()
{
  char num[12];
  sprintf(num, "%d", ++score_p_2);
  fontUpdate(&scene_player_2, num, POS_SCORE_P_2_X, POS_SCORE_P_2_Y);
}
