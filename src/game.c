/*
 * Game made by Irenee & Ulysse
 * 
 */

#include <gb/gb.h>
#include <gb/rand.h>
#include "graphics.h"
#include "sound.h"
#include "game.h"
#include "bonus.h"
#include "paddle.h"
#include "score.h"
#include "ball.h"
#include "control.h"
#include "link.h"
#include "collision.h"
#include "background.h"
#include "missile.h"
#include "text.h"

const unsigned char spr_tiles[] =
{
  0x3C,0x3C,0x42,0x7E,0xA1,0xDF,0x81,0xFF, // ball
  0x81,0xFF,0x81,0xFF,0x42,0x7E,0x3C,0x3C,
  0x0F,0x0F,0x31,0x3F,0x61,0x5F,0x61,0x5F, // paddle lvl0 top
  0xC1,0xBF,0xC1,0xBF,0xC1,0xBF,0xC1,0xBF,
  0xC1,0xBF,0xC1,0xBF,0xC1,0xBF,0xC1,0xBF, // paddle lvl0 body
  0xC1,0xBF,0xC1,0xBF,0xC1,0xBF,0xC1,0xBF,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // blank
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x38,0x38,0x6C,0x54,0xBA,0xFE,0x77,0x55, // bonus
  0x2D,0x3F,0x1A,0x15,0x7F,0x7F,0x82,0xFF,
  0x83,0xFE,0xFE,0xFF,0xAB,0xD6,0xD6,0xAB, // bonus
  0xFF,0xFE,0x82,0xFF,0x83,0xFE,0x7F,0x7F,
  0x1C,0x1C,0x36,0x2A,0x5D,0x7F,0xEE,0xAA, // bonus
  0xB4,0xFC,0x58,0xA8,0xFE,0xFE,0xC1,0x7F,
  0x41,0xFF,0xFF,0x7F,0x55,0xEB,0xEB,0x55, // bonus
  0x7F,0xFF,0xC1,0x7F,0x41,0xFF,0xFE,0xFE,
  0x7E,0x7E,0xC7,0xBF,0xC9,0xB7,0xC9,0xB7, // paddle lvl1 top
  0xC9,0xB7,0xC9,0xB7,0xC7,0xBF,0xC2,0xBE,
  0xC2,0xBE,0xC2,0xBE,0xC2,0xBE,0xC2,0xBE, // paddle lvl1 body
  0xC2,0xBE,0xC2,0xBE,0xC2,0xBE,0xC2,0xBE,
  0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04, // paddle boss head nw
  0x03,0x03,0x03,0x02,0x03,0x02,0x01,0x01,
  0x01,0x01,0x07,0x06,0x0E,0x09,0x38,0x37, // paddle boss head sw
  0x0E,0x09,0x07,0x06,0x01,0x01,0x07,0x06,
  0x04,0x04,0x04,0x04,0x0E,0x0A,0x1B,0x15, // paddle boss head ne
  0x99,0x97,0xF1,0x6F,0x61,0x9F,0xE1,0x7F,
  0xF1,0x5F,0x79,0xCF,0x79,0xDF,0x79,0xCF, // paddle boss head se
  0x39,0xEF,0x19,0xFF,0x85,0x7F,0x01,0xFF,
  0x0C,0x0B,0x38,0x37,0x0E,0x09,0x07,0x06, // paddle boss body nw
  0x01,0x01,0x07,0x06,0x2E,0x29,0x18,0x17,
  0x03,0xFF,0x07,0xFD,0x0F,0xFF,0x0F,0xFB, // paddle boss body ne
  0x8F,0x7F,0x0F,0xFF,0x5D,0xF7,0x5D,0xBF,
  0x00,0x00,0x00,0x00,0xC0,0xC0,0x60,0xA0, // missile head for paddle lvl 1
  0x60,0xA0,0xC0,0xC0,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0xB8,0xB8,0x4C,0xF4, // missile body for paddle lvl 1
  0x4C,0xF4,0xB8,0xB8,0x00,0x00,0x00,0x00,
  0x3C,0x3C,0x7E,0x7E,0xFF,0xDF,0xFF,0xFF, // black ball for paddle boss
  0xFF,0xFF,0xFF,0xFF,0x7E,0x7E,0x3C,0x3C
};

struct text_t txt_youwin;
struct text_t txt_youlose;
struct text_t txt_ready;
struct text_t txt_pause;

UINT8 lvl_current;

