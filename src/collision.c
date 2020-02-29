#include <gb/gb.h>
#include <gb/rand.h>
#include "collision.h"
#include "graphics.h"
#include "sound.h"
#include "background.h"

void intersect_circle_rectangle(struct intersect_t *result, UINT8 cx, UINT8 cy, UINT8 cr, UINT8 rx, UINT8 ry, UINT8 rhw, UINT8 rhh)
{
  INT16 abs_dx;
  INT16 abs_dy;

  INT16 sq_x;
  INT16 sq_y;

  result->axis = 0;
  //result->orientation = 0;

  abs_dx = (INT16)cx-(INT16)rx;
  if (abs_dx<0) abs_dx=-abs_dx;

  abs_dy = (INT16)cy-(INT16)ry;
  if (abs_dy<0) abs_dy=-abs_dy;

  if (abs_dx > (INT16)(rhw+cr)) return;
  if (abs_dy > (INT16)(rhh+cr)) return;

  sq_x = (abs_dx-(INT16)rhw)*(abs_dx-(INT16)rhw);
  sq_y = (abs_dy-(INT16)rhh)*(abs_dy-(INT16)rhh);

  if ((sq_x+sq_y)<=((INT16)cr*(INT16)cr))
  {
    result->axis = 3;
  }
  else if (abs_dx <= (INT16)(rhw+cr) && abs_dy <= (INT16)rhh)
  {
    result->axis = 1;
  }
  else if (abs_dy <= (INT16)(rhh+cr) && abs_dx <= (INT16)rhw)
  {
    result->axis = 2;
  }
  else
  {
    result->axis = 0xFF;
  }
}

const UINT8 all_bonuses[] = { BONUS_BIG, BONUS_SMALL, BONUS_SPEED, BONUS_BLACK, BONUS_INVERT, BONUS_BALL };
const UINT8 ai_only_bonuses[] = { BONUS_BIG, BONUS_SMALL, BONUS_SPEED, BONUS_BLACK, BONUS_BALL };

void execute_bonus(struct bonus_t *bonus, struct paddle_t *paddle, struct ball_t *ball, struct ball_t *extra_ball)
{
  UINT8 bonus_count;
  if (paddle->ai)
  {
    bonus_count = sizeof(ai_only_bonuses);
    if (ball_is_activated(BALL_MAIN) && (ball_is_activated(BALL_BONUS) || ball_is_activated(BALL_BLACK))) // Should bg a ball_get_active_count 
    {
      // Remove the extra ball from the eligible bonus as extra ball is last in bonuses sequence.
      bonus_count--;
    }
    bonus->type = ai_only_bonuses[_rand()%bonus_count];
  }
  else
  {
    bonus_count = sizeof(all_bonuses);
    if (ball_is_activated(BALL_MAIN) && (ball_is_activated(BALL_BONUS) || ball_is_activated(BALL_BLACK)))
    {
      // Remove the extra ball from the eligible bonus as extra ball is last in bonuses sequence.
      bonus_count--;
    }
    bonus->type = all_bonuses[_rand()%bonus_count];
  }

  if (bonus->type == BONUS_BIG)
  {
      set_paddle_big(paddle);
      deactivate_bonus(bonus, 0);
  }
  else if (bonus->type == BONUS_SMALL)
  {
      set_paddle_small(paddle);
      deactivate_bonus(bonus, 0);
  }
  else if (bonus->type == BONUS_BALL)
  {
      extra_ball->posx = ball->posx;
      extra_ball->posy = ball->posy;
      extra_ball->speed_id = ball->speed_id;
      extra_ball->speed_x = -ball->speed_x;
      extra_ball->speed_y = -ball->speed_y;
      extra_ball->last_hit = ball->last_hit;
      ball_activate(BALL_BONUS);
      set_ball_speed(ball, 0);
      set_ball_speed(extra_ball, 0);
      deactivate_bonus(bonus, 0);
  }
  else if (bonus->type == BONUS_SPEED)
  {
    if (ball->bounce < 16)
    {
      ball->bounce = 32;
    }
    else
    {
      ball->bounce += 32;
    }
    set_ball_speed(ball, ball->bounce); // should do acceleration code in ball?

    if (extra_ball->bounce < 16)
    {
      extra_ball->bounce = 32;
    }
    else
    {
      extra_ball->bounce += 32;
    }
    set_ball_speed(extra_ball, extra_ball->bounce);

    set_music_speed(MUSIC_SPEED_FAST);
    deactivate_bonus(bonus, 0);
  }
  else if (bonus->type == BONUS_BLACK)
  {
    bkg_darkmode();
    deactivate_bonus(bonus, 0);
  }
  else if (bonus->type == BONUS_INVERT)
  {
    invert_paddle_direction(paddle); // this will reverse sound as well
    deactivate_bonus(bonus, 0);
  }
}

