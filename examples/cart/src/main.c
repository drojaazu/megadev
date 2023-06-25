#include "main/bootlib.h"
#include "main/io.def.h"
#include "main/io.h"
#include "main/memmap.h"
#include "main/vdp.h"
#include "memory.h"
#include "string.h"
#include "system.h"

extern u8 res_sysfont_1bpp_chr;
extern u8 res_rain_chr;
extern u16 res_rain_chr_sz;
extern Palette res_rain_pal;

u16 test1;

enum Status
{
	Falling = 1,
	Ending = 2,
	Null = 0
};

typedef struct Particle
{
	u8 status;
	u8 speed;
	s16 pos_x;
	s16 pos_y;
	s16 end_at;
	u16 timer;
} Particle;

typedef struct InitSettings
{
	u16 main_tile;
	u16 end_tile;
	u16 end_countdown;
	u8 main_width;
	u8 main_height;
	u8 end_width;
	u8 end_height;
	u8 min_speed;
	u8 max_speed;
	u8 palette;
} InitSettings;

Particle particles[16];

InitSettings settings;

void init_particle(u8 particle_idx)
{
	particles[particle_idx].status = Falling;
	particles[particle_idx].pos_x = blib_prng_mod(320) + 128;
	// particles[particle_idx].pos_x = 84 + 128;
	particles[particle_idx].pos_y = blib_prng_mod(5) + 123;
	// particles[particle_idx].pos_y = 3 + 123;
	particles[particle_idx].speed = blib_prng_mod((settings.max_speed - settings.min_speed) + 1) + settings.min_speed;
	// particles[particle_idx].speed = 4;
	particles[particle_idx].end_at = blib_prng_mod(11) + 320;
	// particles[particle_idx].end_at = 8 + 320;
	particles[particle_idx].timer = 0;

	BLIB_SPRLIST[particle_idx].next = particle_idx + 1;
	BLIB_SPRLIST[particle_idx].pos_y = 128;
	BLIB_SPRLIST[particle_idx].pos_x = particles[particle_idx].pos_x;
	BLIB_SPRLIST[particle_idx].tile = settings.main_tile;
	BLIB_SPRLIST[particle_idx].palette = settings.palette;
	BLIB_SPRLIST[particle_idx].width = settings.main_width;
	BLIB_SPRLIST[particle_idx].height = settings.main_height;
}

void init_particles(u16 main_tile,
	u16 end_tile,
	u8 main_width,
	u8 main_height,
	u8 end_width,
	u8 end_height,
	u16 end_countdown,
	u8 min_speed,
	u8 max_speed,
	u8 palette)
{

	settings.main_tile = main_tile;
	settings.end_tile = end_tile;
	settings.main_width = main_width;
	settings.main_height = main_height;
	settings.end_width = end_width;
	settings.end_height = end_height;
	settings.end_countdown = end_countdown;
	settings.min_speed = min_speed;
	settings.max_speed = max_speed;
	settings.palette = palette;

	for (u8 iter = 0; iter < 16; ++iter)
		init_particle(iter);
}