void update_ball(
  struct ball_t *ball,
  struct ball_t *extra_ball,
  struct paddle_t *paddle0,
  struct paddle_t *paddle1,
  struct bonus_t *bonus
  )
{
  UINT8 ballx;
  UINT8 bally;

  ball->posx += ball->speed_x;
  ball->posy += ball->speed_y;

  if (is_bonus_collidable(bonus))
  {
    intersect_ball_bonus(ball, extra_ball, bonus, ball->last_hit ? paddle1 : paddle0);
  }

  intersect_ball_paddle(ball, paddle0, 0);
  intersect_ball_paddle(ball, paddle1, 1);
  collision_ball_y(ball, TOP_BORDER, BOTTOM_BORDER-TILE_HEIGHT);

  ballx = (ball->posx)>>8;
  bally = (ball->posy)>>8;
  move_sprite(ball->spr, ballx, bally);
}

void check_score(UINT8 ball_id)
{
  UINT8 ballx;
  UINT8 bally;

  struct ball_t *ball = ball_get(ball_id);

  ballx = (ball->posx)>>8;
  bally = (ball->posy)>>8;

  // entering score zone
  if (ballx > SCREENWIDTH+TILE_WIDTH)
  {
    // player 1
    if (ball->speed_x > 0)
    {
      if (ballx > (SCREENWIDTH+(TILE_WIDTH*3U)))
      {
        ball_deactivate(ball_id); // just activated flag to zero?
        score_add(1, 0);
      }
    }
    // player 2
    else
    {
      if (ballx < 256-(TILE_WIDTH*2))
      {
        ball_deactivate(ball_id); // just activated flag to zero?
        score_add(0, 1);
      }
    }
  }
}

UINT8 get_ready_timer;
UINT8 game_paused;

// Returns 1 if the game can continue, 0 is over. <- UNUSED, remove it.
UINT8 update(struct paddle_t *paddle0, struct paddle_t *paddle1, struct bonus_t *bonus)
{
  UINT8 has_winner;

  struct ball_t *ball1;
  struct ball_t *ball2;
  struct ball_t *ball3;

  has_winner = 0;
  score_update();

  if (!game_paused)
  {
    missile_update_all();
  }

  if (get_ready_timer > 0)
  {
    get_ready_timer--;

    if (get_ready_timer < 30)
    {
      if ((get_ready_timer>>1) & 1)
      {
        txt_show(&txt_ready, 68, 80);
      }
      else
      {
        txt_hide(&txt_ready);
      }
    }

    if (get_ready_timer == 0)
    {
      ball_activate(BALL_MAIN);
      game_paused = 0;
    }

    return 1;
  }

  ball1 = ball_get(BALL_MAIN);
  ball2 = ball_get(BALL_BONUS);
  ball3 = ball_get(BALL_BLACK);

  bkg_update(lvl_current);

  update_bonus(bonus);

  if (ball1->activated)
  {
    update_ball(ball1, ball2, paddle0, paddle1, bonus);
    check_score(BALL_MAIN);
  }

  if (ball2->activated)
  {
    update_ball(ball2, ball2, paddle0, paddle1, bonus);
    check_score(BALL_BONUS);
  }

  if (ball3->activated)
  {
    update_ball(ball3, ball3, paddle0, paddle1, bonus);
    check_score(BALL_BLACK);
  }

  update_paddle(paddle0);
  update_paddle(paddle1);

  if (!ball1->activated && !ball2->activated && !ball3->activated)
  {
    ball_reset(BALL_MAIN);
    ball_activate(BALL_MAIN); // The paddle will go back to the initial position.
    
    reset_bonus(bonus);

    bkg_exit_darkmode(lvl_current); // This is not immediate. It needs a update to be effective.
    
    set_music_speed(MUSIC_SPEED_NORMAL);

    has_winner = (score_has_winner() != 0);
    if (!has_winner)
    {
      txt_show(&txt_ready, 68, 80);
      get_ready_timer = 60;
    }
  }

  return !has_winner;
}

struct paddle_t      paddle0;
struct paddle_t      paddle1;
struct bonus_t       bonus;

UINT8 game_mode;

void init_game(UINT16 seed, UINT8 mode)
{
  UINT8 i;
  UINT8 txt_lens[TXT_BANK_COUNT] = { TXT_MAX_CHAR_COUNT, 0 };

  DISPLAY_OFF;

  initrand(seed);
  game_mode = mode;

  lvl_current = 0;

  reset_sprites();
	
  //OBP0_REG = 0xD2; // custom palette
  //OBP1_REG = 0xD2; // custom palette
  set_sprite_data(0, 19, spr_tiles);
  //set_sprite_data(128, 37, symbols);
  for (i=0; i<40; i++)
  {
    move_sprite(i, 0, 0);
  }

  // todo: bkg_init()
  move_bkg(0,0);
  bkg_set_tiles();
  
  //bkg_lightmode(0);
  //hack
  bkg_darkmode();
  bkg_exit_darkmode(lvl_current);

  ball_init_all();

  init_paddle(&paddle0, 0, 0, mode == GAME_MODE_AI);
  init_paddle(&paddle1, 1, lvl_current, mode == GAME_MODE_1_PLAYER || mode == GAME_MODE_AI);
  init_bonus(&bonus);
  score_init();

  missile_init_all();

  //
  txt_bank_init(txt_lens);
  txt_init(&txt_youwin, 0);
  txt_init(&txt_youlose, 0);
  txt_init(&txt_ready, 0);
  txt_init(&txt_pause, 0);

  txt_set_string(&txt_youwin, "you win");
  txt_set_string(&txt_youlose, "you lose");
  txt_set_string(&txt_ready, "ready");
  txt_set_string(&txt_pause, "paused");

  txt_show(&txt_ready, 68, 80);
  get_ready_timer = 60;

  game_paused = 0;

  DISPLAY_ON;
  init_sound();
}

