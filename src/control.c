#include <gb/gb.h>
#include "control.h"
#include "ball.h"
#include "paddle.h"
#include "graphics.h"
#include "game.h"
#include "link.h"

struct game_buffer_t
{
    UINT8 paddle_pos_y;
    UINT8 ball_pos_x;
    UINT8 ball_pos_y;
};

struct game_buffer_t game_buffer_in;
struct game_buffer_t game_buffer_out;

INT8 ai_simple(struct ball_t *b, struct paddle_t *p)
{
  UINT8 by;
  UINT8 py;

  by = (b->posy>>8)+(TILE_HEIGHT/2U);
  py = p->posy+(get_paddle_pixel_height(p)/2U);

  if (by < py)
  {
    return -1;
  }

  if (by > py)
  {
    return 1;
  }

  return 0;
}

INT8 ai_closest_ball(struct paddle_t *p)
{
  UINT8 bx;
  UINT8 by;
  UINT8 py;

  struct ball_t *main, *bonus, *black, *closest;

  closest = 0;
  main = ball_get(BALL_MAIN);
  bonus = ball_get(BALL_BONUS);
  black = ball_get(BALL_BLACK);

  if (p->side == 0)
  {
    if (main->activated)
    {
      closest = main;
      bx = ball_get_position_x(BALL_MAIN);

      if (bonus->activated && (bx > ball_get_position_x(BALL_BONUS)))
      {
        closest = bonus;
        bx = ball_get_position_x(BALL_BONUS);
      }
      if (black->activated && (bx > ball_get_position_x(BALL_BLACK)))
      {
        closest = black;
      }
    }
    else if (bonus->activated)
    {
      closest = bonus;
      bx = ball_get_position_x(BALL_BONUS);
      
      if (black->activated && (bx > ball_get_position_x(BALL_BLACK)))
      {
        closest = black;
      }
    }
    else if (black->activated)
    {
      closest = black;
    }
  }
  else
  {
    if (main->activated)
    {
      closest = main;
      bx = ball_get_position_x(BALL_MAIN);

      if (bonus->activated && (bx < ball_get_position_x(BALL_BONUS)))
      {
        closest = bonus;
        bx = ball_get_position_x(BALL_BONUS);
      }
      if (black->activated && (bx < ball_get_position_x(BALL_BLACK)))
      {
        closest = black;
      }
    }
    else if (bonus->activated)
    {
      closest = bonus;
      bx = ball_get_position_x(BALL_BONUS);
      
      if (black->activated && (bx < ball_get_position_x(BALL_BLACK)))
      {
        closest = black;
      }
    }
    else if (black->activated)
    {
      closest = black;
    }
  }

  if (!closest)
  {
    return 0;
  }

  by = (closest->posy>>8)+(TILE_HEIGHT/2U);
  py = p->posy+(get_paddle_pixel_height(p)/2U);

  if (by < py)
  {
    return -1;
  }

  if (by > py)
  {
    return 1;
  }

  return 0;
}

UINT8 control_wait_up = 0;
UINT8 control_paused = 0;

UINT8 control(struct paddle_t *p1, struct paddle_t *p2, UINT8 mode, UINT8 locked)
{
    UINT8 r;
    UINT8 j;
    struct ball_t *ball;

    ball = ball_get(BALL_MAIN);

    j = joypad();
    if (j & J_SELECT)
    {
      ball_reset(BALL_MAIN); // Debug!
    }

    if (control_wait_up)
    {
      if (!j) // Could be a non-press-start check only as it allow player to press up/down to anticipate...
      {
        control_wait_up = 0;
      }
      return control_paused ? CONTROL_PAUSE : CONTROL_RESUME;
    }

    if (control_paused)
    {
      if (j & J_START)
      {
        control_wait_up = 1;
        control_paused = 0;
      }

      return control_paused ? CONTROL_PAUSE : CONTROL_RESUME;
    }

    if (mode == GAME_MODE_1_PLAYER)
    {
      if (!locked)
      {
        if (j & J_UP)
          move_paddle(p1, 0, (INT8)-1);
        if (j & J_DOWN)
          move_paddle(p1, 0, (INT8)1);
      }

      if (j & J_START)
      {
        control_wait_up = 1;
        control_paused = 1;
        ball_deactivate(BALL_MAIN);
        return CONTROL_PAUSE;        
      }
      
      if (ball_is_activated_any())
      {
        r = ai_closest_ball(p2);
        move_paddle(p2, 0, r);
      }
    }
    else if (mode == GAME_MODE_2_PLAYERS)
    {
      if (j & J_UP)
        move_paddle(p1, 0, (INT8)-1);
      if (j & J_DOWN)
        move_paddle(p1, 0, (INT8)1);

      if (j & J_A)
        move_paddle(p2, 0, (INT8)-1);
      if (j & J_B)
        move_paddle(p2, 0, (INT8)1);
    }
    else if (mode == GAME_MODE_AI)
    {
      if (j)
      {
        return CONTROL_STOP;
      }

      r = ai_closest_ball(p1);
      move_paddle(p1, 0, r);

      r = ai_closest_ball(p2);
      move_paddle(p2, 0, r);
    }
    else if (mode == GAME_MODE_LINK)
    {
      if (j & J_UP)
        move_paddle(p1, 0, (INT8)-1);
      if (j & J_DOWN)
        move_paddle(p1, 0, (INT8)1);

      set_paddle_position(p2, lk_get_data_buffer_in()[0]);
      if (lk_is_connected() == LK_SLAVE)
      {
        ball_set_position(
          BALL_MAIN,
          TILE_WIDTH+SCREENWIDTH-lk_get_data_buffer_in()[1],
          lk_get_data_buffer_in()[2]
        );
      } 

      game_buffer_out.paddle_pos_y = p1->posy;
      game_buffer_out.ball_pos_x = (ball->posx)>>8;
      game_buffer_out.ball_pos_y = (ball->posy)>>8;
      lk_set_data_buffer_out((UINT8*)game_buffer_out, 3);

      lk_transmit();
    }

    return CONTROL_CONTINUE;
}