void intersect_ball_bonus(struct ball_t *ball, struct ball_t *extra_ball, struct bonus_t *bonus, struct paddle_t *paddle)
{
  UINT8 ballx;
  UINT8 bally;
  UINT8 bonusx;
  UINT8 bonusy;
  struct intersect_t intersect;

  if (bonus->active)
  {
    ballx  = (ball->posx>>8)+(TILE_WIDTH/2U);
    bally  = (ball->posy>>8)+(TILE_HEIGHT/2U);
    bonusx = (bonus->posx>>8);    
    bonusy = (bonus->posy>>8);

    intersect_circle_rectangle(
      &intersect,
      ballx,
      bally,
      TILE_WIDTH/2U,
      bonusx,
      bonusy,
      TILE_WIDTH,
      TILE_HEIGHT
    );

    if (intersect.axis > 0)
    {
      execute_bonus(bonus, paddle, ball, extra_ball);
    }
  }
}

void intersect_ball_paddle(struct ball_t *ball, struct paddle_t *paddle, UINT8 side)
{
  UINT8 bx;
  UINT8 by;
  UINT8 ps;
  UINT8 px;
  UINT8 py;
  struct intersect_t intersect;

  INT16 abs_dy; // to compute orientation

    bx = (ball->posx>>8)+(TILE_WIDTH/2U);
    by = (ball->posy>>8)+(TILE_HEIGHT/2U);

  if (ball->last_frame_collision > 0)
  {
    // Enable collision again once the ball is far from the paddle.
    if (ball->last_hit == 0) // left
    {
      if (bx > (SPR_XOFFSET+8U+TILE_WIDTH)+8)
      {
        ball->last_frame_collision = 0;
      }
    }
    else // right
    {
      if (bx < (SPR_XOFFSET+SCREENWIDTH-16U)-8)
      {
        ball->last_frame_collision = 0;
      }
    }
  }
  else
  {
    ps = get_paddle_pixel_height(paddle);
    px = paddle->posx+(TILE_WIDTH/2U);    
    py = paddle->posy+(ps/2U);

    intersect_circle_rectangle(
        &intersect,
        bx,
        by,
        TILE_WIDTH/2U,
        px,
        py,
        TILE_WIDTH/2U,
        ps/2U
    );

    if (intersect.axis == 1U)
    {
      ball->bounce++;
      // add comment for reorientation
      abs_dy = (INT16)by-(INT16)py;
      if (abs_dy > 0 && ball->speed_y < 0)
      {
        if (abs_dy < (ps/4U))
        {
          ball->speed_id = 2;
        }
        else
        {
          ball->speed_id = 1;
        }

        bounce_ball_y(ball);
      }

      if (abs_dy < 0 && ball->speed_y > 0)
      {
        if (-abs_dy < (ps/4U))
        {
          ball->speed_id = 2;
        }
        else
        {
          ball->speed_id = 1;
        }        

        bounce_ball_y(ball);
      }

      bounce_ball_x(ball);
      ball->last_frame_collision = 1;
      ball->last_hit = side;
    }
    if (intersect.axis == 2U)
    {
      if (by > py && ball->speed_y < 0)
      {
        bounce_ball_y(ball);
      }
      if (by < py && ball->speed_y > 0)
      {
        bounce_ball_y(ball);
      }
      ball->last_frame_collision = 1;
      ball->last_hit = side;
    }
    if (intersect.axis == 3U)
    {
      ball->bounce++;
      {
        if (bx > px)
        {
          if (by < py)
          {
            bounce_ball_corner_top_right(ball);
          } 
          else
          {
            bounce_ball_corner_bottom_right(ball);
          }
        }
        else
        {
          if (by < py)
          {
            bounce_ball_corner_top_left(ball);
          } 
          else
          {
            bounce_ball_corner_bottom_left(ball);
          }
        }
      }

      ball->last_frame_collision = 1;
      ball->last_hit = side;
    }
  }

}

void collision_ball_y(struct ball_t *ball, UINT8 min, UINT8 max)
{
  INT16 min16;
  INT16 max16;
  INT16 dmin;
  INT16 dmax;
  UINT16 invspeed;

  // The only way I made it work:
  // - hand-made 2-complement (-speedy not working)
  // - precompute here (not inside the if-s)
  invspeed = 1U+(~((UINT16)ball->speed_y));

  min16 = ((INT16)min)<<8;
  dmin = ball->posy-min16;
  if (dmin<0)
  {
    ball->posy = min16;
    ball->speed_y = invspeed;
  }

  max16 = ((INT16)max)<<8;
  dmax = max16-ball->posy;
  if (dmax<0)
  {
    ball->posy = max16;
    ball->speed_y = invspeed;
  }
}
