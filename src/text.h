#ifndef TEXT_H
#define TEXT_H

#include <gb/gb.h>

// Text have special sprite management has they can use some existing sprite ids.
//
// The idea is to have the 'always on' sprite with id from 0 to n and the 'overridable' sprites from 39 (max sprite_id) to m
// [xxxxxxxxxxxxxxyyyyyyyyyyyyyzzzzzzzz]
// [always-on -> ]
//                       [      <- text]
//                [overridable]
//
#define TXT_BANK_COUNT     2
#define TXT_MAX_CHAR_COUNT 8

struct char_t
{
    UINT8 tile;
    UINT8 dx;
    UINT8 dy;
};

struct text_t
{
    struct char_t string[TXT_MAX_CHAR_COUNT];
    UINT8 bank;
    UINT8 sprite;
    UINT8 len;
};

void txt_bank_init(UINT8 *lens);

void txt_init(struct text_t *text, UINT8 bank);

void txt_set_string(struct text_t *text, char *str);

void txt_show(struct text_t *text, UINT8 x, UINT8 y);
void txt_show_p(struct text_t *text, UINT8 x, UINT8 y, UINT8 prop);

void txt_hide(struct text_t *text);

#endif