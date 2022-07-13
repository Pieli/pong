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
logicWallsPlayer(Player* player);
void
logicPlayer();
void
handleEvent(SDL_Event event);
void
update();

Player leftPlayer;
Player rightPlayer;

const int PLAYER_SPEED = 15;

unsigned int hold_w = 0;
unsigned int hold_s = 0;
unsigned int hold_up = 0;
unsigned int hold_down = 0;

int
main(int argc, char* args[])
{
  Uint64 start, end;
  float elapsedMS;

  init();

  // TODO game dynamic -> 0 y ?
  // TODO game finish after 10
  // TODO test on ubuntu
  // TODO test resize

  // inital positioning
  leftPlayer.drect.x = 30;
  leftPlayer.drect.y = 500;
  rightPlayer.drect.x = 1520;
  rightPlayer.drect.y = 500;

  SDL_Event event;
  for (;;) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        break;
    }
    start = SDL_GetPerformanceCounter();

    handleEvent(event);

    if (paused) {
      render();
      continue;
    }

    update();
    logicPlayer();
    logicWalls();
    logicWallsPlayer(&leftPlayer);
    logicWallsPlayer(&rightPlayer);
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
calculateAngle(Player player)
{
  if (ball.speed < 15)
    ball.speed = 15;

  ball.x_direction *= -1;

  double factor = 1.5;
  double middle = player.drect.y + (player.drect.h / 2.0);

  if (ball.drect.y <= middle) {
    ball.y_direction =
      ((ball.drect.y + ball.drect.h - player.drect.y - (player.drect.h / 2.0)) /
       (player.drect.h / 2.0)) *
      factor;
  } else {
    ball.y_direction =
      ((ball.drect.y - middle) / (player.drect.h / 2.0)) * factor;
  }
}

void
logicPlayer(Player player)
{
  if (ball.drect.x + ball.drect.w >= rightPlayer.drect.x) {
    if (((ball.drect.y + ball.drect.h) >= rightPlayer.drect.y) &&
        (ball.drect.y <= (rightPlayer.drect.y + rightPlayer.drect.h))) {
      calculateAngle(rightPlayer);
      return;
    }
  }

  // left Player
  if (leftPlayer.drect.x + leftPlayer.drect.w >= ball.drect.x) {
    if (((ball.drect.y + ball.drect.h) >= leftPlayer.drect.y) &&
        (ball.drect.y <= (leftPlayer.drect.y + leftPlayer.drect.h))) {
      calculateAngle(leftPlayer);
      return;
    }
  }
}

void
logicWallsPlayer(Player* player)
{
  // right boundary
  if (player->drect.x + player->drect.w > game.wind_w) {
    player->drect.x = game.wind_w - player->drect.w;
    return;
  }
  // left boundary
  if (player->drect.x < 0) {
    player->drect.x = 0;
    return;
  }

  // bottom boundary
  if (player->drect.y + player->drect.h > game.wind_h) {
    player->drect.y = game.wind_h - player->drect.h;
    return;
  }

  // upper bounda->y
  if (player->drect.y < 0) {
    player->drect.y = 0;
    return;
  }
}

void
logicWalls()
{

  // right boundary
  if (ball.drect.x + ball.drect.w > game.wind_w) {
    ball.drect.x = game.wind_w - ball.drect.w;
    ball.speed = 5;
    gameAddScorePlayer1();
    ballReset(1);
    return;
  }
  // left boundary
  if (ball.drect.x < 0) {
    ball.drect.x = 0;
    ball.speed = 5;
    gameAddScorePlayer2();
    ballReset(-1);
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

  if (paused) {
    gamePauseRender();
  }

  // for multiple rendering
  SDL_RenderPresent(game.renderer);
}

void
handleEvent(SDL_Event event)
{
  gameInputAction(event);

  time_t timer;
  char buffer[26];
  struct tm* tm_info;

  timer = time(NULL);
  tm_info = localtime(&timer);

  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

  switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_W:
          hold_w = 1;
          break;
        case SDL_SCANCODE_S:
          hold_s = 1;
          break;
        case SDL_SCANCODE_UP:
          hold_up = 1;
          break;
        case SDL_SCANCODE_DOWN:
          hold_down = 1;
          break;
        default:
          break;
      }
    case SDL_KEYUP:
      if (event.key.state == SDL_PRESSED)
        break;

      switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_W:
          hold_w = event.key.state;
          break;
        case SDL_SCANCODE_S:
          hold_s = event.key.state;
          break;
        case SDL_SCANCODE_UP:
          hold_up = event.key.state;
          break;
        case SDL_SCANCODE_DOWN:
          hold_down = event.key.state;
          break;
        default:
          break;
      }

    default:
      break;
  }

  leftPlayer.drect.y -= hold_w * PLAYER_SPEED;
  leftPlayer.drect.y += hold_s * PLAYER_SPEED;
  rightPlayer.drect.y -= hold_up * PLAYER_SPEED;
  rightPlayer.drect.y += hold_down * PLAYER_SPEED;
}
