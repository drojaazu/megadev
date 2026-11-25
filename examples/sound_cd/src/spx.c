/**
 * Sub CPU program (SPX): the game logic
 *
 * Loads the Main CPU module into Word RAM when the IP asks for it, then
 * waits for the Main CPU to start Echo. From then on it decides what plays:
 * it starts the song, and turns the buttons the Main CPU passes to it into
 * sound commands (sub/sound.h). The Sub CPU never touches the Z80 or Echo;
 * the Main CPU passes each command on at its next VBLANK.
 *
 *   A      sound effect 0 (beep)       C      stop / play the song
 *   B      sound effect 1 (chime)      Start  pause / resume the song
 */

#include "shared.h"
#include "shortcuts.h"
#include <cpu.h>
#include <macro.h>
#include <main/io.def.h>
#include <sub/bios.h>
#include <sub/cdrom.h>
#include <sub/gate_arr.h>
#include <sub/memmap.h>
#include <sub/sound.h>

/* Module layout: code from 0x10000, variables and stack below 0x20000 */
GLOBAL_SYM(MODULE_ROM_ORIGIN, 0x10000);
GLOBAL_SYM(MODULE_RAM_ORIGIN, 0x1E000);
GLOBAL_SYM(STACK_TOP, MODULE_RAM_ORIGIN);
GLOBAL_SYM(STACK_SIZE, 0x1000);
GLOBAL_SYM(MODULE_ROM_LENGTH, STACK_TOP - STACK_SIZE - MODULE_ROM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, 0x2000);

extern void sp_fatal();

static char const * const filenames[] = {"MAIN.MMD;1"};

static __attribute__((noreturn)) void play(void)
{
  u8 state = STATE_PLAYING;
  u8 held  = 0;

  sound_command(SOUND_OP_PLAY_BGM, BGM_SONG);
  ga_sub_reg_comstat1 = state;

  for (;;) {
    bios_wait_vsync();

    // buttons pressed since the last frame
    u8 const now     = (u8) ga_sub_reg_comcmd2;
    u8 const pressed = now & ~held;
    held             = now;

    if (pressed & PAD_A)
      sound_command(SOUND_OP_PLAY_SFX, SFX_BEEP);
    if (pressed & PAD_B)
      sound_command(SOUND_OP_PLAY_SFX, SFX_CHIME);
    if (pressed & PAD_C) {
      if (state == STATE_STOPPED) {
        sound_command(SOUND_OP_PLAY_BGM, BGM_SONG);
        state = STATE_PLAYING;
      } else {
        sound_command(SOUND_OP_STOP_BGM, 0);
        state = STATE_STOPPED;
      }
    }
    if (pressed & PAD_START) {
      if (state == STATE_PLAYING) {
        sound_command(SOUND_OP_PAUSE_BGM, 0);
        state = STATE_PAUSED;
      } else if (state == STATE_PAUSED) {
        sound_command(SOUND_OP_RESUME_BGM, 0);
        state = STATE_PLAYING;
      }
    }
    ga_sub_reg_comstat1 = state;
  }
}

void main()
{
  enable_interrupts();

  for (;;) {
    u16 command;
    do
      command = ga_sub_reg_comcmd0;
    while (command == 0);
    u16 const param = ga_sub_reg_comcmd1;

    switch (command) {
      case CMD_LOAD_FILE:
        // the IP has given Word RAM to the Sub CPU
        load_file(CDROM_LOAD_CDC, filenames[param], (u8 *) WORD_RAM_2M);
        ga_sub_grant_2m();
        if (access_op_result != CDROM_RESULT_OK)
          sp_fatal();
        break;
    }

    // acknowledge, and wait for the Main CPU to clear the command
    ga_sub_reg_comstat0 = command;
    while (ga_sub_reg_comcmd0 != 0)
      ;
    ga_sub_reg_comstat0 = 0;

    if (command == CMD_START)
      play();
  }
}
