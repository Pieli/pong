
#include <SDL2/SDL.h>

#include <SDL2/SDL_rect.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "line.h"

const int GAP_SIZE = 20;
const int LINE_W = 10;
const int LINE_H = 80;

SDL_Rect* rectangles;
unsigned int number_of_spaces = 0;

void
lineInit(void)
{
  double gap = GAP_SIZE;

  double missing_space = game.wind_h - gap - LINE_H;
  number_of_spaces = (int)(missing_space / (gap + LINE_H));
  double epsilon = (missing_space / (gap + LINE_H)) - number_of_spaces;

  if (epsilon > 0.5)
    gap += epsilon / number_of_spaces;

  else
    gap -= epsilon / number_of_spaces;

  rectangles = (SDL_Rect*)malloc((number_of_spaces + 2) * sizeof(SDL_Rect));

  if (rectangles == NULL) {
    SDL_Log("Some error occured durring malloc of sdl_rect");
    exit(1);
  }

  double x_coord = (game.wind_w - LINE_W) / 2.0;

  rectangles[0].x = x_coord;
  rectangles[0].y = game.wind_h - (LINE_H / 2);
  rectangles[0].w = LINE_W;
  rectangles[0].h = LINE_H / 2;

  double previous_y = rectangles[0].y;

  for (int i = 1; i < number_of_spaces + 1; i++) {
    rectangles[i].x = x_coord;
    rectangles[i].y = previous_y - gap - LINE_H;
    rectangles[i].w = LINE_W;
    rectangles[i].h = LINE_H;

    previous_y = rectangles[i].y;
  }

  rectangles[number_of_spaces + 1].x = x_coord;
  rectangles[number_of_spaces + 1].y = 0;
  rectangles[number_of_spaces + 1].w = LINE_W;
  rectangles[number_of_spaces + 1].h = LINE_H / 2;
}

void
lineRender(void)
{
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
  if (SDL_RenderDrawRects(game.renderer, rectangles, number_of_spaces + 2) !=
      0) {
    SDL_Log("error during render draw rects: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
  SDL_RenderFillRects(game.renderer, rectangles, number_of_spaces + 2);
}

void
lineClean(void)
{
  if (rectangles) {
    free(rectangles);
  }
}
