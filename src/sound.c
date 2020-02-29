#include <gb/gb.h>
#include "sound.h"

struct loop_info_t
{
    UINT8 timing; // vbl between each note
    UINT8 repeat; // loop repeat count
    UINT8 start;  // index in data
    UINT8 length; // content length (in notes)
};

struct channel_info_t
{
    UINT8 count; // number of loops
    UINT8 pad0;
    UINT8 pad1;
    UINT8 pad2;
    struct loop_info_t loops[2];
};

struct channel_player_t
{
    UINT8 timer; // timer
    UINT8 curloop; // current loop
    UINT8 currep; // current repeat
    UINT8 curdata;
};

void play_channel(struct channel_player_t *player, struct channel_info_t *info, UINT8 *data, __REG NRx3_REG, __REG NRx4_REG)
{
    UINT8 index;

    if (player->timer == 0)
    {
        // play
        index = player->curdata*2;
        *NRx3_REG = data[index];
        *NRx4_REG = data[index+1]; 

        // next
        player->curdata++;
        if (player->curdata >= (info->loops[player->curloop].start+info->loops[player->curloop].length))
        {
            player->currep++;
            if (player->currep >= info->loops[player->curloop].repeat)
            {
                player->currep = 0;
                player->curloop++;
                if (player->curloop >= info->count)
                {
                    player->curloop = 0;
                }
            }

            player->curdata = info->loops[player->curloop].start;
        }

        player->timer = info->loops[player->curloop].timing;
    }

    player->timer--;
}

struct channel_info_t normal_ch1_info = { 2, 0, 0, 0, {{15,4,0,8},{15,4,8,8}} };
struct channel_info_t normal_ch2_info = { 1, 0, 0, 0, {{120,1,0,4},{0,0,0,0}} };

struct channel_info_t fast_ch1_info = { 2, 0, 0, 0, {{10,4,0,8},{10,4,8,8}} };
struct channel_info_t fast_ch2_info = { 1, 0, 0, 0, {{80,1,0,4},{0,0,0,0}} };

// G6 0x759
// F6 0x744
// E6 0x739
// D6 0x721
// C6 0x706

// G5 0x6B2
// F5 0x689
// E5 0x672
// D5 0x642
// C5 0x60A

// C4 0x416
// B3 0x3DA
// A3 0x356
// G3 0x2C6

UINT8 main_theme_ch1_data[] = { 0x0A,0x86, 0x42,0x86, 0x72,0x86, 0x89,0x86, 0xB2,0x86, 0x89,0x86, 0x72,0x86, 0x42,0x86,  0x06,0x87, 0x21,0x87, 0x39,0x87, 0x44,0x87, 0x59,0x87, 0x44,0x87, 0x39,0x87, 0x21,0x87 };
UINT8 main_theme_ch2_data[] = { 0x16,0x84, 0xDA,0x83, 0x56,0x83, 0xC6,0x82 };

UINT8 reversed_theme_ch1_data[] = { 0x21,0x87, 0x39,0x87, 0x44,0x87, 0x59,0x87, 0x44,0x87, 0x39,0x87, 0x21,0x87, 0x06,0x87,  0x42,0x86, 0x72,0x86, 0x89,0x86, 0xB2,0x86, 0x89,0x86, 0x72,0x86, 0x42,0x86, 0x0A,0x86 };
UINT8 reversed_theme_ch2_data[] = { 0xC6,0x82, 0x56,0x83, 0xDA,0x83, 0x16,0x84 };

struct music_t
{
    UINT8 *ch1_data;
    UINT8 *ch2_data;
};

struct music_t musics[] = 
{
    {main_theme_ch1_data,main_theme_ch2_data},
    {reversed_theme_ch1_data,reversed_theme_ch2_data}
};

UINT8 current_music = 0;
UINT8 current_speed = 0;

struct channel_player_t ch1_player;
struct channel_player_t ch2_player;

void select_music(UINT8 id)
{
    current_music = id;

    ch1_player.curloop = 0;
    ch1_player.timer = 0;
    ch1_player.currep = 0;
    ch1_player.curdata = normal_ch1_info.loops[0].start; // start does not depend of speed...

    ch2_player.curloop = 0;
    ch2_player.timer = 0;
    ch2_player.currep = 0;
    ch2_player.curdata = normal_ch2_info.loops[0].start;
}

void set_music_speed(UINT8 id)
{
    current_speed = id;
}

void start_sound()
{
    NR52_REG = 0x80; // enable sound
    NR51_REG = 0x33; // enable channel 1 & 2 left/right
    NR50_REG = 0x77; // max volume
}

void stop_sound()
{
    NR52_REG = 0x00;
}

void init_sound()
{
    select_music(MUSIC_MAIN_THEME);
    set_music_speed(MUSIC_SPEED_NORMAL);

    start_sound();

    NR11_REG = 0x80; // channel1 setup
    NR12_REG = 0xF0; // channel1 setup

    NR21_REG = 0x80; // channel2 setup
    NR22_REG = 0xF0; // channel2 setup
}

void play_sound()
{
    struct music_t *music = &musics[current_music];

    struct channel_info_t *ch1_info = current_speed == MUSIC_SPEED_NORMAL ? &normal_ch1_info : &fast_ch1_info;
    struct channel_info_t *ch2_info = current_speed == MUSIC_SPEED_NORMAL ? &normal_ch2_info : &fast_ch2_info; // to move all of this when speed change?

    play_channel(&ch1_player, ch1_info, music->ch1_data, (__REG)0xFF13, (__REG)0xFF14);
    play_channel(&ch2_player, ch2_info, music->ch2_data, (__REG)0xFF18, (__REG)0xFF19);
}
