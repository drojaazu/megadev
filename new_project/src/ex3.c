#include "ipx.h"
#include "shared.h"
#include "shortcuts.h"
#include <cpu.h>
#include <main/bios.h>
#include <main/io.def.h>
#include <main/memmap.h>
#include <main/text.h>
#include <main/vdp.h>
#include <types.h>

// The system font is loaded at tile 0x20, so a character's tile is its code
#define TEXT_TILE 0x20

extern u8      next_module;
extern u8      res_bubbles_chr;
extern u16     res_bubbles_chr_sz;
extern Palette res_bubbles_pal;

void main()
{
  disable_interrupts();
  bios_load_pal_update(&res_bubbles_pal);
  bios_dma_xfer_word_ram(
    to_vdp_addr(vram_addr_from_tileidx(0x80)),
    &res_bubbles_chr,
    res_bubbles_chr_sz >> 1);
  enable_interrupts();

  text_print(
    "Module 3",
    VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, 1, 1, Width64),
    TEXT_TILE);

  init_particles(0x81, 0x85, 1, 1, 1, 1, 3, 1, 3, 1);

  do {
    bios_vblank_wait_default();
    process_particles();
  } while (! (bios_joy1_hit & PAD_START));

  next_module = FILE_EX1_MMD;
  return;
}
