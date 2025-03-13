#include <main/io.h>
#include <main/memmap.h>
#include <main/vdp.h>
#include <memory.h>
#include <string.h>
#include <system.h>
#include "res.h"

volatile u8 p1_hold, p1_single, p1_prev;
volatile u8 p2_hold, p2_single, p2_prev;

void update_inputs()
{
	p1_hold = ~read_input_joypad(IO_DATA1);
	p2_hold = ~read_input_joypad(IO_DATA2);
	if (p1_hold == p1_prev)
		p1_single = 0;
	else
		p1_single = p1_hold;
	if(p2_hold == p2_prev)
		p2_single = 0;
	else
		p2_single = p2_hold;
	p1_prev = p1_hold;
	p2_prev = p2_hold;
}

__attribute__((interrupt)) void INT2_EXT()
{
	return;
}

__attribute__((interrupt)) void INT4_HBLANK()
{
	return;
}


volatile bool vblank_done;
u32 vblank_counter;

__attribute__((interrupt)) void INT6_VBLANK()
{
	update_inputs();
	++vblank_counter;
	vblank_done = true;
	return;
}

// copy of VDP registers in memory
// these need to be updated to the VDP whenever they are updated
u16 vdp_regs[18];

void update_vdp_regs()
{
	for (u8 i = 0; i < 18; ++ i)
		VDP_CTRL = vdp_regs[i];
}

// copy of color RAM in memory
u16 cram[64];

void update_cram_line(u8 pal_line)
{
	pal_line <<= 4;
	VDP_CTRL_32 = VDPPTR(pal_line) | CRAM_W;
	u16 * ptr_pal_line = cram + pal_line;
	for (u8 i =0; i < 16; ++ i)
		VDP_DATA = * ptr_pal_line ++;
}

void update_cram()
{
	VDP_CTRL_32 = VDPPTR(0) | CRAM_W;
	u16 * ptr_pal_line = cram;
	for (u8 i = 0; i < 64; ++ i)
		VDP_DATA = * ptr_pal_line ++;
}

void clear_vram()
{
	for(u16 i = 0; i < (0x10000 / 4); ++i)
		VDP_DATA_32 = 0;
}

#define PLANE_A_ADDR 0x2000
#define PLANE_B_ADDR 0x4000

u16 const default_vdp_regs[] = {
	0x8004, // mode register 1
	0x8164, // Mode Register 2 (Display enable, VBLANK interrupt enable)
	0x8208, // plane a table location - VRAM:$2000
	0x8318, // window table location -  VRAM:$3000
	0x8406, // plane b table location - VRAM:$4000
	0x8500, // sprite table location (reg 5) 2*$200 = $400
	0x8600, // sprite pattern generator base addr.
	0x8700, // backgroud colour,  (reg 7)
	0x8800, // 0
	0x8900, // 0
	0x8b00, // Mode register 3
	0x8c00, // mode register 4
	0x8d05, // HBL_scroll data location. ($1400)
	0x8e00, // 0
	0x8f02, // auto-increment value
	0x9000, // plane size
	0x9100, // window plane h-pos
	0x9200, // window place v-pos
};

#define plane_xy(x, y) (VDPPTR(NMT_POS_PLANE_32(x, y, PLANE_A_ADDR)) | VRAM_W)

void print(char const * string, VDP_ADDR pos)
{
	VDP_CTRL_32 = pos;
	while(*string != 0)
		VDP_DATA = *string++;
}

u8 counter;

void main()
{

	disable_interrupts();
	vblank_done = false;
	vblank_counter = 0;
	memcpy16(default_vdp_regs, vdp_regs, 18);
	update_vdp_regs();

	// note: seems to be important to set up vdpregs first, then clear vram
	clear_vram();

	cram[0] = 0x0000;
	cram[1] = 0x0eee;
	cram[2] = 0x000e;
	cram[3] = 0x00e0;
	cram[4] = 0x0e00;
	update_cram();

	init_joypads();
	vdp_regs[1] |= VDP_DMA_ENABLE;
	VDP_CTRL = vdp_regs[1];
	vdp_dma_transfer(res_basic_font, VDPPTR(VRAM_AT(0x20)), (res_basic_font_size << 1));
	vdp_regs[1] &= ~VDP_DMA_ENABLE;
	VDP_CTRL = vdp_regs[1];

	enable_interrupts();

	counter = 0;
	char const * test = "TESTING!";
	print(test, plane_xy(1,1));
	__asm__("main_loop:");
	while(1)
	{
		while (!vblank_done)
			asm("nop");
		vblank_done = false;

		if(p1_single & PAD_A)
			++counter;
		
	}
}
