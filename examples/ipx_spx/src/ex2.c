
#include "io_def.h"
#include "ipx.h"
#include "main/bootlib.h"
#include "main/main.h"
#include "system.h"
#include "types.h"
#include "vdp.h"

extern u8 global_mode;
extern u8 res_snow_chr;
extern u16 res_snow_chr_sz;
extern Palette res_snow_pal;

void main() {
  disable_interrupts();
  boot_load_pal_update(&res_snow_pal);
  boot_dma_xfer_wordram(VDPPTR(AT_TILE(0x80)), &res_snow_chr,
                        res_snow_chr_sz >> 1);
  enable_interrupts();

  boot_print("Module 2\xff",
             (VDPPTR(NMT_POS_PLANE(1, 1, BOOT_PLANEA_ADDR)) | VRAM_W));

  init_particles(0x81, 0x82, 0, 0, 0, 0, 45, 1, 2, 1);

  do {
    boot_vint_wait_default();
    process_particles();
  } while (!(*JOY1_PRESS & PAD_START_MSK));

  global_mode = 2;
  return;
}
