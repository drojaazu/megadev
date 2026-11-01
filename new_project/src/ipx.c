#include "shared.h"
#include <macro.h>

// Module layout (Work RAM map in shared.h): code at the start of Work RAM,
// overwriting the IP, variables at the top below the Boot ROM's work area.
// MMD_DEST says where the module is copied after loading into Word RAM.
GLOBAL_SYM(MODULE_ROM_ORIGIN, IPX_ROM_ORIGIN);
GLOBAL_SYM(MODULE_ROM_LENGTH, IPX_ROM_LENGTH);
GLOBAL_SYM(MODULE_RAM_ORIGIN, IPX_RAM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, IPX_RAM_LENGTH);
GLOBAL_SYM(MMD_DEST, IPX_ROM_ORIGIN);

#include "ipx.h"
#include "shared.h"
#include <main/bios.h>
#include <main/exvec.h>
#include <main/gate_arr.h>
#include <main/io.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <system.h>
#include <types.h>
#include <main/text.h>

// font8x8 is loaded at tile 0x20, so a character's tile is its code
#define TEXT_TILE 0x20

u8 next_module;

Particle particles[16];

InitSettings settings;

void init_particle(u8 particle_idx)
{
  particles[particle_idx].status = Falling;
  particles[particle_idx].pos_x = bios_prng_mod(320) + 128;
  particles[particle_idx].pos_y = bios_prng_mod(5) + 123;
  particles[particle_idx].speed =
    bios_prng_mod((settings.max_speed - settings.min_speed) + 1) +
    settings.min_speed;
  particles[particle_idx].end_at = bios_prng_mod(11) + 320;
  particles[particle_idx].timer = 0;

  bios_sprlist[particle_idx].next = particle_idx + 1;
  bios_sprlist[particle_idx].pos_y = 128;
  bios_sprlist[particle_idx].pos_x = particles[particle_idx].pos_x;
  bios_sprlist[particle_idx].tile = settings.main_tile;
  bios_sprlist[particle_idx].palette = settings.palette;
  bios_sprlist[particle_idx].width = settings.main_width;
  bios_sprlist[particle_idx].height = settings.main_height;
}

void init_particles(
  u16 main_tile,
  u16 end_tile,
  u8  main_width,
  u8  main_height,
  u8  end_width,
  u8  end_height,
  u16 end_countdown,
  u8  min_speed,
  u8  max_speed,
  u8  palette)
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

    if (particles[iter].status == Null)
    {
      init_particle(iter);
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

      bios_sprlist[iter].tile = settings.end_tile;
      bios_sprlist[iter].width = settings.main_width;
      bios_sprlist[iter].height = settings.main_height;
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
    bios_sprlist[iter].pos_x = particles[iter].pos_x;
    bios_sprlist[iter].pos_y = particles[iter].pos_y;
  }
}

void vblank_user()
{
  bios_copy_sprlist();
  // show a Sub CPU crash while a module is running
  exvec_check_sub();
}

// At this point, the full IPX binary has been copied to Work RAM and all
// traces of the security code and tiny IP are gone. We can now get on with
// actually useful game code
void main()
{

  /*
    The function pointer stored in bios_vblank_user is called by the Boot ROM's
    VBLANK handler (BIOS_VBLANK_HANDLER, which we set up in the IP) when
    BIOS_MASK_DO_USERCALL is set in bios_vblank_handler_flags. The handler
    clears the flags every VBLANK and bios_vblank_wait_default sets them again,
    so it runs on frames where the program waits with that call, as the
    modules do.
  */
  *bios_vblank_user = vblank_user;

  // crash handler for Main CPU exceptions (docs/crash_handler.md)
  exvec_install();

  next_module = FILE_EX1_MMD;

  // don't forget, we disabled interrupts earlier in the IPX (ipx_init.s)
  // but be sure we've re-pointed the the vblank handler vector
  enable_interrupts();

  // The modules only show some text, so we'll prepare the font for them here
  // so it doesn't need to happen in the module itself
  vdp_load_1bpp_tiles(font8x8, FONT8X8_CHARS, TEXT_TILE, 1, 0);

  // The font uses palette entry #1, so we'll manually set that to white
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;

  do
  {
    // make sure that the Sub CPU controls 2M Word RAM before we request the
    // file
    ga_main_grant_2m();

    // In this example, we have the command for the Sub CPU stored in COMCMD0
    // and the command argument in COMCMD1. Command 1 will be "load a file"
    // and the argument will be the ID for that file, which is defined in
    // the SPX
    // Set the argument first
    ga_main_reg_comcmd1 = next_module;

    // then set the command
    ga_main_reg_comcmd0 = CMD_LOAD_FILE;

    // wait for acknowledgment from the Sub CPU that the command was
    // received and will be acted on
    do
    {
      // the Sub CPU may crash while working on the command
      exvec_check_sub();
    } while (ga_main_reg_comstat0 == 0);

    // reset the command to none (0) once we have the acknowledgment
    ga_main_reg_comcmd0 = 0;

    // the Sub CPU side work will be complete when COMSTAT0 returns to 0
    do
    {
      exvec_check_sub();
    } while (ga_main_reg_comstat0 != 0);

    ga_main_wait_2m();

    // Sub CPU side work is complete and the MMD should now be in 2M Word RAM
    // init_mmd() sets things up based on the MMD header then returns a pointer
    // to main() in the module, which we'll call as a function pointer
    void (*mmd_main)() = init_mmd();
    asm("call_mmd:");
    mmd_main();

    // module has exited and next_module was updated
    // Clear the tiles on the screen and prepare the next iteration
    bios_clear_tables();

  } while (1);
}
