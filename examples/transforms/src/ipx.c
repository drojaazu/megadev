#include "ipx.h"
#include "ipx.res.h"
#include "shared.h"
#include <main/bios.h>
#include <main/gate_arr.h>
#include <main/io.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <memory.h>
#include <system.h>
#include <types.h>
#include <xform.h>

void vblank_user()
{
  bios_copy_pal();
  bios_copy_sprlist();
}

vdp_cmd tiles = to_vdp_addr(vram_addr_from_tileidx(0x80)) | VRAM_W;

s16 trace_x = to_xform_pos(0.0), trace_y = to_xform_pos(0.0),
    trace_dx = to_xform_delta(1.0), trace_dy = to_xform_delta(0.0);
__attribute__((noreturn)) void main()
{

  *bios_vblank_user = vblank_user;
  grant_2m();

  memset8(0, (u8 *) &bios_sprlist, sizeof(bios_sprlist));

  enable_interrupts();

  bios_clear_vram();

  bios_load_font_defaults();
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;
  bios_load_pal(&res_pal_stamps);

  do
  {
    bios_vblank_wait(0);
    if (bios_joy1_hold & PAD_RIGHT)
    {
      if (bios_joy1_hold & PAD_A)
      {
        trace_dx -= to_xform_delta(0.05);
      }
      else
      {
        trace_x -= to_xform_pos(0.5);
      }
    }

    if (bios_joy1_hold & PAD_LEFT)
    {
      if (bios_joy1_hold & PAD_A)
      {
        trace_dx += to_xform_delta(0.05);
      }
      else
      {
        trace_x += to_xform_pos(0.5);
      }
    }

    if (bios_joy1_hold & PAD_UP)
    {
      if (bios_joy1_hold & PAD_A)
      {
        trace_dy += to_xform_delta(0.05);
      }
      else
      {
        trace_y += to_xform_pos(0.5);
      }
    }

    if (bios_joy1_hold & PAD_DOWN)
    {
      if (bios_joy1_hold & PAD_A)
      {
        trace_dy -= to_xform_delta(0.05);
      }
      else
      {
        trace_y -= to_xform_pos(0.5);
      }
    }

    if (bios_joy1_hit & PAD_C)
    {
      trace_x = to_xform_pos(0.0);
      trace_y = to_xform_pos(0.0);
      trace_dx = to_xform_delta(1.0);
      trace_dy = to_xform_delta(0.0);
    }

    grant_2m();
    *ga_reg_comcmd1 = trace_x;
    *ga_reg_comcmd2 = trace_y;
    *ga_reg_comcmd3 = trace_dx;
    *ga_reg_comcmd4 = trace_dy;
    *ga_reg_comcmd0 = CMD_REDRAW;

    do
    {
      // the NOP is so GCC doesn't optimize the loop away
      // though since comstat is marked volatile it should be fine...
      asm("nop");
    } while (*ga_reg_comstat0 == 0);

    *ga_reg_comcmd0 = 0;

    // the Sub CPU side work will be complete when COMSTAT0 returns to 0
    do
    {
      asm("nop");
    } while (*ga_reg_comstat0 != 0);

    wait_2m();

    bios_vblank_wait_default();
    bios_dma_xfer_word_ram(
      tiles, (void const *) (WORD_RAM + 0x30000), (32 * 32 * 32) / 2);

    bios_load_stamp_tilemap(
      31, 31, to_vdp_addr((BIOS_VDP_DEFAULT_PLANEB)) | VRAM_W, 0x80);
  } while (1);
}
