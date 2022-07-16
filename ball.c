#include <SDL2/SDL_render.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ball.h"
#include "game.h"

Ball ball;

const double BALL_START_X = 787.5;
const double BALL_START_Y = 487.5;

void
ballInit(void)
{
  ball.speed = 20;
  ball.x_direction = 1.0;
  ball.y_direction = 0;

  // size
  ball.drect.w = 25;
  ball.drect.h = 25;

  // positioning
  ball.drect.x = BALL_START_X;
  ball.drect.y = BALL_START_Y;
}

void
ballUpdate(void)
{
  ball.drect.x += ball.speed * ball.x_direction;
  ball.drect.y += ball.speed * ball.y_direction;
}

void
ballRender(void)
{
  if (SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255) != 0) {
    SDL_Log("Something failed: %s", SDL_GetError());
  }

  SDL_RenderDrawRect(game.renderer, &ball.drect);
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(game.renderer, &ball.drect);
}

void
ballReset(int direction)
{
  // if direction == 0 -> direction to player right
  // if direction == -1 -> direction to player left

  ball.drect.x = BALL_START_X;
  ball.drect.y = BALL_START_Y;

  if (direction > 0) {
    ball.x_direction = 1;
  }

  else {
    ball.x_direction = -1;
  }

  ball.y_direction = 0;
}

void
ballClean(void)
{
}
