#ifndef SCORE_H
#define SCORE_H

#define SCORE_MAX 3

struct score_t
{
    UINT8 spr_player1;
    UINT8 spr_player2;
    UINT8 spr_separator;
    UINT8 pad0;
    
    UINT8 pt_player1;
    UINT8 pt_player2;
    UINT8 pad1;
    UINT8 pad2;

    UINT8  active;
    UINT8  pad3;
    UINT16 timer;
};

void score_init();

void score_reset();

void score_add(UINT8 pt1, UINT8 pt2);

void score_update();

// 0 if no player reach max score, 1 if player1 reach max score, 2 if player2 reach max score.
UINT8 score_has_winner();

#endif