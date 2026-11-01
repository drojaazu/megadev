#include "ipx.h"
#include "shared.h"
#include <main/bios.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <system.h>
#include <types.h>
#include <main/text.h>

// font8x8 is loaded at tile 0x20, so a character's tile is its code
#define TEXT_TILE 0x20

extern u8      next_module;
extern u8      res_snow_chr;
extern u16     res_snow_chr_sz;
extern Palette res_snow_pal;

void main()
{
  disable_interrupts();
  bios_load_pal_update(&res_snow_pal);
  bios_dma_xfer_word_ram(
    to_vdp_addr(vram_addr_from_tileidx(0x80)), &res_snow_chr, res_snow_chr_sz >> 1);
  enable_interrupts();

  text_print("Module 2", VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, 1, 1, Width64), TEXT_TILE);

  init_particles(0x81, 0x82, 0, 0, 0, 0, 45, 1, 2, 1);

  do
  {
    bios_vblank_wait_default();
    process_particles();
  } while (! (bios_joy1_hit & PAD_START));

  next_module = FILE_EX3_MMD;
  return;
}
