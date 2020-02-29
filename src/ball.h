#ifndef BALL_H
#define BALL_H

#include <gb/gb.h>

struct ball_t
{
  INT16 posx;   // fixed16
  INT16 posy;   // fixed16
  
  INT16 speed_x; // fixed16
  INT16 speed_y; // fixed16
  
  UINT8 spr;
  UINT8 last_frame_collision; //
  UINT8 last_hit;  // paddle index 0 (left), 1 (right)
  UINT8 speed_id;  // kind of orientation. index in speeds & speeds_inc
  
  UINT8 bounce; // number of bounce. // speed+bounce should be enough to compute speed. (no need for speedx & speedy)
  UINT8 pad0;
  UINT8 pad1;
  UINT8 activated;
};

#define BALL_MAIN      0 // main ball of the game 
#define BALL_BONUS     1 // extra ball on bonus
#define BALL_BLACK     2 // black ball of the boss
#define BALL_MAX_COUNT 3

#define BALL_BLACK_SPEED 24U

struct ball_t *ball_get(UINT8 id);

void ball_init_all();
void ball_reset(UINT8 id);
void ball_reset_all();
void ball_activate(UINT8 id);
void ball_deactivate(UINT8 id);
UINT8 ball_is_activated(UINT8 id);
UINT8 ball_is_activated_any();
void ball_set_position(UINT8 id, UINT8 x, UINT8 y);
UINT8 ball_get_position_x(UINT8 id);
void ball_init_speed(UINT8 id, UINT8 mul, UINT8 side);
void ball_set_last_hit(UINT8 id, UINT8 side);

void set_ball_speed(struct ball_t *ball, UINT8 speed);

void bounce_ball_x(struct ball_t *ball);
void bounce_ball_y(struct ball_t *ball);

void bounce_ball_corner_top_left(struct ball_t *ball);
void bounce_ball_corner_top_right(struct ball_t *ball);
void bounce_ball_corner_bottom_left(struct ball_t *ball);
void bounce_ball_corner_bottom_right(struct ball_t *ball);

#endif