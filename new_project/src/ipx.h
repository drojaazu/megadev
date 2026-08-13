#ifndef MEGADEV__IPXSPX_EXAMPLE_IPX_H
#define MEGADEV__IPXSPX_EXAMPLE_IPX_H

#include "ipx_api.def.h"
#include <types.h>

enum Status { Falling = 1, Ending = 2, Null = 0 };

typedef struct Particle {
  u8 status;
  u8 speed;
  s16 pos_x;
  s16 pos_y;
  s16 end_at;
  u16 timer;
} Particle;

typedef struct InitSettings {
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

/*
 * The IPX itself defines these routines; every other module calls them through
 * the jump table at a fixed address, so a module built today keeps working if
 * the IPX is rebuilt. Entry numbers come from IPX_API_LIST.
 *
 * The IPX defines IPX_IMPLEMENTATION before including this header, so it sees
 * ordinary prototypes rather than macros that would rewrite its own function
 * definitions.
 */

void init_particle(u8 particle_idx);

#ifdef IPX_IMPLEMENTATION

void init_particles(u16 main_tile, u16 end_tile, u8 main_width, u8 main_height,
                    u8 end_width, u8 end_height, u16 end_countdown,
                    u8 min_speed, u8 max_speed, u8 palette);

void process_particles(void);

#else

typedef void (*ipx_init_particles_fn)(u16 main_tile, u16 end_tile, u8 main_width,
                                      u8 main_height, u8 end_width, u8 end_height,
                                      u16 end_countdown, u8 min_speed, u8 max_speed,
                                      u8 palette);
typedef void (*ipx_process_particles_fn)(void);

#define init_particles    ((ipx_init_particles_fn) IPX_ENTRY(0))
#define process_particles ((ipx_process_particles_fn) IPX_ENTRY(1))

#endif

/** @brief Shared with transient modules; lives in the .shared block */
#define next_module (*(volatile u8 *) IPX_SHARED)

#endif