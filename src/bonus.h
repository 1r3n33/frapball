#ifndef BONUS_H
#define BONUS_H

#include <gb/gb.h>

#define BONUS_NONE     0U // no bonus
#define BONUS_BIG      1U // paddle gets bigger
#define BONUS_SMALL    2U // paddle gets smaller
#define BONUS_BALL     3U // extra ball
#define BONUS_SPEED    4U // ball goes faster
#define BONUS_BLACK    5U // light off
#define BONUS_INVERT   6U // invert the command
#define BONUS_BRICK    7U // brick
#define BONUS_DOOR     8U // doors make the goal smaller
#define BONUS_POINTS   9U // doubles the score
#define BONUS_SWALLOW 11U // swallow the ball then spit the ball elsewhere
#define BONUS_BAR     11U // extra

struct bonus_t
{
  UINT16  posx;
  UINT16  posy;

  INT16  slidex; // const. Can be removed.
  INT16  slidey; // const. Can be removed.

  UINT16  refx;
  UINT16  refy;
  
  UINT8  spr;
  UINT8  type; // Type determined at collision time. Can be removed.
  UINT16 timer;
  
  UINT8  active;
  UINT8  pad0;
  UINT8  pad1;
  UINT8  pad2;
};

void init_bonus(struct bonus_t *bonus);
void reset_bonus(struct bonus_t *bonus);

void update_bonus(struct bonus_t *bonus);

void deactivate_bonus(struct bonus_t *bonus, UINT16 extra_time);

UINT8 is_bonus_collidable(struct bonus_t *bonus);

#endif