#include <gb/gb.h>
#include "titlescreen.h"
#include "game.h"
#include "link.h"

struct titlescreen_t ts;

void main()
{
    UINT8 play_intro = 1;

    while(1)
    {
        lk_init();

        init_titlescreen(&ts, play_intro);
        loop_titlescreen(&ts);

        init_game(ts.timer_selection, ts.selection);
        loop_game();

        play_intro = 0;
    }
}