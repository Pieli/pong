#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "font.h"
#include "game.h"

const char FILE_PATH[] = "JetBrainsMono-Regular.ttf";

SDL_Color FORE_COLOR = { 0xFF, 0xFF, 0xFF, 0 };
SDL_Color BACK_COLOR = { 0x00, 0x00, 0x00, 0 };

Font font;
Font font_small;

void
fontInit(Font* font, unsigned int size)
{
  if (TTF_Init() != 0) {
    SDL_Log("Couldn't initialize TTF: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }

  font->font = TTF_OpenFont(FILE_PATH, size);

  if (font->font == NULL) {
    SDL_Log("Couldn't load %d pt font from %s: %s\n",
            size,
            FILE_PATH,
            SDL_GetError());
    gameClean();
    exit(1);
  }
  font->outline = 0;
  font->hinting = TTF_HINTING_NORMAL;
  font->kerning = 1;

  TTF_SetFontStyle(font->font, font->renderstyle);
  TTF_SetFontOutline(font->font, font->outline);
  TTF_SetFontKerning(font->font, font->kerning);
  TTF_SetFontHinting(font->font, font->hinting);
}

void
fontRender(Scene* scene)
{
  SDL_RenderCopy(
    game.renderer, scene->messageTexture, NULL, &(scene->messageRect));
}

void
fontUpdate(Scene* scene,
           Font font,
           const char* message,
           double relative_x,
           double relative_y)
{
  SDL_Surface* tmpText;

  tmpText = TTF_RenderUTF8_Shaded(font.font, message, FORE_COLOR, BACK_COLOR);

  if (tmpText == NULL) {
    SDL_Log("Couldn't render text: %s\n", SDL_GetError());
    TTF_CloseFont(font.font);
    SDL_FreeSurface(tmpText);
    TTF_Quit();
    exit(1);
  }

  scene->messageRect.x = (game.wind_w - tmpText->w) / (1.0 / relative_x);
  scene->messageRect.y = (game.wind_h - tmpText->h) / (1.0 / relative_y);
  scene->messageRect.w = tmpText->w;
  scene->messageRect.h = tmpText->h;
  scene->messageTexture = SDL_CreateTextureFromSurface(game.renderer, tmpText);

  SDL_FreeSurface(tmpText);
}

void
fontClean(Scene* scene)
{
  if (font.font != NULL) {
    TTF_CloseFont(font.font);
    font.font = NULL;
  }

  SDL_DestroyTexture(scene->messageTexture);
  TTF_Quit();
}
