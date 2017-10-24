
#include "types.h"
#include "main/boot.h"
#include "vdp.h"
#include "io_def.h"
#include "main/main.h"
#include "system.h"

extern u8 program_mode;
extern u8 res_bubbles_chr;
extern u16 res_bubbles_chr_sz;
extern u8 res_bubbles_pal;

void main() {
	disable_interrupts();
	boot_load_pal_update(&res_bubbles_pal);
	boot_dma_xfer_wordram(VDPPTR(AT_TILE(0x80)), &res_bubbles_chr, res_bubbles_chr_sz >> 1);
	enable_interrupts();

	boot_print("Module 3\xff", 
		(VDPPTR(
			NMT_POS_PLANE(1,1,BOOT_PLANE_WIDTH,BOOT_PLANEA_ADDR))
			 | VRAM_W));

	init_particles(0x81, 0x85, 5, 5, 3, 1, 3, 1);

	do {
		boot_vint_wait_default();
		process_particles();
	} while(!(*JOY1_PRESS & PAD_START_MSK));

	program_mode = 0;
	return;
}
