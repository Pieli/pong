#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>


typedef struct Ball
{
  // SDL_Rect srect;
  SDL_Rect drect;
  double y_direction;
  double x_direction;
} Ball;


extern Ball ball;

void ballInit();
void ballUpdate();
void ballRender();
void ballClean();

#endif
