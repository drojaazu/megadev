#include "ipx.h"
#include "bridge.h"
#include "ipx.res.h"
#include <main/bios.h>
#include <main/gate_arr.h>
#include <main/io.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <system.h>
#include <types.h>

void vint_user()
{
	bios_copy_pal();
	bios_copy_sprlist();
}

vdp_cmd tiles = vdp_ptr(VRAMPTR(1)) | VRAM_W;

__attribute__((noreturn)) void main()
{

	*bios_vint_user = vint_user;

	enable_interrupts();

	bios_load_font_defaults();
	bios_palette[1] = 0xEEE;
	bios_vdp_update_flags |= BIOS_VDPUPDATE_COPY_PALETTE_FLAG;
	bios_load_pal(&res_pal_stamp01);

	do
	{
		bios_vint_wait_default();
		if (bios_joy1_hit & PAD_START)
		{
			grant_2m();
			*gareg_comcmd0 = CMD_REDRAW;

			do
			{
				// the NOP is so GCC doesn't optimize the loop away
				// though since comstat is marked volatile it should be fine...
				asm("nop");
			} while (*GA_COMSTAT0 == 0);

			*gareg_comcmd0 = 0;

			// the Sub CPU side work will be complete when COMSTAT0 returns to 0
			do
			{
				asm("nop");
			} while (*GA_COMSTAT0 != 0);

			wait_2m();

			bios_vint_wait_default();
			bios_vint_wait_default();
			bios_vint_wait_default();
			bios_vint_wait_default();
			bios_dma_xfer_word_ram(
				vdp_ptr(VRAMPTR(1)) | VRAM_W,
				(void const *) (WORD_RAM + 0x30000),
				(32 * 32 * 32) / 2);

			bios_load_stamp_tilemap(
				31, 31, vdp_ptr((BIOS_VDP_DEFAULT_PLANEA)) | VRAM_W, 1);
		}
	} while (1);
}
