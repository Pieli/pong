// #include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "player.h"

Player player;

void
playerInit(Player* player)
{
  // inital texture
  player->drect.w = game.wind_w * 0.02;
  player->drect.h = game.wind_h * 0.2;

  // positioning
  player->drect.x = (game.wind_w - player->drect.w) / 2;
  player->drect.y = (game.wind_h - player->drect.h) / 2;

  if (game.wind_w > 1000)
    player->speed = 15;
  else
    player->speed = 10;
}

void
playerUpdate(Player* player)
{
}

void
playerRender(Player* player)
{
  if (SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255) != 0) {
    SDL_Log("Something failed: %s", SDL_GetError());
  }
  SDL_RenderDrawRect(game.renderer, &(player->drect));
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(game.renderer, &(player->drect));
}

void
playerClean(Player* player)
{
}
