#include <gb/gb.h>
#include "background.h"
#include "sound.h"

const unsigned char bkg_tiles[] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // white
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, // black
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0x81,0xFF,0xBD,0xC3,0xA5,0xC3, // wall lvl0
  0xA5,0xC3,0xBD,0xC3,0x81,0xFF,0xFF,0xFF,
  0xFF,0xFF,0x99,0x00,0x66,0x00,0x66,0x00, // wall lvl1
  0x99,0x00,0x99,0x00,0x66,0x00,0xFF,0xFF,
  0xFF,0xFF,0x80,0xC1,0x80,0x80,0x80,0xB6, // wall lvl2
  0x80,0x94,0x80,0xC1,0x80,0xD5,0xFF,0xFF
};

void bkg_set_tiles()
{
  set_bkg_data(0, 5, bkg_tiles);
}

UINT16 dark_mode_timer = 0;
UINT16 PAD0;

void bkg_lightmode(UINT8 level)
{
  UINT8 i, j;
  UINT8 blank;
  UINT8 wall;

  if (OBP0_REG != 0xD2)
  {
    OBP0_REG = 0xD2;

    blank = 0;
    for(i = 0; i < 20; i++)
        for(j = 0; j < 18; j++)
            set_bkg_tiles(i, j, 1, 1, &blank);

    wall = 2 + level;
    for(i = 0; i < 20; i++)
    {
        set_bkg_tiles(i, 0,  1, 1, &wall);
        set_bkg_tiles(i, 17, 1, 1, &wall);
    }

    init_sound();
  }

  dark_mode_timer = 0;
}

void bkg_darkmode()
{
  UINT8 i, j;
  UINT8 dark;

  if (OBP0_REG != 0xF2)
  {
    OBP0_REG = 0xF2; // dark sprites

    dark = 1;
    for(i = 0; i < 20; i++)
        for(j = 0; j < 18; j++)
            set_bkg_tiles(i, j, 1, 1, &dark);

    stop_sound();
  }

  dark_mode_timer = DARK_MODE_DURATION;
}

void bkg_level_transition(UINT8 cur_level, UINT8 next_level, UINT8 tick)
{
  UINT8 cur_wall = 2+cur_level;
  UINT8 next_wall = 2+next_level;

  UINT8 beg = tick/8;

  if (beg > 0)
  {
      set_bkg_tiles(beg-1, 0,  1, 1, &next_wall);
      set_bkg_tiles(beg-1, 17, 1, 1, &next_wall);
  }

  if (beg+20 < 32)
  {
      set_bkg_tiles(beg+20, 0,  1, 1, &next_wall);
      set_bkg_tiles(beg+20, 17, 1, 1, &next_wall);
  }
}

void bkg_update(UINT8 level)
{
  if (dark_mode_timer > 0)
  {
    dark_mode_timer--;
    if (dark_mode_timer == 0)
    {
      bkg_lightmode(level);
    }
  }
}

void bkg_exit_darkmode(UINT8 level)
{
    dark_mode_timer = 0;
    bkg_lightmode(level);
}

UINT8 bkg_is_light()
{
  return !dark_mode_timer;
}

UINT8 bkg_is_dark()
{
  return dark_mode_timer;
}
