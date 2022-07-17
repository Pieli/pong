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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ball.h"
#include "game.h"
#include "player.h"

void
init(void);
void
clean(void);
void
restart(void);
void
render(void);
void
logicWalls(void);
void
logicWallsPlayer(Player* player);
void
logicPlayer(void);
void
handleEvent(SDL_Event event);
void
update(void);
void
calculateAngle(Player player);

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

  // TODO game finish after 10
  // TODO test on ubuntu
  // TODO test resize

  // inital positioning
  leftPlayer.drect.x = 30;
  leftPlayer.drect.y = game.wind_h / 2.0;
  rightPlayer.drect.x = game.wind_w - 30 - 50;
  rightPlayer.drect.y = game.wind_h / 2.0;

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

    if (gameCheckWinner() != 0) {
      winner = 1;
      gameWinnerScreenRender(gameCheckWinner());
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
init(void)
{
  gameInit();
  playerInit(&leftPlayer);
  playerInit(&rightPlayer);
  ballInit();
}

void
clean(void)
{
  playerClean(&leftPlayer);
  playerClean(&rightPlayer);
  ballClean();
  gameClean();
}

void
calculateAngle(Player player)
{
  if (ball.speed < 20)
    ball.speed = 20;

  ball.x_direction *= -1;

  double factor = 0.9;
  double middle = player.drect.y + (player.drect.h / 2.0);

  if (ball.drect.y <= middle) {
    ball.y_direction = ((ball.drect.y + ball.drect.h - player.drect.y -
                         (player.drect.h / 2.0) + 5) /
                        (player.drect.h / 2.0)) *
                       factor;
  } else {
    ball.y_direction =
      ((ball.drect.y - middle) / (player.drect.h / 2.0)) * factor;
  }

  double ball_dir_old = ball.x_direction;
  int dir = 1;

  if (ball_dir_old < 0)
    dir = -1;

  ball.x_direction = dir * sqrt(1 - (ball.y_direction * ball.y_direction));
}

void
logicPlayer(void)
{
  double ball_x_2 = ball.drect.x + ball.drect.w;
  double ball_y_2 = ball.drect.y + ball.drect.h;

  if (ball_x_2 >= rightPlayer.drect.x) {
    if ((ball_y_2 >= rightPlayer.drect.y) &&
        (ball.drect.y <= (rightPlayer.drect.y + rightPlayer.drect.h))) {
      if ((ball.drect.x + ball.drect.w) >
          (rightPlayer.drect.x + (rightPlayer.drect.w / 2.0))) {
        if (ball.drect.y < (rightPlayer.drect.y + rightPlayer.drect.h / 2.0))
          ball.drect.y = rightPlayer.drect.y - ball.drect.h;
        else
          ball.drect.y = rightPlayer.drect.y + rightPlayer.drect.h;
      }
      calculateAngle(rightPlayer);
      return;
    }
  }

  // left Player
  if (leftPlayer.drect.x + leftPlayer.drect.w >= ball.drect.x) {
    if ((ball_y_2 >= leftPlayer.drect.y) &&
        (ball.drect.y <= (leftPlayer.drect.y + leftPlayer.drect.h))) {
      if (ball.drect.x < (leftPlayer.drect.x + (leftPlayer.drect.w / 2.0))) {
        if (ball.drect.y < (leftPlayer.drect.y + (leftPlayer.drect.h / 2.0))) {
          ball.drect.y = leftPlayer.drect.y - ball.drect.h;
        } else {
          ball.drect.y = leftPlayer.drect.y + leftPlayer.drect.h;
        }
      }
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
logicWalls(void)
{

  // right boundary
  if (ball.drect.x + ball.drect.w > game.wind_w) {
    ball.drect.x = game.wind_w - ball.drect.w;
    ball.speed = 10;
    gameAddScorePlayer1();
    ballReset(1);
    return;
  }
  // left boundary
  if (ball.drect.x < 0) {
    ball.drect.x = 0;
    ball.speed = 10;
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
update(void)
{
  // playerUpdate();
  ballUpdate();
}

void
render(void)
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
restart(void)
{
  hold_s = 0;
  hold_w = 0;
  hold_down = 0;
  hold_up = 0;

  gameResetScores();

  playerInit(&leftPlayer);
  playerInit(&rightPlayer);

  leftPlayer.drect.x = 30;
  leftPlayer.drect.y = 500;
  rightPlayer.drect.x = 1520;
  rightPlayer.drect.y = 500;

  ballInit();
}

void
handleEvent(SDL_Event event)
{
  gameInputAction(event);

  if (event.key.keysym.scancode == SDL_SCANCODE_R && winner) {
    winner = 0;
    restart();
  }

  if (paused || winner)
    return;

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
