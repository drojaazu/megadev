#ifndef MEGADEV__IPXSPX_EXAMPLE_IPX_H
#define MEGADEV__IPXSPX_EXAMPLE_IPX_H

#include "types.h"

enum Status {
	Falling = 1,
	Ending = 2,
	Null = 0
};

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
	u8 main_tile_size;
	u8 end_tile_size;
	u16 end_countdown;
	u8 min_speed;
	u8 max_speed;
	u8 palette;
} InitSettings;

void init_particle(u8 particle_idx);

void init_particles(u16 main_tile, u16 end_tile, u8 main_tile_size, u8 end_tile_size,
		u16 end_countdown, u8 min_speed, u8 max_speed, u8 palette);

void process_particles();

#endif