#ifndef GAME_H
#define GAME_H

#define TOP_BORDER    (SPR_YOFFSET+TILE_WIDTH)
#define BOTTOM_BORDER ((SCREENHEIGHT+SPR_YOFFSET)-TILE_HEIGHT)

#define SIDE_LEFT  0U
#define SIDE_RIGHT 1U

#define GAME_MODE_1_PLAYER  0
#define GAME_MODE_2_PLAYERS 1
#define GAME_MODE_LINK      2
#define GAME_MODE_AI        3

#define GAME_STOP     0
#define GAME_CONTINUE 1

void init_game(UINT16 seed, UINT8 mode);

void loop_game();

struct paddle_t *get_paddle(UINT8 id);

#endif