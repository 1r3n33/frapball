#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <gb/gb.h>

// core lib, include early.

// may exist in the gbdk
#define SPR_XOFFSET 8U
#define SPR_YOFFSET 16U
#define TILE_WIDTH  8U
#define TILE_HEIGHT 8U

#define SPR_BALL                0
#define SPR_PADDLE_0_HEAD       1
#define SPR_PADDLE_0_BODY       2
#define SPR_BLANK               3
#define SPR_BONUS_NW            4
#define SPR_BONUS_NE            5
#define SPR_BONUS_SW            6
#define SPR_BONUS_SE            7
#define SPR_PADDLE_1_HEAD       8
#define SPR_PADDLE_1_BODY       9
#define SPR_PADDLE_BOSS_HEAD_NW 10
#define SPR_PADDLE_BOSS_HEAD_SW 11
#define SPR_PADDLE_BOSS_HEAD_NE 12
#define SPR_PADDLE_BOSS_HEAD_SE 13
#define SPR_PADDLE_BOSS_BODY_NW 14
#define SPR_PADDLE_BOSS_BODY_NE 15
#define SPR_MISSILE_HEAD        16
#define SPR_MISSILE_BODY        17
#define SPR_BLACK_BALL          18

#define SPR_CHAR        128
#define SPR_CHAR_0      128
#define SPR_CHAR_1      129
#define SPR_CHAR_2      130
#define SPR_CHAR_3      131
#define SPR_CHAR_4      132
#define SPR_CHAR_5      133
#define SPR_CHAR_6      134
#define SPR_CHAR_7      135
#define SPR_CHAR_8      136
#define SPR_CHAR_9      137
#define SPR_CHAR_A      138
#define SPR_CHAR_B      139
#define SPR_CHAR_C      140
#define SPR_CHAR_D      141
#define SPR_CHAR_E      142
#define SPR_CHAR_F      143
#define SPR_CHAR_G      144
#define SPR_CHAR_H      145
#define SPR_CHAR_I      146
#define SPR_CHAR_J      147
#define SPR_CHAR_K      148
#define SPR_CHAR_L      149
#define SPR_CHAR_M      150
#define SPR_CHAR_N      151
#define SPR_CHAR_O      152
#define SPR_CHAR_P      153
#define SPR_CHAR_Q      154
#define SPR_CHAR_R      155
#define SPR_CHAR_S      156
#define SPR_CHAR_T      157
#define SPR_CHAR_U      158
#define SPR_CHAR_V      159
#define SPR_CHAR_W      160
#define SPR_CHAR_X      161
#define SPR_CHAR_Y      162
#define SPR_CHAR_Z      163
#define SPR_CHAR_SPACE  164
#define SPR_CHAR_HYPHEN 165

#define SPR_CHAR_0_ALT      166
#define SPR_CHAR_1_ALT      167
#define SPR_CHAR_2_ALT      168
#define SPR_CHAR_3_ALT      169
#define SPR_CHAR_4_ALT      170
#define SPR_CHAR_5_ALT      171
#define SPR_CHAR_6_ALT      172
#define SPR_CHAR_7_ALT      173
#define SPR_CHAR_8_ALT      174
#define SPR_CHAR_9_ALT      175
#define SPR_CHAR_HYPHEN_ALT 176
#define SPR_CHAR_A_ALT      177
#define SPR_CHAR_B_ALT      178
#define SPR_CHAR_C_ALT      179
#define SPR_CHAR_D_ALT      180
#define SPR_CHAR_E_ALT      181
#define SPR_CHAR_F_ALT      182
#define SPR_CHAR_G_ALT      183
#define SPR_CHAR_H_ALT      184
#define SPR_CHAR_I_ALT      185
#define SPR_CHAR_J_ALT      186
#define SPR_CHAR_K_ALT      187
#define SPR_CHAR_L_ALT      188
#define SPR_CHAR_M_ALT      189
#define SPR_CHAR_N_ALT      190
#define SPR_CHAR_O_ALT      191
#define SPR_CHAR_P_ALT      192
#define SPR_CHAR_Q_ALT      193
#define SPR_CHAR_R_ALT      194
#define SPR_CHAR_S_ALT      195
#define SPR_CHAR_T_ALT      196
#define SPR_CHAR_U_ALT      197
#define SPR_CHAR_V_ALT      198
#define SPR_CHAR_W_ALT      199
#define SPR_CHAR_X_ALT      200
#define SPR_CHAR_Y_ALT      201
#define SPR_CHAR_Z_ALT      202

void set_sprite_symbols();

void reset_sprites();

UINT8 alloc_sprites(UINT8 count);

#endif
