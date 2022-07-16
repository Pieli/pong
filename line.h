#ifndef LINEH_H
#define LINEH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

extern SDL_Rect* rectangles;

void lineInit(void);
void lineRender(void);
void lineClean(void);

#endif
