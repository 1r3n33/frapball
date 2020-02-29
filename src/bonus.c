#include <gb/gb.h>
#include <gb/rand.h>
#include "graphics.h"
#include "bonus.h"

#define BONUS_COUNT 6U // number of bonus implemented

#if 0
#define BONUS_PROP_ACTIVE_TIME      300U
#define BONUS_PROP_INACTIVE_TIME    600U
#else
#define BONUS_PROP_ACTIVE_TIME      300U
#define BONUS_PROP_INACTIVE_TIME    120U
#endif

#define BONUS_PROP_FADE_SPEED       4U
#define BONUS_PROP_SLIDE_X          20U
#define BONUS_PROP_SLIDE_Y          4U

#define BONUS_WINDOW_MINX (8U+((SCREENWIDTH/2U)-10U))
#define BONUS_WINDOW_MAXX (8U+((SCREENWIDTH/2U)+10U))
#define BONUS_WINDOW_MINY (16U+((SCREENHEIGHT/2U)-50U))
#define BONUS_WINDOW_MAXY (16U+((SCREENHEIGHT/2U)+50U))

void reset_bonus(struct bonus_t *bonus)
{
  bonus->active = 0;
  bonus->posx   = 0;
  bonus->posy   = 0;
  bonus->refx   = 0;
  bonus->refy   = 0;
  bonus->type   = BONUS_NONE;
  bonus->timer  = 2*BONUS_PROP_INACTIVE_TIME;
  bonus->slidex = 64;
  bonus->slidey = 32;

  move_sprite(bonus->spr+0, 0, 0);
  move_sprite(bonus->spr+1, 0, 0);
  move_sprite(bonus->spr+2, 0, 0);
  move_sprite(bonus->spr+3, 0, 0);
}

void init_bonus(struct bonus_t *bonus)
{
  bonus->spr = alloc_sprites(4);

  set_sprite_tile(bonus->spr+0, SPR_BONUS_NW);
  set_sprite_prop(bonus->spr+0, S_PALETTE);

  set_sprite_tile(bonus->spr+1, SPR_BONUS_NE);
  set_sprite_prop(bonus->spr+1, S_PALETTE);

  set_sprite_tile(bonus->spr+2, SPR_BONUS_SW);
  set_sprite_prop(bonus->spr+2, S_PALETTE);

  set_sprite_tile(bonus->spr+3, SPR_BONUS_SE);
  set_sprite_prop(bonus->spr+3, S_PALETTE);

  reset_bonus(bonus);
}

void update_bonus(struct bonus_t *bonus)
{
  INT16 max;
  INT16 min;

  if (bonus->active)
  {
    // fade in & out
         if (bonus->timer > BONUS_PROP_ACTIVE_TIME-(1U*BONUS_PROP_FADE_SPEED)) OBP1_REG = 0x02;
    else if (bonus->timer > BONUS_PROP_ACTIVE_TIME-(2U*BONUS_PROP_FADE_SPEED)) OBP1_REG = 0x42;
    else if (bonus->timer > BONUS_PROP_ACTIVE_TIME-(3U*BONUS_PROP_FADE_SPEED)) OBP1_REG = 0x92;
    else if (bonus->timer > (3U*BONUS_PROP_FADE_SPEED)) OBP1_REG = 0xD2;
    else if (bonus->timer > (2U*BONUS_PROP_FADE_SPEED)) OBP1_REG = 0x92;
    else if (bonus->timer > (1U*BONUS_PROP_FADE_SPEED)) OBP1_REG = 0x42;
    else OBP1_REG = 0x02;

    move_sprite(bonus->spr+0, (bonus->posx>>8)-8, ((UINT16)bonus->posy>>8)-8);
    move_sprite(bonus->spr+1, (bonus->posx>>8)-8, ((UINT16)bonus->posy>>8));
    move_sprite(bonus->spr+2, (bonus->posx>>8),   ((UINT16)bonus->posy>>8)-8);
    move_sprite(bonus->spr+3, (bonus->posx>>8),   ((UINT16)bonus->posy>>8));

    max = bonus->refx+((INT16)(BONUS_PROP_SLIDE_X/2U)<<8);
    min = bonus->refx-((INT16)(BONUS_PROP_SLIDE_X/2U)<<8);

    bonus->posx += bonus->slidex;
    if (bonus->posx > max)
    {
        bonus->posx = max;
        bonus->slidex = -bonus->slidex;
    } 
    if (bonus->posx < min)
    {
        bonus->posx = min;
        bonus->slidex = -bonus->slidex;
    } 

    max = bonus->refy+((INT16)(BONUS_PROP_SLIDE_Y/2U)<<8);
    min = bonus->refy-((INT16)(BONUS_PROP_SLIDE_Y/2U)<<8);

    bonus->posy += bonus->slidey;
    if (bonus->posy > max)
    {
        bonus->posy = max;
        bonus->slidey = -bonus->slidey;
    } 
    if (bonus->posy < min)
    {
        bonus->posy = min;
        bonus->slidey = -bonus->slidey;
    } 
  }
  else
  {
    move_sprite(bonus->spr+0, 0, 0);
    move_sprite(bonus->spr+1, 0, 0);
    move_sprite(bonus->spr+2, 0, 0);
    move_sprite(bonus->spr+3, 0, 0);
  }

  if (bonus->timer > 0)
  {
    bonus->timer--;
  }

  if (bonus->timer == 0)
  {
    bonus->active = !bonus->active;
    if (bonus->active)
    {
      bonus->refx  = (BONUS_WINDOW_MINX+(_rand()%(BONUS_WINDOW_MAXX-BONUS_WINDOW_MINX)))<<8;
      bonus->refy  = (BONUS_WINDOW_MINY+(_rand()%(BONUS_WINDOW_MAXY-BONUS_WINDOW_MINY)))<<8;
      bonus->posx  = bonus->refx;
      bonus->posy  = bonus->refy;
      bonus->type  = BONUS_NONE; // Bonus type to be determined at the collision time.
      bonus->timer = BONUS_PROP_ACTIVE_TIME+(_rand()%60);
    }
    else
    {
      bonus->timer = BONUS_PROP_INACTIVE_TIME+(_rand()%60);
    }
  }
}

void deactivate_bonus(struct bonus_t *bonus, UINT16 extra_time)
{
  reset_bonus(bonus);
  bonus->timer = BONUS_PROP_INACTIVE_TIME+(_rand()%60) + extra_time;
}

UINT8 is_bonus_collidable(struct bonus_t *bonus)
{
  // fade is not collidable
  return 
    bonus->active &&
    (bonus->timer < BONUS_PROP_ACTIVE_TIME-(3U*BONUS_PROP_FADE_SPEED)) &&
    (bonus->timer > (3U*BONUS_PROP_FADE_SPEED));
}