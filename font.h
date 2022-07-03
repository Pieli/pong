#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct Font
{
  int renderstyle;
  int outline;
  int hinting;
  int kerning;
  TTF_Font* font;
} Font;

typedef struct Scene
{
  SDL_Texture* messageTexture;
  SDL_Rect messageRect;
} Scene;

extern Font font;

void fontInit();
void fontRender();
void fontUpdate();
void fontClean();

#endif
