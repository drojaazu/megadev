/**
 * Bit positions of the bitfields that the VDP or the Boot ROM reads.
 *
 * offsetof cannot reach a bitfield, so each entry below sets one field to all
 * ones and leaves the rest zero. make check-asserts compiles this file and
 * compares the bytes of the object's .rodata with the bytes after "//=" on
 * each line, in order. The expected bytes are the hardware formats:
 *
 *  - VDP sprite attribute table entry (Sprite): ------yy yyyyyyyy,
 *    ----hhvv -lllllll, pccvhnnn nnnnnnnn, -------x xxxxxxxx
 *  - Boot ROM sprite mapping (SpriteMapping): Y offset, ----hhvv, the VDP
 *    attribute word, X offset, flipped X offset
 *  - Boot ROM entity (Entity): first word, enable flag in bit 15 and the
 *    routine table offset below it
 */

#include <build.def.h>
#include <main/bios.h>
#include <main/vdp.h>
#include <types.h>

union SpriteBits {
  Sprite  s;
  uint8_t b[8];
};

union MappingBits {
  struct SpriteMapping s;
  uint8_t              b[6];
};

union EntityBits {
  Entity  s;
  uint8_t b[26];
};

__attribute__((used)) static struct {
  union SpriteBits  sprite[10];
  union MappingBits mapping[7];
  union EntityBits  entity[2];
} const bits = {
  .sprite = {
    {.s.pos_y = -1},    //= 03 ff 00 00 00 00 00 00
    {.s.width = -1},    //= 00 00 0c 00 00 00 00 00
    {.s.height = -1},   //= 00 00 03 00 00 00 00 00
    {.s.next = -1},     //= 00 00 00 7f 00 00 00 00
    {.s.priority = -1}, //= 00 00 00 00 80 00 00 00
    {.s.palette = -1},  //= 00 00 00 00 60 00 00 00
    {.s.v_flip = -1},   //= 00 00 00 00 10 00 00 00
    {.s.h_flip = -1},   //= 00 00 00 00 08 00 00 00
    {.s.tile = -1},     //= 00 00 00 00 07 ff 00 00
    {.s.pos_x = -1},    //= 00 00 00 00 00 00 01 ff
  },
  .mapping = {
    {.s.width = 3},      //= 00 0c 00 00 00 00
    {.s.height = 3},     //= 00 03 00 00 00 00
    {.s.priority = 1},   //= 00 00 80 00 00 00
    {.s.pal_line = 3},   //= 00 00 60 00 00 00
    {.s.v_flip = 1},     //= 00 00 10 00 00 00
    {.s.h_flip = 1},     //= 00 00 08 00 00 00
    {.s.chridx = 0x7FF}, //= 00 00 07 ff 00 00
  },
  .entity = {
    {.s.enable = 1},             //= 80 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    {.s.jmptbl_offset = 0x7FFF}, //= 7f ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  },
};
