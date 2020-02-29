#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <gb/gb.h>

struct titlescreen_t
{
    UINT8  posx;
    UINT8  posy;
    UINT16 timer_intro;
    UINT16 timer_selection;
    UINT16 timeout; // enter the ai vs ai demonstration mode when 0
    UINT8  spr_cursor; // NOT USED
    UINT8  selection; // 0 (1 player game), 1 (2 players game), 2 (link game), 3 (ai vs ai) see game.h
    UINT8  pad1;
    UINT8  pad2;
};

void init_titlescreen(struct titlescreen_t *ts, UINT8 play_intro);

void loop_titlescreen(struct titlescreen_t *ts);

#endif