#include <gb/gb.h>
#include <gb/rand.h>
#include "graphics.h"
#include "ball.h"

struct ball_t balls[BALL_MAX_COUNT];

struct ball_t *ball_get(UINT8 id)
{
  return &balls[id];
}

struct speed_t
{
  INT16 x;
  INT16 y;
};

// must be static initialized
void init_speeds(struct speed_t *speeds, struct speed_t *speeds_inc)
{
  speeds[0].x     = (INT16)98;
  speeds[0].y     = (INT16)237;
  speeds_inc[0].x = (INT16)3;
  speeds_inc[0].y = (INT16)7;

  speeds[1].x     = (INT16)181;
  speeds[1].y     = (INT16)181;
  speeds_inc[1].x = (INT16)6;
  speeds_inc[1].y = (INT16)6;

  speeds[2].x     = (INT16)237;
  speeds[2].y     = (INT16)98;
  speeds_inc[2].x = (INT16)7;
  speeds_inc[2].y = (INT16)3;
}

struct speed_t speeds[3];
struct speed_t speeds_inc[3];

void move_ball(struct ball_t *ball, INT8 dx, INT8 dy)
{
  ball->posx += (((INT16)dx)<<8);
  ball->posy += (((INT16)dy)<<8);

  if (ball->activated)
  {
    move_sprite(ball->spr, ball->posx>>8, ball->posy>>8);
  }
  else
  {
    move_sprite(ball->spr, 0, 0);
  }
}

void set_ball_position(struct ball_t *ball, UINT8 x, UINT8 y)
{
  ball->posx = (((INT16)x)<<8);
  ball->posy = (((INT16)y)<<8);

  if (ball->activated)
  {
    move_sprite(ball->spr, ball->posx>>8, ball->posy>>8);
  }
  else
  {
    move_sprite(ball->spr, 0, 0);
  }
}

void reset_ball(struct ball_t *ball)
{
  ball->activated = 0;

  ball->posx = ((SCREENWIDTH/2U)+SPR_XOFFSET)-(TILE_WIDTH/2U)<<8;
  ball->posy = ((SCREENHEIGHT/2U)+SPR_YOFFSET)-(TILE_HEIGHT/2U)<<8;

  ball->bounce = 0;
  ball->speed_id = _rand()%3;
  bounce_ball_x(ball);
  bounce_ball_y(ball);

  ball->last_frame_collision = 0;

  ball->last_hit = 0xff;

  move_ball(ball, 0, 0);
}

void init_ball(UINT8 id)
{
  struct ball_t *ball = &balls[id];

  ball->spr = alloc_sprites(1);
  set_sprite_tile(ball->spr, (id == BALL_BLACK) ? SPR_BLACK_BALL : SPR_BALL);
  set_sprite_prop(ball->spr, 0U);

  reset_ball(ball);
}

void set_ball_speed(struct ball_t *ball, UINT8 speed)
{
  ball->bounce = speed;
  if (ball->speed_x > 0)
  {
    ball->speed_x = (speeds[ball->speed_id].x + (speeds_inc[ball->speed_id].x * ball->bounce));
  }
  else
  {
    ball->speed_x = -(speeds[ball->speed_id].x + (speeds_inc[ball->speed_id].x * ball->bounce));
  }
  if (ball->speed_y > 0)
  {
    ball->speed_y = (speeds[ball->speed_id].y + (speeds_inc[ball->speed_id].y * ball->bounce));
  }
  else
  {
    ball->speed_y = -(speeds[ball->speed_id].y + (speeds_inc[ball->speed_id].y * ball->bounce));
  }
}

void bounce_ball_x(struct ball_t *ball)
{
  INT16 bounce = ball->bounce;
  if (ball->speed_x > 0)
  {
    ball->speed_x = -(speeds[ball->speed_id].x + (speeds_inc[ball->speed_id].x * bounce));
  }
  else
  {
    ball->speed_x = (speeds[ball->speed_id].x + (speeds_inc[ball->speed_id].x * bounce));
  }
}

