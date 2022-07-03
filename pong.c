#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ball.h"
#include "game.h"
#include "player.h"

void
init();
void
clean();
void
render();
void
logicWalls();
void
logicPlayer();
void
handleEvent(SDL_Event event);
void
update();

Player leftPlayer;
Player rightPlayer;

const int PLAYER_SPEED = 10;

int
main(int argc, char* args[])
{
  Uint64 start, end;
  float elapsedMS;

  init();

  // TODO rewrite to own function, called when point is scored
  // inital positioning
  leftPlayer.drect.x = 30;
  leftPlayer.drect.y = 500;
  rightPlayer.drect.x = 1500;
  rightPlayer.drect.y = 500;

  SDL_Event event;
  for (;;) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }
    start = SDL_GetPerformanceCounter();

    handleEvent(event);
    update();
    logicPlayer();
    logicWalls();
    // score?
    render();

    end = SDL_GetPerformanceCounter();
    // calculates to 60 fps
    elapsedMS = 0.0;
    elapsedMS =
      ((end - start) / (float)SDL_GetPerformanceFrequency()) * 1000.0f;

    // Cap to 60 FPS
    if (elapsedMS <= 16.0)
      SDL_Delay(floor((1000.0 / 60) - elapsedMS));
    else
      SDL_Delay(1000 / 60);
  }
  clean();
  SDL_Quit();
}

void
init()
{
  gameInit();
  playerInit(&leftPlayer);
  playerInit(&rightPlayer);
  ballInit();
}

void
clean()
{
  playerClean(&leftPlayer);
  playerClean(&rightPlayer);
  ballClean();
  gameClean();
}

void
logicPlayer()
{
  // right Player
  if (ball.drect.x + ball.drect.w >= rightPlayer.drect.x) {
    if ((ball.drect.y >= (rightPlayer.drect.y - ball.drect.h)) &&
        (ball.drect.y <= (rightPlayer.drect.y + rightPlayer.drect.h))) {
      SDL_Log("test");
      ball.x_direction *= -1;
      return;
    }
  }

  // left Player
  if (leftPlayer.drect.x + leftPlayer.drect.w >= ball.drect.x) {
    if ((ball.drect.y >= (leftPlayer.drect.y - ball.drect.h)) &&
        (ball.drect.y <= (leftPlayer.drect.y + leftPlayer.drect.h))) {
      SDL_Log("test 2");
      ball.x_direction *= -1;
      return;
    }
  }
}

void
logicWalls()
{
  // right boundary
  if (ball.drect.x + ball.drect.w > game.wind_w) {
    ball.drect.x = game.wind_w - ball.drect.w;
    ball.x_direction *= -1;
    return;
  }
  // left boundary
  if (ball.drect.x < 0) {
    ball.drect.x = 0;
    ball.x_direction *= -1;
    return;
  }

  // bottom boundary
  if (ball.drect.y + ball.drect.h > game.wind_h) {
    ball.drect.y = game.wind_h - ball.drect.h;
    ball.y_direction *= -1;
    return;
  }

  // upper bounda->y
  if (ball.drect.y < 0) {
    ball.drect.y = 0;
    ball.y_direction *= -1;
    return;
  }
}

void
update()
{
  // playerUpdate();
  ballUpdate();
}

void
render()
{

  gameRender();
  playerRender(&leftPlayer);
  playerRender(&rightPlayer);
  ballRender();

  // for multiple rendering
  SDL_RenderPresent(game.renderer);
}

// Todo: incorporate
static void
cleanup(int exitcode)
{
  TTF_Quit();
  SDL_Quit();
  exit(exitcode);
}

void
renderFont()
{
  if (TTF_Init() != 0) {
    SDL_Log("Couldn't initialize TTF: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }

  const char filepath[] = "Test test";

  // pointsize
  int ptsize = 18;
  TTF_Font* font;
  SDL_Surface* test;

  font = TTF_OpenFont(argv[0], ptsize);

  if (font == NULL) {
    SDL_Log("Couldn't load %d pt font from %s: %s\n",
            ptsize,
            filepath,
            SDL_GetError());
    cleanup(2);
  }
}

void
handleEvent(SDL_Event event)
{
  gameInputAction(event);

  switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_W:
          leftPlayer.drect.y -= PLAYER_SPEED;
          break;
        case SDL_SCANCODE_S:
          leftPlayer.drect.y += PLAYER_SPEED;
          break;
        case SDL_SCANCODE_UP:
          rightPlayer.drect.y -= PLAYER_SPEED;
          break;
        case SDL_SCANCODE_DOWN:
          rightPlayer.drect.y += PLAYER_SPEED;
          break;
        default:
          break;
      }
  }
}
