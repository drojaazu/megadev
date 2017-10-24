#ifndef MEGADEV__Z80_H
#define MEGADEV__Z80_H

#include "types.h"
#include "z80_def.h"

#define Z80_RAM ((volatile u8*)_Z80_RAM)
#define Z80_BUSREQ ((volatile u16*)_Z80_BUSREQ)
#define Z80_RESET ((volatile u16*)_Z80_RESET)


extern void load_z80_init_program();

extern void get_z80_bus();

extern void release_z80_bus();

extern void load_z80(u8 data[], u16 length);

#endif