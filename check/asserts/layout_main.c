/**
 * Compile-time assertions for the layout of the Main CPU structures that
 * something other than the compiler reads: the Boot ROM, the VDP, or the
 * kit's assembly.
 *
 * The expected offsets come from that reader (the CRASH_* offsets the crash
 * handlers use, the Boot ROM's parameter blocks), not from a compiler, so a
 * compiler or header change that moves a field fails here. Bitfield
 * positions, which offsetof cannot reach, are in bitfields.c.
 */

#include <build.def.h>
#include <main/bios.h>
#include <main/crash.h>
#include <main/vdp.h>
#include <types.h>

#define ASSERT_AT(type, field, off) \
  _Static_assert(__builtin_offsetof(type, field) == (off), #type "." #field " at " #off)

#define ASSERT_SIZE(type, size) _Static_assert(sizeof(type) == (size), #type " is " #size " bytes")

/* Crash snapshot: written by crash_main.s and crash_sub.s at the CRASH_* offsets */
ASSERT_AT(CrashState, vector, CRASH_VECTOR);
ASSERT_AT(CrashState, sr, CRASH_SR);
ASSERT_AT(CrashState, pc, CRASH_PC);
ASSERT_AT(CrashState, access_addr, CRASH_ACCESS_ADDR);
ASSERT_AT(CrashState, access_info, CRASH_ACCESS_INFO);
ASSERT_AT(CrashState, ir, CRASH_IR);
ASSERT_AT(CrashState, d, CRASH_D);
ASSERT_AT(CrashState, a, CRASH_A);
ASSERT_AT(CrashState, stack, CRASH_STACK);
ASSERT_SIZE(CrashState, CRASH_SIZE);

/* VDP sprite attribute table entry: four words */
ASSERT_SIZE(Sprite, 8);
ASSERT_SIZE(SpriteEx, 8);

/* Boot ROM sprite mapping: Y offset, size, attribute word, X offset, flipped X offset */
ASSERT_AT(struct SpriteMapping, relpos_y, 0);
ASSERT_AT(struct SpriteMapping, relpos_x, 4);
ASSERT_AT(struct SpriteMapping, mirrored_relpos_x, 5);
ASSERT_SIZE(struct SpriteMapping, 6);

/* Boot ROM sprite layout: count, a byte copied to Entity offset 0x19, the mappings */
ASSERT_AT(SpriteLayout, mapping_count, 0);
ASSERT_AT(SpriteLayout, unknown, 1);
ASSERT_AT(SpriteLayout, mappings, 2);
ASSERT_SIZE(SpriteLayout, 2);

/* Boot ROM entity */
ASSERT_AT(Entity, display_flags, 2);
ASSERT_AT(Entity, user, 3);
ASSERT_AT(Entity, layout, 4);
ASSERT_AT(Entity, pos_x, 8);
ASSERT_AT(Entity, pos_y, 12);
ASSERT_AT(Entity, move_x, 16);
ASSERT_AT(Entity, move_y, 20);
ASSERT_AT(Entity, sprite_flags, 24);
ASSERT_AT(Entity, unknown, 0x19);
ASSERT_SIZE(Entity, 26);

/* Boot ROM palette: CRAM index, count minus 1, the colors */
ASSERT_AT(Palette, cram_offset, 0);
ASSERT_AT(Palette, length, 1);
ASSERT_AT(Palette, colors, 2);
ASSERT_SIZE(Palette, 2);

/* Boot ROM DMA transfer block: length in words, VDP command, source */
ASSERT_AT(DmaTransfer, length, 0);
ASSERT_AT(DmaTransfer, vdp_addr, 2);
ASSERT_AT(DmaTransfer, source, 6);
ASSERT_SIZE(DmaTransfer, 10);
