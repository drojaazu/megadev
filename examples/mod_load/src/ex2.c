
#include "types.h"
#include "main/boot.h"
#include "vdp.h"
#include "io_def.h"
#include "main/main.h"

extern u16 global_mode;

void main() {
	boot_print("Example file Number Two!\xff", 
		(to_vdpptr(
			NMT_POS_PLANE(9, 6, BOOT_PLANE_WIDTH, BOOT_PLANEA_ADDR))
			 | VRAM_W));

	do {
		boot_vint_wait_default();
	} while(!(*JOY1_PRESS & PAD_START_MSK));

	global_mode = 2;
	return;
}

