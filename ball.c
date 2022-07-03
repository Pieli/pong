#include <SDL2/SDL_render.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ball.h"
#include "game.h"

Ball ball;
const int BALL_SPEED = 20;

void
ballInit()
{
  ball.x_direction = 1;
  ball.y_direction = 0;

  // size
  ball.drect.w = 25;
  ball.drect.h = 25;

  // positioning
  ball.drect.x = (1000 - ball.drect.w) / 2;
  ball.drect.y = (1000 - ball.drect.h) / 2;
}

void
ballUpdate()
{
  ball.drect.x += BALL_SPEED * ball.x_direction;
  ball.drect.y += BALL_SPEED * ball.y_direction;
}

void
ballRender()
{
  if (SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255) != 0) {
    SDL_Log("Something failed: %s", SDL_GetError());
  }

  SDL_RenderDrawRect(game.renderer, &ball.drect);
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(game.renderer, &ball.drect);
}

void
ballClean()
{}
