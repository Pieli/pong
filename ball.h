#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>


typedef struct Ball
{
  // SDL_Rect srect;
  SDL_Rect drect;
  double speed;
  double y_direction;
  double x_direction;
} Ball;


extern Ball ball;

void ballInit(void);
void ballUpdate(void);
void ballRender(void);
void ballClean(void);
void ballReset(int direction);

#endif
