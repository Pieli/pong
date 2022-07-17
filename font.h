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
extern Font font_small;

void fontInit(Font* font, unsigned int size);
void fontRender(Scene* scene);
void fontUpdate(Scene* scene,
           Font font,
           const char* message,
           double relative_x,
           double relative_y);
void fontClean(Scene* scene);

#endif