void loop_level()
{
  UINT8 r;

  while (1)
  {
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;	
    
    r = control(
      &paddle0,
      &paddle1,
      game_mode,
      game_paused
    );
    
    if (r == CONTROL_STOP)
    {
      break;
    }
    else if (r == CONTROL_PAUSE)
    {
      game_paused = 1;
      txt_show_p(&txt_pause, 64, 80, bkg_is_light() ? 0 : S_PALETTE);
    }
    else if (r == CONTROL_RESUME)
    {
      txt_hide(&txt_pause); // TODO: why needed? the following txt_show should be enough.
      txt_show_p(&txt_ready, 68, 80, bkg_is_light() ? 0 : S_PALETTE);
      get_ready_timer = 60;
    }
    else
    {
      if (lk_is_connected() != LK_SLAVE)
      {
        update(
          &paddle0,
          &paddle1,
          &bonus
        );

        if (score_has_winner())
        {
          bkg_update(lvl_current); // Make sure to leave the dark mode.
          ball_reset_all(); // Make sure to hide all the balls.
          break;
        }
      }

      play_sound();
    }
    
    if (lk_is_connected() != LK_SLAVE)
    {
		  wait_vbl_done();
    }
    else
    {
      lk_wait_sio_done(); // slave only
    }
  }
}

UINT8 loop_end_level()
{
  UINT8 r;
  UINT8 win;
  UINT8 winner;

  winner = score_has_winner();

  for (r=0; r<150; r++)
  {
    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;	

    if (game_mode == GAME_MODE_1_PLAYER && winner == 1)
    {
      win = 1;
    }
    else if (game_mode == GAME_MODE_2_PLAYERS)
    {
      win = 1;
    }
    else if (game_mode == GAME_MODE_LINK && winner == 1)
    {
      win = 1;
    }
    /* should not happen
    else if (game_mode == GAME_MODE_AI)
    {
      win = 1;
    }
    */
    else
    {
      win = 0;
    }

    if (win)
    {
      txt_show(&txt_youwin, 60, 80);
    }
    else
    {
      txt_show(&txt_youlose, 56, 80);
    }

    ball_set_position(BALL_MAIN, 0, ((SCREENHEIGHT/2U)+SPR_YOFFSET)-(TILE_HEIGHT/2U));

    control(
      &paddle0,
      &paddle1,
      game_mode,
      0
    );

    missile_update_all(); // level 1 only
    score_update();

    play_sound();
    
	  wait_vbl_done();
  }

  if (win)
  {
    txt_hide(&txt_youwin);
  }
  else
  {
    txt_hide(&txt_youlose);
  }

  return win && (lvl_current < 2);/* && (game_mode != GAME_MODE_AI); */
}

void loop_next_level()
{
  UINT8 i;
  UINT8 winner;

  winner = score_has_winner();

  if (game_mode == GAME_MODE_1_PLAYER && winner == 1) // Why doing this?
  {

    i = 0;
    do{

      control(
        &paddle0,
        &paddle1,
        game_mode,
        0
      );

      // TODO x out of screen as well
      set_paddle_position(&paddle1, 200); // out of screen

      i++;
      bkg_level_transition(lvl_current, lvl_current+1, i);
      move_bkg(i, 0);  
      play_sound();      
      wait_vbl_done();

    } while(i);

    score_init(); // TODO level_init()?
    //paddle_reset(&paddle0, 0);
    paddle_reset(&paddle1, lvl_current+1);
  }
}

void loop_game()
{
  UINT8 next;

	while (1)
  {
    loop_level();
    if (game_mode == GAME_MODE_AI)
    {
      break;
    }

    next = loop_end_level();
    if (next)
    {
      loop_next_level();
      lvl_current++;
	  }
    else
    {
      break;
    }
  }

  stop_sound();
}

struct paddle_t *get_paddle(UINT8 id)
{
  if (id == 0)
  {
    return &paddle0;
  }
  else if (id == 1)
  {
    return &paddle1;
  }
  else
  {
    return &paddle0;
  }
}
