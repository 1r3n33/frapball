#include <gb/gb.h>
#include "missile.h"
#include "graphics.h"
#include "collision.h"
#include "game.h"

struct missile_t missiles[2];
struct paddle_t *target;

#define FREEZE_DURATION 120U

void missile_move_sprite(struct missile_t *m)
{
    move_sprite(m->sprite, m->pos_x, m->pos_y);
}

void missile_move(struct missile_t *m)
{
    m->pos_x += m->speed_x;
    m->pos_y += m->speed_y;

    missile_move_sprite(m);
}

void missile_reset(struct missile_t *m)
{
    m->pos_x = 0;
    m->pos_y = 0;
    m->speed_x = 0;
    m->speed_y = 0;
    missile_move_sprite(m);
}

void missile_init(struct missile_t *m)
{
    m->sprite = alloc_sprites(1);

    set_sprite_tile(m->sprite, SPR_MISSILE_BODY);

    missile_reset(m);
}

void missile_shoot(struct missile_t *m, UINT8 x, UINT8 y, UINT8 side) // side: 0 (left), 1 (right)
{
    m->pos_x = x;
    m->pos_y = y;
    m->speed_x = side ? -2 : 2;
    m->speed_y = 0;

    set_sprite_prop(m->sprite, side ? S_FLIPX : 0);
}

void missile_update(struct missile_t *m)
{
    UINT8 tx;
    UINT8 ty;
    UINT8 thw;
    UINT8 thh;
    UINT8 mx;
    UINT8 my;
    struct intersect_t intersect;

    missile_move(m);
    // check collision
    if (m->pos_x == 0)
    {
        missile_reset(m);
        return;
    }

    thw = get_paddle_pixel_width(target)/2U;
    thh = get_paddle_pixel_height(target)/2U;

    tx = target->posx+thw;    
    ty = target->posy+thh;

    mx = m->pos_x+(TILE_WIDTH/2U);
    my = m->pos_y+(TILE_HEIGHT/2U);

    intersect_circle_rectangle(
        &intersect,
        mx,
        my,
        2,
        tx,
        ty,
        thw,
        thh
    );

    if (intersect.axis)
    {
        missile_reset(m);
        freeze_paddle(target, FREEZE_DURATION);
    }
}

void missile_init_all()
{
    target = 0;
    missile_init(&missiles[0]);
    missile_init(&missiles[1]);
}

void missile_shoot_all(UINT8 side)
{
    struct paddle_t *origin = get_paddle(side);
    
    target = get_paddle(!side);

    missile_shoot(&missiles[0], origin->posx-TILE_WIDTH, origin->posy, 1);
    missile_shoot(&missiles[1], origin->posx-TILE_WIDTH, origin->posy+get_paddle_pixel_height(origin)-TILE_HEIGHT, 1);
}

void missile_update_all()
{
    missile_update(&missiles[0]);
    missile_update(&missiles[1]);
}