void process_particles()
{
	for (u8 iter = 0; iter < 16; ++iter)
	{
		// u16 v = ((iter + 0x30) << 8) | 0x00ff;
		//  blib_print((char *) &v, (VDPPTR(NMT_POS_PLANE(2, 10, _BLIB_PLANEA_ADDR)) | VRAM_W));
		//  blib_vint_wait(0x80);

		if (particles[iter].status == Null)
		{
			// blib_print((char *) &v, (VDPPTR(NMT_POS_PLANE(2, 11, _BLIB_PLANEA_ADDR)) | VRAM_W));
			// blib_vint_wait(0x80);
			init_particle(iter);
			continue;
		}

		if (particles[iter].status == Ending)
		{
			// ending animation
			// blib_print((char *) &v, (VDPPTR(NMT_POS_PLANE(2, 12, _BLIB_PLANEA_ADDR)) | VRAM_W));
			// blib_vint_wait(0x80);
			if (particles[iter].timer > 0)
			{
				--particles[iter].timer;
				if (particles[iter].timer == 0)
				{
					particles[iter].status = Null;
				}
				// goto update_sprites;
				continue;
			}

			BLIB_SPRLIST[iter].tile = settings.end_tile;
			BLIB_SPRLIST[iter].width = settings.main_width;
			BLIB_SPRLIST[iter].height = settings.main_height;
			particles[iter].timer = settings.end_countdown;
			goto update_sprites;
		}

		if (particles[iter].status == Falling)
		{
			// blib_print((char *) &v, (VDPPTR(NMT_POS_PLANE(2, 13, _BLIB_PLANEA_ADDR)) | VRAM_W));
			// blib_vint_wait(0x80);
			if (particles[iter].pos_y >= particles[iter].end_at)
			{
				// reached its final destination
				particles[iter].status = Ending;
				continue;
			}
			particles[iter].pos_y += particles[iter].speed;
		}

	update_sprites:
		BLIB_SPRLIST[iter].pos_x = particles[iter].pos_x;
		BLIB_SPRLIST[iter].pos_y = particles[iter].pos_y;
	}
}

__attribute__((interrupt)) void _INT_VBLANK()
{
	// blib_vint_handler();
	blib_copy_pal();
	blib_copy_sprlist();
	blib_prng();
	blib_update_inputs();
	BLIB_VINT_FLAGS = 0;
}

char rx_buffer[16];
char * rx_buffer_at;

u32 to_atoi(char * p_c)
{
	u32 out = 0;
	while ((*p_c != '\n') && (*p_c != ' '))
	{
		char c = *p_c;
		if (c > 0x60)
			c -= 0x56;
		else if (c > 0x40)
			c -= 0x36;
		else
			c -= 0x30;
		out <<= 4;
		out |= c;
		++p_c;
	}
	return out;
}

__attribute__((interrupt)) void _INT_EXT()
{
	blib_print("INT2 last cmd:\xff", (VDPPTR(NMT_POS_PLANE(1, 8, _BLIB_PLANEA_ADDR)) | VRAM_W));

	char c = IO_RXDATA2;
	if (rx_buffer_at >= rx_buffer + 16)
	{
		memset8(0, rx_buffer, 16);
		rx_buffer_at = rx_buffer;
	}
	*rx_buffer_at++ = c;

	if (c == '\n')
	{

		char cmd = *rx_buffer;
		switch (cmd)
		{
			case 'g':
				blib_print("get\xff", (VDPPTR(NMT_POS_PLANE(16, 8, _BLIB_PLANEA_ADDR)) | VRAM_W));
				if (rx_buffer[1] != ' ')
					break;
				rx_buffer_at = rx_buffer + 2;

				u32 addr = atoi(rx_buffer_at);
				// check for a second space
				u16 length;
				while ((*rx_buffer_at != ' ') && (*rx_buffer_at != '\0'))
					++rx_buffer_at;
				if (*rx_buffer_at == '\0')
				{
					length = 1;
				}
				else
				{
					++rx_buffer_at;
					length = atoi(rx_buffer_at);
				}

				while (length > 0)
				{
					do
					{
						asm("nop");
					} while ((IO_SCTRL2 & SCTRL_TFUL_MSK) != 0);
					IO_TXDATA2 = *((u8 *) addr++);
					--length;
				}
				break;
			case 's':
				blib_print("set\xff", (VDPPTR(NMT_POS_PLANE(16, 8, _BLIB_PLANEA_ADDR)) | VRAM_W));
				if (rx_buffer[1] != ' ')
					break;
				break;
			case '\n':
			default:
				break;
		}
		rx_buffer_at = rx_buffer;
		memset8(0, rx_buffer, 16);
	}
}

char info[17];

