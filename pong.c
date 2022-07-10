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
#include "font.h"
#include "game.h"
#include "line.h"
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

const int PLAYER_SPEED = 30;

// TODO: move Scenesome where else
Scene scene_player_1;
Scene scene_player_2;

unsigned int hold_w = 0;
unsigned int hold_s = 0;
unsigned int hold_up = 0;
unsigned int hold_down = 0;

unsigned int score_p_1 = 0;
unsigned int score_p_2 = 0;

const double POS_SCORE_P_1_X = (1.0 / 3);
const double POS_SCORE_P_1_Y = (1.0 / 3);

const double POS_SCORE_P_2_X = (2.0 / 3);
const double POS_SCORE_P_2_Y = (1.0 / 3);

int
main(int argc, char* args[])
{
  Uint64 start, end;
  float elapsedMS;

  init();
  fontUpdate(&scene_player_1, "0", POS_SCORE_P_1_X, POS_SCORE_P_1_Y);
  fontUpdate(&scene_player_2, "0", POS_SCORE_P_2_X, POS_SCORE_P_2_Y);

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
    logicWallsPlayer(&leftPlayer);
    logicWallsPlayer(&rightPlayer);
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
  fontInit();
  initLine();
  playerInit(&leftPlayer);
  playerInit(&rightPlayer);
  ballInit();
}

void
clean()
{
  cleanLine();
  fontClean(&scene_player_1);
  fontClean(&scene_player_2);
  playerClean(&leftPlayer);
  playerClean(&rightPlayer);
  ballClean();
  gameClean();
}

void
logicPlayer2()
{
  // right Player
  if (ball.drect.x + ball.drect.w >= rightPlayer.drect.x) {
    if (((ball.drect.y + ball.drect.h) >= rightPlayer.drect.y) &&
        (ball.drect.y <= (rightPlayer.drect.y + rightPlayer.drect.h))) {
      ball.x_direction *= -1;
      return;
    }
  }

  // left Player
  if (leftPlayer.drect.x + leftPlayer.drect.w >= ball.drect.x) {
    if (((ball.drect.y + ball.drect.h) >= leftPlayer.drect.y) &&
        (ball.drect.y <= (leftPlayer.drect.y + leftPlayer.drect.h))) {
      ball.x_direction *= -1;
      return;
    }
  }
}

void
logicPlayer()
{
  double center = ball.drect.y + (ball.drect.h / 2.0);
  double block = rightPlayer.drect.h / 9.0;
  double area_top, area_bottom;

  // right Player
  if (ball.drect.x + ball.drect.w >= rightPlayer.drect.x) {
    if (((ball.drect.y + ball.drect.h) >= rightPlayer.drect.y) &&
        (ball.drect.y <= (rightPlayer.drect.y + rightPlayer.drect.h))) {

      ball.x_direction = -1;
      area_top = rightPlayer.drect.y + (block / 2);
      area_bottom = rightPlayer.drect.y + rightPlayer.drect.h - (block / 2);
      double factor = 1.0;

      for (int i = 0; i < 5; i++) {
        factor -= 0.2;
        SDL_Log("here %f", factor);
        if (center <= area_top) {
          ball.y_direction = -factor;
          return;
        }
        if (center > area_bottom) {
          ball.y_direction = factor;
          return;
        }

        area_top += block;
        area_bottom -= block;
      }

      return;
    }
  }

  // left Player
  if (leftPlayer.drect.x + leftPlayer.drect.w >= ball.drect.x) {
    if (((ball.drect.y + ball.drect.h) >= leftPlayer.drect.y) &&
        (ball.drect.y <= (leftPlayer.drect.y + leftPlayer.drect.h))) {
      ball.x_direction *= -1;
      ball.y_direction = 0;
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
  char num[12];

  // right boundary
  if (ball.drect.x + ball.drect.w > game.wind_w) {
    ball.drect.x = game.wind_w - ball.drect.w;
    ball.x_direction *= -1;
    sprintf(num, "%d", ++score_p_1);
    fontUpdate(&scene_player_1, num, POS_SCORE_P_1_X, POS_SCORE_P_1_Y);
    return;
  }
  // left boundary
  if (ball.drect.x < 0) {
    ball.drect.x = 0;
    ball.x_direction *= -1;
    sprintf(num, "%d", ++score_p_2);
    fontUpdate(&scene_player_2, num, POS_SCORE_P_2_X, POS_SCORE_P_2_Y);
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
  renderLine();
  playerRender(&leftPlayer);
  playerRender(&rightPlayer);
  fontRender(&scene_player_1);
  fontRender(&scene_player_2);
  ballRender();

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
