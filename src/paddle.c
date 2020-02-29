#include <gb/gb.h>
#include "graphics.h"
#include "sound.h"
#include "game.h"
#include "paddle.h"
#include "missile.h"
#include "ball.h"

#define PADDLE_BONUS_TIME 1200U // big/small effect stays 20 sec.
#define PADDLE_SHOOT_TIME 300U  // paddle level 1 shoots bullets every x sec.

// state
#define PADDLE_STATE_SMALL     0x00
#define PADDLE_STATE_NORMAL    0x01
#define PADDLE_STATE_BIG       0x02

// transition, last byte is size
#define PADDLE_STATE_SMALL_TO_NORMAL 0xa0 
#define PADDLE_STATE_NORMAL_TO_SMALL 0xa1
#define PADDLE_STATE_NORMAL_TO_BIG   0xb1
#define PADDLE_STATE_BIG_TO_NORMAL   0xa2

void move_paddle_sprites(struct paddle_t *paddle)
{
    UINT8 i;
    UINT8 x    = paddle->posx;
    UINT8 y    = paddle->posy;
    UINT8 spr  = paddle->spr;
    UINT8 size = paddle->state & 0x0f;

    if (paddle->level < 2)
    {
        move_sprite(spr+0, x, y);
        y += TILE_HEIGHT;

        if (size > PADDLE_STATE_SMALL)
        {
            move_sprite(spr+1, x, y);
            y += TILE_HEIGHT;
        }
        else
        {
            move_sprite(spr+1, 0, 0);
        }

        if (size > PADDLE_STATE_NORMAL)
        {
            move_sprite(spr+2, x, y);
            y += TILE_HEIGHT;
            move_sprite(spr+3, x, y);
            y += TILE_HEIGHT;
        }
        else
        {
            move_sprite(spr+2, 0, 0);
            move_sprite(spr+3, 0, 0);
        }

        move_sprite(spr+4, x, y);

        for (i=5; i<(paddle->side ? 12U : 5U); i++)
        {
            move_sprite(paddle->spr+i, 0, 0);
        }
    }
    else
    {
        for (i=0; i<12; i+=2)
        {
            move_sprite(paddle->spr+i,   x,            y);
            move_sprite(paddle->spr+i+1, x+TILE_WIDTH, y);
            y += TILE_HEIGHT;
        }
    }
}

