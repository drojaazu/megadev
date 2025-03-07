#include <main/io.def.h>
#include <main/io.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <memory.h>
#include <string.h>
#include <system.h>

extern u8 res_sysfont_1bpp_chr;


__attribute__((interrupt)) void _INT_VBLANK()
{
	return;
}

__attribute__((interrupt)) void _INT_EXT()
{
	return;
}

u16 const default_vdp_regs[] = {
	0x8004,		// mode register 1
	0x8144,		// mode register 2 - Display enable bit set ($40) + VBI ($20)
	0x8208,		// plane a table location - VRAM:$2000
	0x8318,		// window table location -  VRAM:$3000
	0x8406,		// plane b table location - VRAM:$4000
	0x8500,		// sprite table location (reg 5) 2*$200 = $400
	0x8600,		// sprite pattern generator base addr.
	0x8700,		// backgroud colour,  (reg 7)
	0x8800,		// 0
	0x8900,		// 0
	0x8b00,		// Mode register 3
	0x8c00,		// mode register 4
	0x8d05,		// HBL_scroll data location. ($1400)
	0x8e00,		// 0
	0x8f02,		// auto-increment value
	0x9000,		// plane size
	0x9100,		// window plane h-pos
	0x9200,		// window place v-pos
};

void main()
{

	disable_interrupts();

	// note: seems to be important to set up vdpregs first, then clear vram
	for (u8 i =0; i < 18; ++ i)
		VDP_CTRL = default_vdp_regs[i];
	VDP_CTRL_32 = VDPPTR(0) | VRAM_W;
	
	for(u16 i = 0; i < (0x10000 / 4); ++i)
	{
		VDP_DATA_32 = 0;
	}

	VDP_CTRL_32 = VDPPTR(0)| CRAM_W;
	VDP_DATA = 0x0000;
	VDP_DATA = 0x000f;
	VDP_DATA = 0x00f0;
	VDP_DATA = 0x0f00;
	VDP_DATA = 0x00ff;
	VDP_DATA = 0x0f0f;
	VDP_DATA = 0x00ff;
	VDP_DATA = 0x0ff0;
	VDP_DATA = 0x0ff0;
	VDP_DATA = 0x0f0f;
	VDP_DATA = 0x0fff;

	while(1)
	{
		asm("nop");
	}
}
