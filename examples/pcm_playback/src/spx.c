
#include "sub/cdrom.h"
#include "sub/pcm.h"
#include "sub/sub.h"

void pcm_playback(u8 * pcm_data, u32 pcm_data_size);

const PcmChannelSettings pcmSettings = {0xff, 0xff, 0x6b, 0x5, 0, 0, 0};

void load_ipx();

extern void sp_fatal();

__attribute__((section(".init"))) void main() {

  register u16 cmd0;

  do {

    do {
      cmd0 = *GA_COMCMD0;
    } while (cmd0 == 0);

    if (cmd0 != *GA_COMCMD0)
      continue;

    switch (cmd0) {

    case 2:
      load_file(ACC_OP_LOAD_CDC, "AUDIO.PCM;1", (u8 *)PRG_RAM2);
      if (access_op_result != RESULT_OK) {
        sp_fatal();
      }
      break;

    case 0x10:
      clear_pcm_ram_c();

      config_pcm_channels_c(CHANNEL(1), &pcmSettings);

      // we use the highest bit of the comflags to inform main that the
      // audio is playing
      // (be careful about setting low bits on COMFLAGS when using the
      // default VINT handler in the Main Boot ROM Library; see the
      // Predefined Comm Flag Semantics section of bootrom.md for more
      // info)
      *GA_COMFLAGS_SUB |= 0x80;
      // pcm_playback((u8 *)PRG_RAM2, 0x39bc1);
      pcm_playback((u8 *)PRG_RAM2, 0x40000);
      *((volatile u8 *)_PCM_CDISABLE) = 0xff;

      *GA_COMFLAGS_SUB &= ~0x80;
      break;

    // load IPX
    case 0xfe:
      load_file(ACC_OP_LOAD_CDC, "IPX.MMD;1", (u8 *)SUB_2M_BASE);
      grant_2m();
      if (access_op_result != RESULT_OK) {
        sp_fatal();
      }
      break;
    }

    *GA_COMSTAT0 = *GA_COMCMD0;
    do {
      cmd0 = *GA_COMCMD0;
    } while (cmd0 != 0);

    do {
      cmd0 = *GA_COMCMD0;
    } while (cmd0 != 0);

    *GA_COMSTAT0 = 0;

  } while (1);
}

bool pcmPutUpper = false;
/**
 * This is a very simple playback driver.
 * All PCM data must be already loaded into the buffer
 * Audio is played on channel 1
 *
 * The data is divided into 32kb blocks, with any remainder
 * not played
 * For this reason, the file should be padded out to the next
 * 32kb boundary to have the full clip played
 *
 * (I said it was simple. It can certainly be expanded to support
 * oddly sized files and streaming from disc with some further
 * modification.)
 */
void pcm_playback(u8 * pcm_data, u32 pcm_data_size) {

  // 32kb blocks
  u8 pcmram_blocks = pcm_data_size / 0x8000;

  // wave bank select
  // put initial block of data into lower bank
  u8 wb_select = 0x80;
  for (u8 copyIter = 0; copyIter < 8; ++copyIter) {
    *PCM_CTRL = wb_select;
    u8 * pcm_ram = (u8 *)(_PCM_RAM + 1);
    for (u16 addr = 0; addr < 0x1000; ++addr) {
      *pcm_ram = *pcm_data++;
      pcm_ram += 2;
    }
    ++wb_select;
  }
  pcmPutUpper = true;

  // begin playback on lower bank
  *PCM_CDISABLE = 0b11111110;

  // begin looping over the rest of the data, loading from the upper
  // wave ram block
  for (u16 block_iter = 0; block_iter < (pcmram_blocks - 1); ++block_iter) {
    if (pcmPutUpper)
      wb_select = 0x88;
    else
      wb_select = 0x80;

    for (u8 copyIter = 0; copyIter < 8; ++copyIter) {
      *PCM_CTRL = wb_select;
      u8 * pcm_ram = (u8 *)(_PCM_RAM + 1);
      for (u16 addr = 0; addr < 0x1000; ++addr) {
        *pcm_ram = *pcm_data++;
        pcm_ram += 2;
      }
      ++wb_select;
    }

    if (pcmPutUpper) {
      while (*((volatile u8 *)_PCM_PLAY_CH1_H) <= 0x7f) {
        asm("nop");
        asm("nop");
      }
    } else {
      while (*((volatile u8 *)_PCM_PLAY_CH1_H) > 0x7f) {
        asm("nop");
        asm("nop");
      }
    }
    pcmPutUpper = !pcmPutUpper;
  }

  // wait to finish playback before returning
  if (pcmPutUpper) {
    while (*((volatile u8 *)_PCM_PLAY_CH1_H) <= 0x7f) {
      asm("nop");
      asm("nop");
    }
  } else {
    while (*((volatile u8 *)_PCM_PLAY_CH1_H) > 0x7f) {
      asm("nop");
      asm("nop");
    }
  }
}
