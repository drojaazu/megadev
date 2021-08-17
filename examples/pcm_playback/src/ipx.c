#include "ipx.h"
#include "io_def.h"
#include "main/bootlib.h"
#include "main/main.h"
#include "mmd_exec.h"
#include "system.h"
#include "vdp.h"

u8 pcm_file;

void play_pcm() {
  *GA_COMCMD0 = 0x10;

  boot_print("Playing...\xff",
             (VDPPTR(NMT_POS_PLANE(1, 2, BOOT_PLANEA_ADDR)) | VRAM_W));

  // wait for the playing flag to clear
  while (*GA_COMFLAGS_SUB & 0x80)
    ;

  while (*GA_COMSTAT0 == 0)
    ;
  *GA_COMCMD0 = 0;
  while (*GA_COMSTAT0 != 0)
    ;

  boot_print("Done      \xff",
             (VDPPTR(NMT_POS_PLANE(1, 2, BOOT_PLANEA_ADDR)) | VRAM_W));
}

void load_pcm() {
  boot_print("Loading...\xff",
             (VDPPTR(NMT_POS_PLANE(1, 2, BOOT_PLANEA_ADDR)) | VRAM_W));
  *GA_COMCMD0 = 2;
  while (*GA_COMSTAT0 == 0)
    ;
  *GA_COMCMD0 = 0;
  while (*GA_COMSTAT0 != 0)
    ;

  play_pcm();
}

void main() {
  // basic setup stuff we've covered elsewhere
  *MLEVEL6_VECTOR = (void *(*))BOOT_VINT;
  enable_interrupts();
  boot_load_font_defaults();
  PALETTE[1] = 0xeee;
  *VDP_UPDATE_FLAGS |= PAL_UPDATE_MSK;

  boot_print("PCM Audio Playback\xff",
             (VDPPTR(NMT_POS_PLANE(1, 1, BOOT_PLANEA_ADDR)) | VRAM_W));

  load_pcm();

  boot_print("Press A to replay\xff",
             (VDPPTR(NMT_POS_PLANE(1, 4, BOOT_PLANEA_ADDR)) | VRAM_W));

  // main loop
  while (true) {
    boot_vint_wait_default();

    if ((*JOY1_PRESS & PAD_A_MSK)) {
      play_pcm();
    }
  }
}
