#include <SDL2/SDL_render.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ball.h"
#include "game.h"

Ball ball;

void
ballInit(void)
{
  double ball_start_x = (game.wind_w - ball.drect.w) / 2.0;
  double ball_start_y = (game.wind_h - ball.drect.h) / 2.0;

  ball.speed = 20;
  ball.x_direction = 1.0;
  ball.y_direction = 0;

  // size
  ball.drect.w = game.wind_h * 0.025;
  ball.drect.h = ball.drect.w;

  // positioning
  ball.drect.x = ball_start_x;
  ball.drect.y = ball_start_y;
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
  double ball_start_x = (game.wind_w - ball.drect.w) / 2.0;
  double ball_start_y = (game.wind_h - ball.drect.h) / 2.0;

  ball.drect.x = ball_start_x;
  ball.drect.y = ball_start_y;

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