void main()
{

	disable_interrupts();

	memset8(0, rx_buffer, 16);
	rx_buffer_at = rx_buffer;

	// repoint the VINT vector to the boot rom library version
	// MLEVEL6_VECTOR = (void *(*) ) _BLIB_VINT_HANDLER;
	//*BLIB_VINT_EX_PTR = vint_ex;

	// note: seems to be important to set up vdpregs first, then clear vram
	blib_load_vdpregs_default();
	blib_clear_vram();

	BLIB_FONT_TILE_BASE = 0;
	// blib_load_1bpp_tiles(&res_sysfont_1bpp_chr, 0x60, VDPPTR(VRAM_AT(0x20)) | VRAM_W, 0x00011011);
	blib_load_1bpp_tiles((void *) 0x40b000, 0x60, VDPPTR(VRAM_AT(0x20)) | VRAM_W, 0x00011011);

	// initialize serial comm on port 2
	// 4800bps, serial in/out mode, ext interrupt enable
	IO_SCTRL2 = B4800_MSK | SCTRL_SIN_MSK | SCTRL_SOUT_MSK | SCTRL_RINT_MSK;
	IO_CTRL2 = 0x7f;
	BLIB_VDPREGS[0x0b] |= 0x08;
	VDP_CTRL_16 = BLIB_VDPREGS[0x0b];

	// The modules only show some text, so we'll prepare the font for them here
	// so it doesn't need to happen in the module itself
	// blib_load_font_defaults();

	blib_dma_xfer(VDPPTR(VRAM_AT(0x80)) | VRAM_W, &res_rain_chr, res_rain_chr_sz >> 1);

	// The font uses palette entry #1, so we'll manually set that to white
	BLIB_PALETTE[0] = 0x000;
	BLIB_PALETTE[1] = 0xeee;

	blib_load_pal(&res_rain_pal);

	BLIB_VDP_UPDATE_FLAGS |= PAL_UPDATE_MSK;
	blib_copy_pal();

	init_particles(0x81, 0x82, 0, 0, 0, 0, 3, 3, 5, 1);

	enable_interrupts();
	blib_vdp_disp_enable();

	memset8(0, info, 16);
	info[16] = '\xff';

	blib_print("Megadev Mode 1 testing\xff", (VDPPTR(NMT_POS_PLANE(1, 1, _BLIB_PLANEA_ADDR)) | VRAM_W));
	blib_print("Mega CD rev: \xff", (VDPPTR(NMT_POS_PLANE(1, 3, _BLIB_PLANEA_ADDR)) | VRAM_W));

	char * i = (char *) 0x400120;
	for (int c = 0; c < 16; ++c)
		info[c] = *i++;
	// memcpy8((u8 *) 0x400120, (u8 *) info, 15);
	blib_print(info, (VDPPTR(NMT_POS_PLANE(1, 4, _BLIB_PLANEA_ADDR)) | VRAM_W));
	// memcpy8((char *) 0x400130, info, 15);
	i = (char *) 0x400130;
	for (int c = 0; c < 16; ++c)
		info[c] = *i++;
	blib_print(info, (VDPPTR(NMT_POS_PLANE(1, 5, _BLIB_PLANEA_ADDR)) | VRAM_W));
	// memcpy8((char *) 0x400140, info, 15);
	i = (char *) 0x400140;
	for (int c = 0; c < 16; ++c)
		info[c] = *i++;
	blib_print(info, (VDPPTR(NMT_POS_PLANE(1, 6, _BLIB_PLANEA_ADDR)) | VRAM_W));

	do
	{
		asm("nop");
	} while ((IO_SCTRL2 & SCTRL_TFUL_MSK) != 0);
	IO_TXDATA2 = 'o';
	do
	{
		asm("nop");
	} while ((IO_SCTRL2 & SCTRL_TFUL_MSK) != 0);
	IO_TXDATA2 = 'k';
	do
	{
		asm("nop");
	} while ((IO_SCTRL2 & SCTRL_TFUL_MSK) != 0);
	IO_TXDATA2 = '1';
	do
	{
		asm("nop");
	} while ((IO_SCTRL2 & SCTRL_TFUL_MSK) != 0);
	IO_TXDATA2 = '3';

	do
	{
		blib_vint_wait(0x81);
		process_particles();
	} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	blib_print("BIG TEST\xff", (VDPPTR(NMT_POS_PLANE(2, 10, _BLIB_PLANEA_ADDR)) | VRAM_W));
}
