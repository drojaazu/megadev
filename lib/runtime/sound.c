/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sound.c
 * @brief Main CPU side of the Sub CPU sound commands (main/sound.h)
 */

#include <main/echo.h>
#include <main/gate_arr.def.h>
#include <main/sound.h>

#define STAT (*(uint16_t volatile const *) (GA_MAIN_REG_COMSTAT0 + 2 * SOUND_COMM))
#define ACK  (*(uint16_t volatile *) (GA_MAIN_REG_COMCMD0 + 2 * SOUND_COMM))

static void const * const * bgm_table;
static void const * const * sfx_table;
static uint8_t              bgm_entries;
static uint8_t              sfx_entries;

void sound_set_tables(void const * const * bgm, uint8_t bgm_count,
  void const * const * sfx, uint8_t sfx_count)
{
  bgm_table   = bgm;
  bgm_entries = bgm_count;
  sfx_table   = sfx;
  sfx_entries = sfx_count;
}

void sound_forward(void)
{
  uint16_t const word = STAT;
  if (word == ACK)
    return;

  uint8_t const arg = (uint8_t) word;
  switch ((word >> 8) & 0x7F) {
    case SOUND_OP_PLAY_BGM:
      if (arg < bgm_entries)
        echo_play_bgm(bgm_table[arg]);
      break;
    case SOUND_OP_STOP_BGM:
      echo_stop_bgm();
      break;
    case SOUND_OP_PAUSE_BGM:
      echo_pause_bgm();
      break;
    case SOUND_OP_RESUME_BGM:
      echo_resume_bgm();
      break;
    case SOUND_OP_PLAY_SFX:
      if (arg < sfx_entries)
        echo_play_sfx(sfx_table[arg]);
      break;
    case SOUND_OP_STOP_SFX:
      echo_stop_sfx();
      break;
    case SOUND_OP_SET_VOLUME:
      echo_set_volume(arg);
      break;
  }
  ACK = word;
}
