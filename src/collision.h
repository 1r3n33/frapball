#ifndef COLLISION_H
#define COLLISION_H

#include <gb/gb.h>
#include "ball.h"
#include "bonus.h"
#include "paddle.h"

struct intersect_t
{
  UINT8 axis; // 0 (no intersect), 1 (x intersect), 2 (y interesect), 3 (both)
  UINT8 pad0; //
  UINT8 pad1; //
  UINT8 orientation;   // NOT USED
};

void intersect_circle_rectangle(struct intersect_t *result, UINT8 cx, UINT8 cy, UINT8 cr, UINT8 rx, UINT8 ry, UINT8 rhw, UINT8 rhh);

void intersect_ball_bonus(struct ball_t *ball, struct ball_t *extra_ball, struct bonus_t *bonus, struct paddle_t *paddle);

void intersect_ball_paddle(struct ball_t *ball, struct paddle_t *paddle, UINT8 side);

void collision_ball_y(struct ball_t *ball, UINT8 min, UINT8 max);

#endif