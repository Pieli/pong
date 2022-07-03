#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "font.h"
#include "game.h"

const char FILE_PATH[] = "JetBrainsMono-Regular.ttf";
const int PT = 18;
SDL_Color FORE_COLOR = { 0xFF, 0xFF, 0xFF, 0 };
SDL_Color BACK_COLOR = { 0x00, 0x00, 0x00, 0 };

Font font;

void
fontInit()
{
  if (TTF_Init() != 0) {
    SDL_Log("Couldn't initialize TTF: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }

  font.font = TTF_OpenFont(FILE_PATH, PT);

  if (font.font == NULL) {
    SDL_Log(
      "Couldn't load %d pt font from %s: %s\n", PT, FILE_PATH, SDL_GetError());
    gameClean();
    exit(1);
  }

  font.renderstyle = TTF_STYLE_NORMAL;
  font.outline = 0;
  font.hinting = TTF_HINTING_NORMAL;
  font.kerning = 1;

  TTF_SetFontStyle(font.font, font.renderstyle);
  TTF_SetFontOutline(font.font, font.outline);
  TTF_SetFontKerning(font.font, font.kerning);
  TTF_SetFontHinting(font.font, font.hinting);
}

void
fontRender(Scene* scene)
{
  SDL_RenderCopy(
    game.renderer, scene->messageTexture, NULL, &(scene->messageRect));
}

void
fontUpdate(const char* message, Scene* scene)
{
  SDL_Surface* tmpText;

  tmpText = TTF_RenderUTF8_Solid(font.font, message, FORE_COLOR);

  if (tmpText == NULL) {
    SDL_Log("Couldn't render text: %s\n", SDL_GetError());
    TTF_CloseFont(font.font);
    SDL_FreeSurface(tmpText);
    TTF_Quit();
    exit(1);
  }

  scene->messageRect.x = (game.wind_w - tmpText->w) / 2;
  scene->messageRect.y = (game.wind_h - tmpText->h) / 2;
  scene->messageRect.w = tmpText->w;
  scene->messageRect.h = tmpText->h;
  scene->messageTexture = SDL_CreateTextureFromSurface(game.renderer, tmpText);

  SDL_FreeSurface(tmpText);
}

void
fontClean(Scene* scene)
{
  TTF_CloseFont(font.font);
  SDL_DestroyTexture(scene->messageTexture);
  TTF_Quit();
}
