#ifndef PADDLE_H
#define PADDLE_H

#include <gb/gb.h>

struct paddle_t
{
  UINT8  posx;
  UINT8  posy;
  UINT8  spr;
  UINT8  side;
  UINT8  state;
  UINT8  inverted;              // direction are inverted: 0 (normal), 1 (about to be inverted), 2 (inverted). Use timer_effect_inverted instead! Remove it.
  UINT8  level;                 // level
  UINT8  ai;                    // the paddle is piloted by ai
  UINT16 timer_effect_size;     // frame count for size effect
  UINT16 timer_effect_freeze;   // frame count for freeze effect
  UINT16 timer_effect_inverted; // frame count for inverted effect
  UINT16 timer_shoot; // for level1 & level 2 paddle shooting
};

void init_paddle(struct paddle_t *paddle, UINT8 side, UINT8 level, UINT8 is_ai);

void paddle_reset(struct paddle_t *paddle, UINT8 level);
//void set_paddle_sprites(struct paddle_t *paddle, UINT8 level);

void move_paddle(struct paddle_t *paddle, INT8 dx, INT8 dy);
void set_paddle_position(struct paddle_t *paddle, INT8 y);

void set_paddle_big(struct paddle_t *paddle);
void set_paddle_small(struct paddle_t *paddle);
void invert_paddle_direction(struct paddle_t *paddle);

UINT8 get_paddle_pixel_width(struct paddle_t *paddle);
UINT8 get_paddle_pixel_height(struct paddle_t *paddle);

void update_paddle(struct paddle_t *paddle);

void freeze_paddle(struct paddle_t *paddle, UINT8 t);

#endif