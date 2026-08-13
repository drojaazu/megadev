/**
 * [ M E G A D E V ]
 *
 * Compile-time assertions for array-typed memory-mapped regions.
 *
 * These macros are the pattern (*((T(*)[N]) ADDR)), which yields an lvalue of
 * array type -- so indexing works AND sizeof() reports the region size. Two of
 * them were previously written as a cast to array type ((T[N]) ADDR), which is
 * illegal C and failed the moment the macro was expanded. Because nothing
 * expanded them, the per-header compile never noticed.
 *
 * Expanding them here is the whole point.
 */

#include <main/bios.h>
#include <main/io.def.h>
#include <main/io.h>
#include <types.h>

/* --- Boot ROM work buffer ------------------------------------------------ */

_Static_assert(sizeof(bios_work_buffer) == 0x200,
	"bios_work_buffer must be 0x200 bytes");
_Static_assert(BIOS_WORK_BUFFER_LEN == 0x200,
	"BIOS_WORK_BUFFER_LEN must agree with the buffer it describes");
_Static_assert(sizeof(bios_work_buffer[0]) == 1, "work buffer is byte-addressed");

/* --- Boot ROM sprite list ----------------------------------------------- */

_Static_assert(sizeof(bios_sprlist) == 80 * sizeof(Sprite),
	"sprite cache holds 80 sprites");

/* --- Cartridge /TIME region --------------------------------------------- */

_Static_assert(sizeof(time_mapping) == 0x100, "/TIME region is 0x100 bytes");
_Static_assert(sizeof(time_mapping[0]) == 1, "/TIME region is byte-addressed");

/* Addresses are what the hardware documentation says. */
_Static_assert(TIME_MAPPING == 0xA13000, "/TIME base address");
