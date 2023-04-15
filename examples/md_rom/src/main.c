#include "main/bootlib.h"
#include "main/io.def.h"
#include "main/memmap.h"
#include "main/vdp.h"
#include "system.h"

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

void vint_ex()
{
	blib_copy_sprlist();
}

void main()
{

	disable_interrupts();
	// repoint the VINT vector to the boot rom library version
	// MLEVEL6_VECTOR = (void *(*) ) _BLIB_VINT_HANDLER;
	//*BLIB_VINT_EX_PTR = vint_ex;

	blib_load_vdpregs_default();

	// The modules only show some text, so we'll prepare the font for them here
	// so it doesn't need to happen in the module itself
	blib_load_font_defaults();

	blib_dma_xfer(VDPPTR(VRAM_AT(0x80)) | VRAM_W, &res_rain_chr, res_rain_chr_sz >> 1);
	blib_load_pal(&res_rain_pal);

	// The font uses palette entry #1, so we'll manually set that to white
	BLIB_PALETTE[1] = 0xe42;
	BLIB_PALETTE[2] = 0xea8;
	BLIB_PALETTE[16] = 0x000;
	BLIB_PALETTE[17] = 0xEee;
	BLIB_PALETTE[18] = 0xE0e;
	BLIB_VDP_UPDATE_FLAGS |= PAL_UPDATE_MSK;

	// init_particles is defined in the ipx
	init_particles(0x81, 0x82, 0, 0, 0, 0, 3, 3, 5, 0);

	enable_interrupts();
	blib_vdp_disp_enable();

	blib_print("Rainin'\xff", (VDPPTR(NMT_POS_PLANE(2, 2, _BLIB_PLANEA_ADDR)) | VRAM_W));

	do
	{
		blib_vint_wait(0x81);
		process_particles();
	} while (! (BLIB_JOY1_PRESS & PAD_START_MSK));

	blib_print("BIG TEST\xff", (VDPPTR(NMT_POS_PLANE(2, 10, _BLIB_PLANEA_ADDR)) | VRAM_W));
}
