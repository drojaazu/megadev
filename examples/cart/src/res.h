#ifndef RES_H
#define RES_H

#include "types.h"

typedef struct
{
  u32  size;
  char data[];
} DataChunk;

extern DataChunk const res_basic_font;
extern DataChunk const res_letters;

extern u16 const res_megadev_pal[16];

extern s16 const sintab[256];

#endif