void set_paddle_sprites(struct paddle_t *paddle, UINT8 level)
{
  UINT8 i;
  UINT8 spr_head;
  UINT8 spr_body;
  UINT8 posx; 
  UINT8 flipx;

  paddle->level = (paddle->side) ? level : 0; // the user paddle (left) is always the level0 paddle
  flipx = (paddle->side) ? S_FLIPX : 0;

  if (paddle->level < 2)
  {
    posx = (paddle->side) ? SPR_XOFFSET+SCREENWIDTH-16U : SPR_XOFFSET+8U;
    paddle->posx = posx;
    paddle->posy = ((SCREENHEIGHT/2U)+SPR_YOFFSET)-(get_paddle_pixel_height(paddle)/2U);

    if (level == 0)
    {
        spr_head = SPR_PADDLE_0_HEAD;
        spr_body = SPR_PADDLE_0_BODY;
    }
    else if (level == 1)
    {
        spr_head = SPR_PADDLE_1_HEAD;
        spr_body = SPR_PADDLE_1_BODY;
    }
    else
    {
        spr_head = SPR_PADDLE_0_HEAD;
        spr_body = SPR_PADDLE_0_BODY;
    }

    set_sprite_tile(paddle->spr, spr_head);
    set_sprite_prop(paddle->spr, flipx);

    set_sprite_tile(paddle->spr+1, spr_body);
    set_sprite_prop(paddle->spr+1, flipx);

    set_sprite_tile(paddle->spr+2, spr_body);
    set_sprite_prop(paddle->spr+2, flipx);

    set_sprite_tile(paddle->spr+3, spr_body);
    set_sprite_prop(paddle->spr+3, flipx);

    set_sprite_tile(paddle->spr+4, spr_head);
    set_sprite_prop(paddle->spr+4, flipx|S_FLIPY);

    if (paddle->side)
    {
        if (paddle->level == 1)
        {
            set_sprite_tile(paddle->spr+5, SPR_MISSILE_HEAD);
            set_sprite_prop(paddle->spr+5, flipx);

            set_sprite_tile(paddle->spr+6, SPR_MISSILE_HEAD);
            set_sprite_prop(paddle->spr+6, flipx);

            for (i=7; i<12; i++)
            {
                set_sprite_tile(paddle->spr+i, 0);
                set_sprite_prop(paddle->spr+i, 0);
            }
        }
        else
        {
            for (i=5; i<12; i++)
            {
                set_sprite_tile(paddle->spr+i, 0);
                set_sprite_prop(paddle->spr+i, 0);
            }
        } 
    }
  }    
  else
  {
    posx = (paddle->side) ? SPR_XOFFSET+SCREENWIDTH-16U : SPR_XOFFSET;
    paddle->posx = posx;
    paddle->posy = ((SCREENHEIGHT/2U)+SPR_YOFFSET)-(get_paddle_pixel_height(paddle)/2U);

    set_sprite_tile(paddle->spr+0, flipx ? SPR_PADDLE_BOSS_HEAD_NE : SPR_PADDLE_BOSS_HEAD_NW);
    set_sprite_prop(paddle->spr+0, flipx);

    set_sprite_tile(paddle->spr+1, flipx ? SPR_PADDLE_BOSS_HEAD_NW : SPR_PADDLE_BOSS_HEAD_NE);
    set_sprite_prop(paddle->spr+1, flipx);

    set_sprite_tile(paddle->spr+2, flipx ? SPR_PADDLE_BOSS_HEAD_SE : SPR_PADDLE_BOSS_HEAD_SW);
    set_sprite_prop(paddle->spr+2, flipx);

    set_sprite_tile(paddle->spr+3, flipx ? SPR_PADDLE_BOSS_HEAD_SW : SPR_PADDLE_BOSS_HEAD_SE);
    set_sprite_prop(paddle->spr+3, flipx);

    set_sprite_tile(paddle->spr+4, flipx ? SPR_PADDLE_BOSS_BODY_NE : SPR_PADDLE_BOSS_BODY_NW);
    set_sprite_prop(paddle->spr+4, flipx);

    set_sprite_tile(paddle->spr+5, flipx ? SPR_PADDLE_BOSS_BODY_NW : SPR_PADDLE_BOSS_BODY_NE);
    set_sprite_prop(paddle->spr+5, flipx);

    set_sprite_tile(paddle->spr+6, flipx ? SPR_PADDLE_BOSS_BODY_NE : SPR_PADDLE_BOSS_BODY_NW);
    set_sprite_prop(paddle->spr+6, flipx|S_FLIPY);

    set_sprite_tile(paddle->spr+7, flipx ? SPR_PADDLE_BOSS_BODY_NW : SPR_PADDLE_BOSS_BODY_NE);
    set_sprite_prop(paddle->spr+7, flipx|S_FLIPY);

    set_sprite_tile(paddle->spr+8, flipx ? SPR_PADDLE_BOSS_HEAD_SE : SPR_PADDLE_BOSS_HEAD_SW);
    set_sprite_prop(paddle->spr+8, flipx|S_FLIPY);

    set_sprite_tile(paddle->spr+9, flipx ? SPR_PADDLE_BOSS_HEAD_SW : SPR_PADDLE_BOSS_HEAD_SE);
    set_sprite_prop(paddle->spr+9, flipx|S_FLIPY);

    set_sprite_tile(paddle->spr+10, flipx ? SPR_PADDLE_BOSS_HEAD_NE : SPR_PADDLE_BOSS_HEAD_NW);
    set_sprite_prop(paddle->spr+10, flipx|S_FLIPY);

    set_sprite_tile(paddle->spr+11, flipx ? SPR_PADDLE_BOSS_HEAD_NW : SPR_PADDLE_BOSS_HEAD_NE);
    set_sprite_prop(paddle->spr+11, flipx|S_FLIPY);
  }

  move_paddle_sprites(paddle);

  // if (paddle->level) //(paddle->level == 1 || paddle->level == 2)
  {
      paddle->timer_shoot = PADDLE_SHOOT_TIME;
  }
}

