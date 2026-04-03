#include "ipx.h"
#include "shared.h"
#include <main/bios.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>

extern u8      next_module;
extern u8      res_bubbles_chr;
extern u16     res_bubbles_chr_sz;
extern Palette res_bubbles_pal;

void main()
{
  disable_interrupts();
  bios_load_pal_update(&res_bubbles_pal);
  bios_dma_xfer_word_ram(
    vdp_ptr(VRAMPTR(0x80)), &res_bubbles_chr, res_bubbles_chr_sz >> 1);
  enable_interrupts();

  bios_print(
    "Module 3\xff",
    (vdp_ptr(PLANE_POS(1, 1, Width64) + BIOS_VDP_DEFAULT_PLANEA) | VRAM_W));

  init_particles(0x81, 0x85, 1, 1, 1, 1, 3, 1, 3, 1);

  do
  {
    bios_vblank_wait_default();
    process_particles();
  } while (! (bios_joy1_hit & PAD_START));

  next_module = FILE_EX1_MMD;
  return;
}
