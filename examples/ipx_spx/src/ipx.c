
#include "ipx.h"
#include "main/bootlib.h"
#include "main/main.h"
#include "mmd_exec.h"
#include "system.h"
#include "vdp.h"

u8 global_mode;

Particle particles[16];

InitSettings settings;

void init_particle (u8 particle_idx)
{
	particles[particle_idx].status = Falling;
	particles[particle_idx].pos_x = boot_prng_mod (320) + 128;
	particles[particle_idx].pos_y = boot_prng_mod (5) + 123;
	particles[particle_idx].speed = boot_prng_mod ((settings.max_speed - settings.min_speed) + 1) + settings.min_speed;
	particles[particle_idx].end_at = boot_prng_mod (11) + 320;
	particles[particle_idx].timer = 0;

	SPRITE_LIST[particle_idx].next = particle_idx + 1;
	SPRITE_LIST[particle_idx].pos_y = 128;
	SPRITE_LIST[particle_idx].pos_x = particles[particle_idx].pos_x;
	SPRITE_LIST[particle_idx].tile = settings.main_tile;
	SPRITE_LIST[particle_idx].palette = settings.palette;
	SPRITE_LIST[particle_idx].width = settings.main_width;
	SPRITE_LIST[particle_idx].height = settings.main_height;
}

void init_particles (u16 main_tile,
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
		init_particle (iter);
}

void process_particles()
{
	for (u8 iter = 0; iter < 16; ++iter)
	{

		if (particles[iter].status == Null)
		{
			init_particle (iter);
			continue;
		}

		if (particles[iter].status == Ending)
		{
			// ending animation
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

			SPRITE_LIST[iter].tile = settings.end_tile;
			SPRITE_LIST[iter].width = settings.main_width;
			SPRITE_LIST[iter].height = settings.main_height;
			particles[iter].timer = settings.end_countdown;
			goto update_sprites;
		}

		if (particles[iter].status == Falling)
		{
			if (particles[iter].pos_y >= particles[iter].end_at)
			{
				// reached its final destination
				particles[iter].status = Ending;
				continue;
			}
			particles[iter].pos_y += particles[iter].speed;
		}

	update_sprites:
		SPRITE_LIST[iter].pos_x = particles[iter].pos_x;
		SPRITE_LIST[iter].pos_y = particles[iter].pos_y;
	}
}

void vint_ex()
{
	boot_copy_sprlist();
}

// At this point, the full IPX binary has been copies to Work RAM and all
// traces of the security code and tiny IP are gone. We can now get on with
// actually useful game code
void main()
{

	// repoint the VINT vector to the boot rom library version
	MLEVEL6_VECTOR = (void *(*) ) _BLIB_VINT_HANDLER;
	*VINT_EX_PTR = vint_ex;

	// don't forget, we disabled interrupts earlier in the IPX (ipx_init.s)
	// but be sure we've re-pointed the the vblank handler vector
	enable_interrupts();

	// this will hold the current "mode" of the entire program
	// in this case it corresponds to the three modules on the disc
	global_mode = 0;

	// The modules only show some text, so we'll prepare the font for them here
	// so it doesn't need to happen in the module itself
	boot_load_font_defaults();

	// The font uses palette entry #1, so we'll manually set that to white
	PALETTE[1] = 0xeee;
	*VDP_UPDATE_FLAGS |= PAL_UPDATE_MSK;

	do
	{
		// make sure that the Sub CPU controls 2M Word RAM before we request the
		// file
		grant_2m();

		// In this example, we have the command for the Sub CPU stored in COMCMD0
		// and the command argument in COMCMD1. Command 1 will be "load a file"
		// and the argument will be the ID for that file, which is defined in
		// the SPX
		// Set the argument first
		*GA_COMCMD1 = global_mode;

		// then set the command
		*GA_COMCMD0 = 1;

		// wait for acknowledgment from the Sub CPU that the command was
		// received and will be acted on
		do
		{
			// the NOP is so GCC doesn't optimize the loop away
			// though since comstat is marked volatile it should be fine...
			asm("nop");
		} while (*GA_COMSTAT0 == 0);

		// reset the command to none (0) once we have the acknowledgment
		*GA_COMCMD0 = 0;

		// the Sub CPU side work will be complete when COMSTAT0 returns to 0
		do
		{
			asm("nop");
		} while (*GA_COMSTAT0 != 0);

		// Sub CPU side work is complete and the MMD should now be in 2M Word RAM
		// Run it!
		mmd_exec();

		// module has exited and program_mode was updated
		// Clear the tiles on the screen and prepare the next iteration
		boot_clear_nmtbl();

	} while (1);
}