void bounce_ball_y(struct ball_t *ball)
{
  INT16 bounce = ball->bounce;
  if (ball->speed_y > 0)
  {
    ball->speed_y = -(speeds[ball->speed_id].y + (speeds_inc[ball->speed_id].y * bounce));
  }
  else
  {
    ball->speed_y = (speeds[ball->speed_id].y + (speeds_inc[ball->speed_id].y * bounce));
  }
}

void bounce_ball_corner_top_left(struct ball_t *ball)
{
    ball->speed_id = 0;
    ball->speed_x = -(speeds[ball->speed_id].x + (speeds_inc[ball->speed_id].x * ball->bounce));
    ball->speed_y = -(speeds[ball->speed_id].y + (speeds_inc[ball->speed_id].y * ball->bounce));
}

void bounce_ball_corner_top_right(struct ball_t *ball)
{
    ball->speed_id = 0;
    ball->speed_x = (speeds[ball->speed_id].x + (speeds_inc[ball->speed_id].x * ball->bounce));
    ball->speed_y = -(speeds[ball->speed_id].y + (speeds_inc[ball->speed_id].y * ball->bounce));
}

void bounce_ball_corner_bottom_left(struct ball_t *ball)
{
    ball->speed_id = 0;
    ball->speed_x = -(speeds[ball->speed_id].x + (speeds_inc[ball->speed_id].x * ball->bounce));
    ball->speed_y = (speeds[ball->speed_id].y + (speeds_inc[ball->speed_id].y * ball->bounce));
}

void bounce_ball_corner_bottom_right(struct ball_t *ball)
{
    ball->speed_id = 0;
    ball->speed_x = (speeds[ball->speed_id].x + (speeds_inc[ball->speed_id].x * ball->bounce));
    ball->speed_y = (speeds[ball->speed_id].y + (speeds_inc[ball->speed_id].y * ball->bounce));
}

void activate_ball(struct ball_t *ball, UINT8 activate)
{
  ball->activated = activate;
}

void ball_init_all()
{
    UINT8 i;
    
    init_speeds(speeds, speeds_inc);

    for (i=0; i<BALL_MAX_COUNT; i++)
    {
      init_ball(i);
    }
}

void ball_reset(UINT8 id)
{
  reset_ball(&balls[id]);
}

void ball_reset_all()
{
  UINT8 i = 0;

    for (i=0; i<BALL_MAX_COUNT; i++)
    {
      ball_reset(i);
    }  
}

void ball_activate(UINT8 id)
{
  activate_ball(&balls[id], 1);
}

void ball_deactivate(UINT8 id)
{
  activate_ball(&balls[id], 0);
}

UINT8 ball_is_activated(UINT8 id)
{
  return balls[id].activated;
}

UINT8 ball_is_activated_any()
{
  return 
    balls[BALL_MAIN].activated  ||
    balls[BALL_BONUS].activated ||
    balls[BALL_BLACK].activated;
}

void ball_set_position(UINT8 id, UINT8 x, UINT8 y)
{
  set_ball_position(&balls[id], x, y);
}

UINT8 ball_get_position_x(UINT8 id)
{
  UINT16 posx16;
  UINT8 posx8;

  posx16 = (UINT16)(balls[id].posx);
  posx8  = (UINT8)(posx16>>8);
  
  return posx8;
}

void ball_init_speed(UINT8 id, UINT8 mul, UINT8 side)
{
  balls[id].speed_id = _rand()%3;
  balls[id].speed_x = side ? -1 : 1; // the goal here is to obtain whatever pos/neg value
  balls[id].speed_y = (_rand()%64)-32; // the goal here is to obtain whatever pos/neg value
  set_ball_speed(&balls[id], mul);
}

void ball_set_last_hit(UINT8 id, UINT8 side)
{
  balls[id].last_hit = side;
}
