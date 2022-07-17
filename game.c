#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>

#include "font.h"
#include "game.h"
#include "line.h"

Game game;

int paused = 0;
int winner = 0;

Scene scene_player_1;
Scene scene_player_2;
Scene paused_text;
Scene winner_text_1;
Scene winner_text_2;

unsigned int score_p_1 = 0;
unsigned int score_p_2 = 0;

const double POS_SCORE_P_1_X = (1.0 / 3);
const double POS_SCORE_P_1_Y = (1.0 / 3);

const double POS_SCORE_P_2_X = (2.0 / 3);
const double POS_SCORE_P_2_Y = (1.0 / 3);

void
gameInit(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("error during init of SDL: %s\n", SDL_GetError());
    return;
  }

  SDL_DisplayMode dm;

  SDL_GetDesktopDisplayMode(0, &dm);

  game.wind_w = dm.w * 0.4;
  game.wind_h = (game.wind_w / 16) * 9;

  game.window = SDL_CreateWindow("pong game",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 game.wind_w,
                                 game.wind_h,
                                 SDL_WINDOW_OPENGL);

  game.renderer = SDL_CreateRenderer(
    game.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  lineInit();

  if (game.wind_w < 1000) {
    fontInit(&font, 72);
    fontInit(&font_small, 50);
  } else {
    fontInit(&font, 108);
    fontInit(&font_small, 54);
  }

  fontUpdate(&scene_player_1, font, "0", POS_SCORE_P_1_X, POS_SCORE_P_1_Y);
  fontUpdate(&scene_player_2, font, "0", POS_SCORE_P_2_X, POS_SCORE_P_2_Y);
}

void
gameUpdate(void)
{
}

void
gameRender(void)
{
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
gameClean(void)
{
  lineClean();
  fontClean(&scene_player_1);
  fontClean(&scene_player_2);
  fontClean(&paused_text);
  fontClean(&winner_text_1);
  fontClean(&winner_text_2);

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

  if (event.key.type == SDL_KEYDOWN) {
    if (event.key.keysym.scancode == SDL_SCANCODE_P && !paused) {
      paused = 1;
    } else {
      paused = 0;
    }
  }
}

void
gamePauseRender(void)
{
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
  fontUpdate(&paused_text, font_small, "Paused", 0.02, 0.02);
  fontRender(&paused_text);
}

int
gameCheckWinner(void)
{
  if (score_p_1 == 10) {
    return 1;
  }
  if (score_p_2 == 10) {
    return 2;
  } else {
    return 0;
  }
}

void
gameWinnerScreenRender(int player_num)
{
  SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
  SDL_RenderClear(game.renderer);
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);

  char text[15];
  sprintf(text, "Player %d won!", player_num);

  fontUpdate(&winner_text_1, font, text, 0.5, 0.3);
  fontUpdate(&winner_text_2, font_small, "Press 'r' to restart", 0.5, 0.75);
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
  fontRender(&winner_text_1);
  fontRender(&winner_text_2);

  SDL_RenderPresent(game.renderer);
}

void
gameAddScorePlayer1(void)
{
  char num[12];
  sprintf(num, "%d", ++score_p_1);
  fontUpdate(&scene_player_1, font, num, POS_SCORE_P_1_X, POS_SCORE_P_1_Y);
}

void
gameAddScorePlayer2(void)
{
  char num[12];
  sprintf(num, "%d", ++score_p_2);
  fontUpdate(&scene_player_2, font, num, POS_SCORE_P_2_X, POS_SCORE_P_2_Y);
}

void
gameResetScores(void)
{
  score_p_1 = 0;
  score_p_2 = 0;
  fontUpdate(&scene_player_1, font, "0", POS_SCORE_P_1_X, POS_SCORE_P_1_Y);
  fontUpdate(&scene_player_2, font, "0", POS_SCORE_P_2_X, POS_SCORE_P_2_Y);
}
