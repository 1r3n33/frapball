#ifndef CONTROL_H
#define CONTROL_H

#include <gb/gb.h>
#include "ball.h"
#include "paddle.h"

#define CONTROL_STOP      0
#define CONTROL_CONTINUE  1
#define CONTROL_PAUSE     2
#define CONTROL_RESUME    3

UINT8 control(struct paddle_t *p1, struct paddle_t *p2, UINT8 mode, UINT8 locked);

#endif