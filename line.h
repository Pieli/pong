#ifndef LINEH_H
#define LINEH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

extern SDL_Rect* rectangles;

void initLine();
void renderLine();
void cleanLine();

#endif
