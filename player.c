// #include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "player.h"

const int SPEED = 3;

Player player;

void
playerInit()
{
  // inital texture
  player.drect.w = 100;
  player.drect.h = 100;

  //  scaling
  player.drect.w /= 2;
  player.drect.h /= 1;

  // positioning
  player.drect.x = (game.wind_w - player.drect.w) / 2;
  player.drect.y = (game.wind_h - player.drect.h) / 2;
}

void
playerUpdate()
{
  // player.drect.x += SPEED * player.x_direction;
  // player.drect.y += SPEED * player.y_direction;
}

void
playerRender()
{
  if (SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255) != 0) {
    SDL_Log("Something failed: %s", SDL_GetError());
  }
  SDL_RenderDrawRect(game.renderer, &player.drect);
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(game.renderer, &player.drect);
}

void
playerClean()
{}