void paddle_reset(struct paddle_t *paddle, UINT8 level)
{
    paddle->state = PADDLE_STATE_NORMAL;

    paddle->timer_effect_size = 0U;
    paddle->timer_effect_freeze = 0U;
    paddle->timer_effect_inverted = 0U;
    paddle->timer_shoot = 0U; // Always set in set_paddle_sprites. Remove it.

    paddle->inverted = 0U;

    set_paddle_sprites(paddle, level);
}

void init_paddle(struct paddle_t *paddle, UINT8 side, UINT8 level, UINT8 is_ai)
{
    paddle->spr = alloc_sprites((side==0) ? 5U : 12U); // For 1 player game the paddle on the right can use a lot of sprite.

    paddle->side = side;
    paddle->ai = is_ai;

    paddle_reset(paddle, level);
}

void move_paddle(struct paddle_t *paddle, INT8 dx, INT8 dy)
{
    UINT8 size = get_paddle_pixel_height(paddle);

    if (paddle->timer_effect_freeze > 0)
    {
        return;
    }

    if (paddle->inverted == 2)
    {
        paddle->posx -= dx;
        paddle->posy -= dy;
    }
    else
    {
        paddle->posx += dx;
        paddle->posy += dy;
    }

    // Collision check
    if (paddle->posy < TOP_BORDER)
    {
        paddle->posy = TOP_BORDER;
    }
    if (paddle->posy > (BOTTOM_BORDER-size))
    {
        paddle->posy = (BOTTOM_BORDER-size);
    }

    move_paddle_sprites(paddle);
}

void set_paddle_position(struct paddle_t *paddle, INT8 y)
{
    paddle->posy = y;
    move_paddle_sprites(paddle);
}

void set_paddle_big(struct paddle_t *paddle)
{
  if (paddle->state == PADDLE_STATE_NORMAL) // not needed if bonus timing is right
  {
    paddle->state = PADDLE_STATE_NORMAL_TO_BIG;
    paddle->timer_effect_size = PADDLE_BONUS_TIME;
  }
}

void set_paddle_small(struct paddle_t *paddle)
{
  if (paddle->state == PADDLE_STATE_NORMAL) // not needed if bonus timing is right
  {
    paddle->state = PADDLE_STATE_NORMAL_TO_SMALL;
    paddle->timer_effect_size = PADDLE_BONUS_TIME;
  }
}

void invert_paddle_direction(struct paddle_t *paddle)
{
    paddle->inverted = 1; // what if inverted already?
    paddle->timer_effect_inverted = PADDLE_BONUS_TIME;
}

UINT8 get_paddle_pixel_width(struct paddle_t *paddle)
{
    return (paddle->level == 2) ? 2U*TILE_WIDTH : TILE_WIDTH;
}

UINT8 get_paddle_pixel_height(struct paddle_t *paddle)
{
    UINT8 size;
    
    if (paddle->level == 2)
    {
        return 6U*TILE_HEIGHT;
    }
    
    size = paddle->state & 0x0f;

    if (size == PADDLE_STATE_SMALL)
    {
        return 2U*TILE_HEIGHT;
    }
    else if (size == PADDLE_STATE_NORMAL)
    {
        return 3U*TILE_HEIGHT;
    }
    else if (size == PADDLE_STATE_BIG)
    {
        return 5U*TILE_HEIGHT;
    }
    else
    {
        return 0;
    }
}

void update_paddle_effect_size(struct paddle_t *paddle);
void update_paddle_effect_freeze(struct paddle_t *paddle);
void update_paddle_effect_inverted(struct paddle_t *paddle);

