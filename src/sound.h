#ifndef SOUND_H
#define SOUND_H

#define MUSIC_MAIN_THEME     0
#define MUSIC_REVERSED_THEME 1

#define MUSIC_SPEED_NORMAL 0
#define MUSIC_SPEED_FAST   1

void init_sound();

void play_sound();

void start_sound();
void stop_sound();

void select_music(UINT8 id);
void set_music_speed(UINT8 id);

#endif