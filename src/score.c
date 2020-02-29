#include <gb/gb.h>
#include "graphics.h"
#include "score.h"

#define SCORE_ACTIVE_TIME  90U
#define SCORE_FADE_SPEED    4U

struct score_t score;

void score_init()
{
    score.spr_player1   = alloc_sprites(1);
    score.spr_player2   = alloc_sprites(1);
    score.spr_separator = alloc_sprites(1);

    set_sprite_tile(score.spr_separator, SPR_CHAR_HYPHEN);
    set_sprite_prop(score.spr_separator, S_PALETTE);

    score_reset();
}

void score_reset()
{
    score.pt_player1 = 0;
    score.pt_player2 = 0;
    score.active     = 0;
    score.timer      = 0;

    set_sprite_prop(score.spr_player1, S_PALETTE);

    set_sprite_prop(score.spr_player2, S_PALETTE);

    score_add(0, 0); // score_add triggers a display.
}

void score_add(UINT8 pt1, UINT8 pt2)
{
    score.pt_player1 += pt1;
    score.pt_player2 += pt2;

    set_sprite_tile(score.spr_player1, SPR_CHAR+score.pt_player1);
    set_sprite_tile(score.spr_player2, SPR_CHAR+score.pt_player2);

    score.active = 1;
    score.timer = SCORE_ACTIVE_TIME;
}

void score_update()
{
    UINT8 mid_screen;

    if (score.active)
    {
        // fade in & out
             if (score.timer > SCORE_ACTIVE_TIME-(1U*SCORE_FADE_SPEED)) OBP1_REG = 0x02;
        else if (score.timer > SCORE_ACTIVE_TIME-(2U*SCORE_FADE_SPEED)) OBP1_REG = 0x42;
        else if (score.timer > SCORE_ACTIVE_TIME-(3U*SCORE_FADE_SPEED)) OBP1_REG = 0x92;
        else if (score.timer > (3U*SCORE_FADE_SPEED)) OBP1_REG = 0xD2;
        else if (score.timer > (2U*SCORE_FADE_SPEED)) OBP1_REG = 0x92;
        else if (score.timer > (1U*SCORE_FADE_SPEED)) OBP1_REG = 0x42;
        else OBP1_REG = 0x02;

        mid_screen = (SCREENWIDTH/2U)+SPR_XOFFSET;
        move_sprite(score.spr_player1,   mid_screen-(TILE_WIDTH*2U), 40);
        move_sprite(score.spr_player2,   mid_screen+(TILE_WIDTH),    40);
        move_sprite(score.spr_separator, mid_screen-(TILE_WIDTH/2U), 40);

        if (score.timer > 0)
        {
            score.timer--;
        }
        else
        {
            score.active = 0;
        }
    }
    else
    {
        move_sprite(score.spr_player1,   0, 0);
        move_sprite(score.spr_player2,   0, 0);
        move_sprite(score.spr_separator, 0, 0);
    }
}

// 0 if no player reach max score, 1 if player1 reach max score, 2 if player2 reach max score.
UINT8 score_has_winner()
{
    if (score.pt_player1 >= SCORE_MAX)
    {
        return 1;
    }

    if (score.pt_player2 >= SCORE_MAX)
    {
        return 2;
    }

    return 0;
}