void update_paddle(struct paddle_t *paddle)
{
  UINT8 i;

  if (paddle->state != PADDLE_STATE_NORMAL || paddle->inverted)
  {
    if (paddle->timer_effect_size >= PADDLE_BONUS_TIME-30U)
    {
        if ((paddle->timer_effect_size/2U)%2)
        {
            move_paddle_sprites(paddle);
        }
        else
        {
            for (i=0; i<(paddle->side ? 12U : 5U); i++)
            {
                move_sprite(paddle->spr+i, 0, 0);
            }
        }
    }    
    
    if (paddle->timer_effect_size == PADDLE_BONUS_TIME-30U)
    {
        if (paddle->state == PADDLE_STATE_NORMAL_TO_SMALL)
        {
            paddle->state = PADDLE_STATE_SMALL;
            move_paddle(paddle, 0, (TILE_HEIGHT/2U));
        }
        else if (paddle->state == PADDLE_STATE_NORMAL_TO_BIG)
        {
            paddle->state = PADDLE_STATE_BIG;
            move_paddle(paddle, 0, -(TILE_HEIGHT/2U));
        }
        else if (paddle->inverted == 1)
        {
            paddle->inverted = 2;
            select_music(MUSIC_REVERSED_THEME);
        }
    }
    
    if (paddle->timer_effect_size == 30U)
    {
        if (paddle->state == PADDLE_STATE_SMALL)
        {
            paddle->state = PADDLE_STATE_SMALL_TO_NORMAL;
        }
        else if (paddle->state == PADDLE_STATE_BIG)
        {
            paddle->state = PADDLE_STATE_BIG_TO_NORMAL;
        }
    }

    if (paddle->timer_effect_size <= 30U)
    {
        if ((paddle->timer_effect_size/2U)%2)
        {
            move_paddle_sprites(paddle);
        }
        else
        {
            for (i=0; i<(paddle->side ? 12U : 5U); i++)
            {
                move_sprite(paddle->spr+i, 0, 0);
            }
        }
    }

    if (paddle->timer_effect_size == 0U)
    {
        if (paddle->state == PADDLE_STATE_SMALL_TO_NORMAL)
        {
            paddle->state = PADDLE_STATE_NORMAL;
            move_paddle(paddle, 0, -(TILE_HEIGHT/2U));
        }
        else if (paddle->state == PADDLE_STATE_BIG_TO_NORMAL)
        {
            paddle->state = PADDLE_STATE_NORMAL;
            move_paddle(paddle, 0, (TILE_HEIGHT/2U));
        }
        else if (paddle->inverted)
        {
            paddle->inverted = 0;
            move_paddle(paddle, 0, 0);
            select_music(MUSIC_MAIN_THEME);
        }
    }
  }    

  if (paddle->timer_effect_size > 0)
  {
    paddle->timer_effect_size--;
  }

  if (paddle->timer_effect_freeze > 0)
  {
      paddle->timer_effect_freeze--;
  }

  if (paddle->level == 1 || paddle->level == 2)
  {
    if (paddle->timer_shoot < 60U)
    {
        if (paddle->level == 1)
        {
            // missile head showing up
            move_sprite(paddle->spr+5, paddle->posx-TILE_WIDTH, paddle->posy);
            move_sprite(paddle->spr+6, paddle->posx-TILE_WIDTH, paddle->posy+get_paddle_pixel_height(paddle)-TILE_HEIGHT);
        }

        if (paddle->level == 2)
        {
            // opening mouth
        }
    }     

    paddle->timer_shoot--;
    if (paddle->timer_shoot == 0)
    {
        if (paddle->level == 1)
        {
            missile_shoot_all(paddle->side);
            move_sprite(paddle->spr+5, 0, 0);
            move_sprite(paddle->spr+6, 0, 0);
        }

        if (paddle->level == 2)
        {
            if (!ball_is_activated(BALL_BLACK))
            {
                // TODO: all of this could be in a spawn method and reuse in bonus code
                ball_set_position(BALL_BLACK, paddle->posx-TILE_WIDTH, paddle->posy+(get_paddle_pixel_height(paddle)/2U)-(TILE_HEIGHT/2U));
                ball_init_speed(BALL_BLACK, BALL_BLACK_SPEED, paddle->side);
                ball_set_last_hit(BALL_BLACK, paddle->side);
                ball_activate(BALL_BLACK);
            }
        }

        paddle->timer_shoot = PADDLE_SHOOT_TIME;
    }
  }
}

void freeze_paddle(struct paddle_t *p, UINT8 t)
{
    p->timer_effect_freeze = t;
}