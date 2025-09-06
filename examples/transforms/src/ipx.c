#include "ipx.h"
#include "bridge.h"
#include <main/bios.h>
#include <main/gate_arr.h>
#include <main/io.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <system.h>
#include <types.h>

void vint_user()
{
	bios_copy_sprlist();
}

void main()
{

	*bios_vint_user = vint_user;

	enable_interrupts();

	bios_load_font_defaults();
	bios_palette[1] = 0xeee;
	bios_vdp_update_flags |= BIOS_VDPUPDATE_COPY_PALETTE_FLAG;

	grant_2m();
	do
	{

	} while (1);
}
