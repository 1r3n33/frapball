#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <gb/gb.h>

#define DARK_MODE_DURATION 600

void bkg_set_tiles();

void bkg_lightmode(UINT8 level);
void bkg_darkmode();

void bkg_level_transition(UINT8 cur_level, UINT8 next_level, UINT8 tick);

void bkg_update(UINT8 level);
void bkg_exit_darkmode(UINT8 level);

UINT8 bkg_is_light();
UINT8 bkg_is_dark();

#endif