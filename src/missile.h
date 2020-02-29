#ifndef MISSILE_H
#define MISSILE_H

#include "paddle.h"

struct missile_t
{
    UINT8 pos_x;
    UINT8 pos_y;
    INT8 speed_x;
    INT8 speed_y;
    UINT8 sprite;
};

void missile_init_all();
void missile_shoot_all(UINT8 side);
void missile_update_all();

#endif