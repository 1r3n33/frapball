#include <gb/gb.h>
#include "text.h"
#include "graphics.h"

struct txt_allocator_t
{
    UINT8 banks[TXT_BANK_COUNT];
};

struct txt_display_manager_t
{
    UINT8 banks[TXT_BANK_COUNT];
    UINT8 backup_tiles[TXT_BANK_COUNT][TXT_MAX_CHAR_COUNT];
    UINT8 backup_props[TXT_BANK_COUNT][TXT_MAX_CHAR_COUNT];
};

struct txt_allocator_t allocator;
struct txt_display_manager_t display_manager;

void txt_bank_init(UINT8 *lens)
{
    UINT8 i,j;

    UINT8 id = 40;
    for (i=0; i<TXT_BANK_COUNT; i++)
    {
        id = id-lens[i];
        allocator.banks[i] = id;

        display_manager.banks[i] = 0;
        for (j=0; j<TXT_MAX_CHAR_COUNT; j++)
        {
            display_manager.backup_tiles[i][j] = 0;
            display_manager.backup_props[i][j] = 0;
        }
    }
}

void txt_init(struct text_t *t, UINT8 bank)
{
    UINT8 i;

    t->bank   = 0;
    t->sprite = allocator.banks[bank];
    t->len    = 0;

    for (i=0; i<TXT_MAX_CHAR_COUNT; i++)
    {
        t->string[i].tile = 0;
        t->string[i].dx = 0;
        t->string[i].dy = 0;
    }
}

UINT8 txt_is_alpha(char c)
{
    return (c >= 'a' && c <= 'z');
}

UINT8 txt_is_digit(char c)
{
    return (c >= '0' && c <= '9'); 
}

UINT8 txt_is_space(char c)
{
    return (c == ' ');
}

UINT8 txt_is_linebreak(char c)
{
    return (c == '\n');
}

UINT8 txt_is_hyphen(char c)
{
    return (c == '-');
}

UINT8 txt_char_to_tile(char c)
{
    if (txt_is_alpha(c))
    {
        return SPR_CHAR_A + (c-'a');
    }
    else if (txt_is_digit(c))
    {
        return SPR_CHAR_0 + (c-'0');
    }
    else if (txt_is_hyphen(c))
    {
        return SPR_CHAR_HYPHEN;
    }
    else
    {
        return 0;
    }
}

void txt_set_string(struct text_t *t, char *str)
{
    UINT8 i;
    UINT8 dx;
    UINT8 dy;
    
    i = 0;
    dx = 0;
    dy = 0;

    while (*str && i<TXT_MAX_CHAR_COUNT)
    {
        char c = *str;
        if (txt_is_space(c))
        {
            dx += TILE_WIDTH;
        }
        else if (txt_is_linebreak(c))
        {
            dy += TILE_HEIGHT*2U;
        }
        else
        {
            t->string[i].tile = txt_char_to_tile(*str);
            t->string[i].dx = dx;
            t->string[i].dy = dy;
            dx += TILE_WIDTH;
            i++;
        }

        str++;
    }

    t->len = i;
}

void txt_show_p(struct text_t *t, UINT8 x, UINT8 y, UINT8 prop)
{
    UINT8 i;

    for (i=0; i<t->len; i++)
    {
        set_sprite_tile(t->sprite+i, t->string[i].tile);
        set_sprite_prop(t->sprite+i, prop);
        move_sprite(t->sprite+i, x+t->string[i].dx, y+t->string[i].dy);
    }

    display_manager.banks[t->bank] = 1;
}

void txt_show(struct text_t *t, UINT8 x, UINT8 y)
{
    txt_show_p(t, x, y, 0);
}

void txt_hide(struct text_t *t)
{
    UINT8 i;

    if (display_manager.banks[t->bank]) // display of this bank
    {
        // restore state
        for (i=0; i<TXT_MAX_CHAR_COUNT && (t->sprite+i)<40; i++)
        {
            move_sprite(t->sprite+i, 0, 0);
        }
    }

    display_manager.banks[t->bank] = 0;
}