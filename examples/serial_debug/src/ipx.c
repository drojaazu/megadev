#include "ipx.h"
#include "shared.h"
#include <cpu.h>
#include <macro.h>
#include <main/bios.h>
#include <main/comm.h>
#include <main/crash.h>
#include <main/gate_arr.h>
#include <main/io.h>
#include <main/memmap.def.h>
#include <main/memmap.h>
#include <main/mmd.h>
#include <main/text.h>
#include <memory.h>
#include <str_util.h>
#include <string.h>
#include <types.h>

// The system font is loaded at tile 0x20, so a character's tile is its code
#define TEXT_TILE 0x20

// Module layout (Work RAM map in shared.h): the IPX runs from Work RAM and
// stays resident; the ex* modules import its symbols.
GLOBAL_SYM(MODULE_ROM_ORIGIN, IPX_ROM_ORIGIN);
GLOBAL_SYM(MODULE_ROM_LENGTH, IPX_ROM_LENGTH);
GLOBAL_SYM(MODULE_RAM_ORIGIN, IPX_RAM_ORIGIN);
GLOBAL_SYM(MODULE_RAM_LENGTH, IPX_RAM_LENGTH);
GLOBAL_SYM(MMD_DEST, IPX_ROM_ORIGIN);

#define print_xy(x, y) VDP_PLANE_XY(BIOS_VDP_DEFAULT_PLANEA, x, y, Width64)

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
  for (u8 iter = 0; iter < 16; ++iter) {

    if (particles[iter].status == Null) {
      init_particle(iter);
      continue;
    }

    if (particles[iter].status == Ending) {
      // ending animation
      if (particles[iter].timer > 0) {
        --particles[iter].timer;
        if (particles[iter].timer == 0) {
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

    if (particles[iter].status == Falling) {
      if (particles[iter].pos_y >= particles[iter].end_at) {
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

void vint_user()
{
  bios_copy_sprlist();
}

char tx_buffer[32];

void transmit()
{
  char * this_char = tx_buffer;
  while (*this_char)
    ext_tx((u8) *this_char++);
  memset8(0, tx_buffer, 32);
}

char   rx_buffer[32];
char * rx_buffer_at;

__attribute__((interrupt)) void INT2_EXT()
{
  // Read the control register before RxData. RRDY is set when a byte is
  // waiting; RERR flags a framing/overrun error, in which case the byte is
  // discarded rather than echoed into the command buffer.
  u8 const status = ext_status();
  if (! (status & SCTRL_RX_READY_MASK) || (status & SCTRL_RX_ERR_MASK))
    return;

  char const c = (char) ext_read_byte();

  ext_tx((u8) c); // echo
  if (rx_buffer_at >= ((char *) rx_buffer + 32)) {
    memset8(0, rx_buffer, 32);
    rx_buffer_at = rx_buffer;
  }
  *rx_buffer_at++ = c;

  if (c == '\n') {
    ext_tx('\r');

    if (strcmpt(rx_buffer, "peek", ' ')) {
      strcpy(tx_buffer, "got a peek");
      transmit();
    } else if (strcmpt(rx_buffer, "poke", ' ')) {
      strcpy(tx_buffer, "got a poke");
      transmit();
    } else {
      strcpy(tx_buffer, "invalid");
      transmit();
    }
    memset8(0, rx_buffer, 32);
  }
}

// At this point, the full IPX binary has been copied to Work RAM and all
// traces of the security code and tiny IP are gone. We can now get on with
// actually useful game code
void main()
{
  init_ext_port();
  *bios_vblank_user = vint_user;
  // crash handler for Main CPU exceptions (docs/crash_handler.md)
  crash_install();

  *((volatile void *(*) ) EXVEC_LEVEL2) = INT2_EXT;
  // enable INT2 interrupts
  vdp_reg const vdp_mode3[] = {VDP_REG_MODE3 | VDP_EXTINT_ENABLE, 0};
  bios_load_vdp_regs(vdp_mode3);

  enable_interrupts();
  vdp_load_1bpp_tiles(sysfont, SYSFONT_CHARS, TEXT_TILE, 1, 0);
  bios_palette[1] = 0xEEE;
  bios_vdp_update_flags |= BIOS_MASK_COPY_PALETTE;

  text_print("Serial Debugging", print_xy(1, 1), TEXT_TILE);

  // Announce ourselves on the serial link. Without this nothing is transmitted
  // until a command arrives, which makes the outbound path impossible to
  // observe on its own.
  strcpy(tx_buffer, "megadev serial debug ready\r\n");
  transmit();

  do {
    ga_main_grant_2m();
    crash_check_sub();
  } while (1);
}
