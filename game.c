#include <SDL2/SDL.h>

#include "game.h"

Game game;

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
}

void
gameUpdate()
{}

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
}

void
gameClean()
{
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
}
