/**
 * Compile-time assertions for the layout of the Sub CPU structures that
 * something other than the compiler reads: the Boot ROM, the PCM chip, or the
 * kit's assembly.
 *
 * The expected offsets come from that reader (cdrom.s, the PCM register
 * order, the Boot ROM's parameter blocks), not from a compiler, so a compiler
 * or header change that moves a field fails here.
 */

#include <build.def.h>
#include <sub/bios.h>
#include <sub/bram.h>
#include <sub/cdrom.h>
#include <sub/pcm.h>
#include <types.h>

#define ASSERT_AT(type, field, off) \
  _Static_assert(__builtin_offsetof(type, field) == (off), #type "." #field " at " #off)

#define ASSERT_SIZE(type, size) _Static_assert(sizeof(type) == (size), #type " is " #size " bytes")

/* File list entry: cdrom.s builds 22-byte entries, sector at 14, size at 18 */
ASSERT_AT(FileInfo, filename, 0);
ASSERT_AT(FileInfo, offset, 14);
ASSERT_AT(FileInfo, size, 18);
ASSERT_SIZE(FileInfo, 22);

/* PCM channel settings: pcm_config_channel copies them in register order */
ASSERT_AT(PcmChannelSettings, envelope, 0);
ASSERT_AT(PcmChannelSettings, panning, 1);
ASSERT_AT(PcmChannelSettings, freq_lo, 2);
ASSERT_AT(PcmChannelSettings, freq_hi, 3);
ASSERT_AT(PcmChannelSettings, loop_point_lo, 4);
ASSERT_AT(PcmChannelSettings, loop_point_hi, 5);
ASSERT_AT(PcmChannelSettings, start_point, 6);
ASSERT_SIZE(PcmChannelSettings, 7);

/* Backup RAM file parameters: 11-character name, mode byte, size in blocks */
ASSERT_AT(BramFileInfo, filename, 0);
ASSERT_AT(BramFileInfo, mode, 11);
ASSERT_AT(BramFileInfo, blocksize, 12);
ASSERT_SIZE(BramFileInfo, 14);

/* Boot ROM status block (CDBSTAT) */
ASSERT_AT(struct BiosStatus, bios_status, 0);
ASSERT_AT(struct BiosStatus, led, 2);
ASSERT_AT(struct BiosStatus, cdd_status, 4);
ASSERT_AT(struct BiosStatus, volume, 24);
ASSERT_AT(struct BiosStatus, header, 28);
ASSERT_SIZE(struct BiosStatus, 32);

/* Boot ROM read parameters: first sector, then count or last sector */
ASSERT_AT(struct RomreadParams, start_sector, 0);
ASSERT_AT(struct RomreadParams, sector_count, 4);
ASSERT_SIZE(struct RomreadParams, 8);

/* Boot ROM drive init parameters: first TOC track, last track */
ASSERT_AT(DriveInitParams, toc_track_number, 0);
ASSERT_AT(DriveInitParams, last_track, 1);
ASSERT_SIZE(DriveInitParams, 2);
