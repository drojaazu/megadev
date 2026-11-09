# Changelog

## 2.0.0 (unreleased)

2.0 renames much of the hardware definition layer. Projects written against
1.x need changes; `docs/migration-2.0.md` lists every rename with its
replacement.

### Added

- **Partial loads and streaming**: the CD-ROM access loop loads part of a
  file, `part_count` sectors from `part_offset`, into any destination
  (`load_file_part`). The `pcm_playback` example now streams its audio from
  the disc straight into the PCM chip's wave RAM by DMA, one 4 KB bank at a
  time while the other half plays, instead of staging 256 KB in Program RAM.
  The `mode1` example also loads its file into Program RAM by DMA.
- **Mode 1 support**: drive the Mega CD from a cartridge. Set
  `TARGET:=MEGACD_MODE1`. `main/mode1.h` detects the Mega CD, unpacks the Sub
  CPU BIOS from the Boot ROM, starts the Sub CPU with your program and builds
  the system vector table. The Sub CPU reads the disc with the usual BIOS
  calls and access loop; the `mode1` example loads a file into Word RAM by CPU
  copy and by DMA and checks it. Tested in BlastEm with 22 Boot ROM versions.
  See `docs/mode1.md` and the `mode1` example.
- **`ISO_BOOT`**: the boot sector image the ISO is mastered with. Defaults to
  the kit's boot sector for `MEGACD` and to none for `MEGACD_MODE1`, whose
  disc is read by the cartridge rather than booted.
- **C library subset**: `string.h`, `stdlib.h` (`abs`, `div`, `atoi`,
  `strtol`, `rand`, `qsort`, `bsearch` and others), `stdio.h` (`snprintf`,
  `vsnprintf`, integer conversions only), `ctype.h`, and `assert.h`, which
  reports failures over the serial port. Only the functions a program uses
  are linked.
- **Compiler runtime**: `memcpy`, `memmove`, `memset`, `memcmp`, and 32-bit
  multiply, divide and modulo helpers, which GCC calls on its own. Linked into
  every program automatically.
- **Crash handler** (`main/crash.h`, `sub/crash.h`): on an exception, shows
  the CPU state on screen and over the serial port. Main CPU on cartridges,
  in Mode 1 and on the Mega CD; Sub CPU crashes are reported by the Main CPU.
  `CRASH_FONT` selects the kit's font or the Boot ROM's. See
  `docs/crash_handler.md`.
  Every example and the `new_project` template install it.
- **Debug text without the Boot ROM** (`main/text.h`): `vdp_load_1bpp_tiles`
  loads a 1bpp font, `text_print` prints a C string at a `VDP_PLANE_XY`
  position. Includes the system font (`sysfont`), a public domain 8x8 ASCII font, for the Mega
  Drive alone. `str_to_bios` converts a C string for `bios_print`.
  The examples print with it: C strings, `snprintf` and the system font.
- **Cartridge header**: the build writes the ROM end address and checksum into
  the header of every cartridge (`tools/romfix.sh`).
- **Layout symbols**: every linker script defines the same symbols describing
  the program's layout (`_ROM_ORIGIN`, `_BSS_LENGTH` and others), listed in
  `docs/dev_in_c.md`.
- **Stack placement for modules that never return**: `std_init_stack.s` and
  `main/ipx_init_stack.s` move the stack to `STACK_TOP` before `main`. The
  linker rejects a stack that overlaps the module or the exception table.
- **Serial port selection**: `EXT_PORT` and `EXT_BAUD` choose the port and
  speed for the serial routines.
- **Header dependency tracking**: editing a header rebuilds what uses it.
- Examples: `ground_plane` (rotation and scaling hardware), `serial_debug`,
  `mode1`.
- Documentation: `gate_array.md`, `rotation_scaling.md`, `mode1.md`,
  `migration-2.0.md`.
- Headers can be included from C++ (`extern "C"` guards). C++ is not
  supported.
- `NOTICE`: the Mega CD security block is Sega's, shipped unmodified, and
  excluded from the MIT license.

### Changed

- Reference documentation (`make docs`): every header member is documented
  and Doxygen reports no warnings, down from about 1200. Assembly sources are
  no longer Doxygen input (Doxygen cannot parse them); their routines are
  documented through their C wrappers, and `docs/asm_api.md` gives the
  assembly caller's view: registers in, registers out, registers clobbered,
  and the macros. Implementation files (`lib/runtime`, the security block)
  are not input either.
- Gate array names carry their CPU side: `GA_MAIN_*` / `GA_SUB_*`,
  `ga_main_*` / `ga_sub_*`. A program can include both sides, as Mode 1
  requires.
- Register fields follow one scheme: `<SUBSYSTEM>_<FIELD>_POS`, `_MASK` and,
  for multi-bit fields, `_WIDTH`, with `FIELD_PREP` and `FIELD_GET` helpers.
  Registers the hardware splits into bytes have byte names.
- Register accessors are lvalues: `ga_main_reg_comcmd0 = x`.
- Sub CPU system calls have descriptive names (`BIOS_AUDIO_PLAY_ONCE`,
  `BRAM_SEARCH`, ...). Sega's names remain as aliases. `sub/cdboot.def.h` is
  now `sub/boot.def.h`.
- Assembly macro files end in `.macro.s`.
- `types.h` defines only the standard names: `size_t`, `ptrdiff_t`, `bool`
  and the `intN_t` family. The `u8`/`s8` to `u32`/`s32` shorthand and
  `sptr`/`uptr` are gone from the kit; the examples define the shorthand they
  use in their own `src/shortcuts.h`.
- Module layouts can be declared in C with `GLOBAL_SYM` (`macro.h`).
  `GLOBAL_SYM` and the assembly `GLOBAL` macro make their symbols hidden.
- Resident modules: a module links against a resident module's exported
  symbols. A function or variable defined in both now fails the link with
  `multiple definition`; layout symbols, linker script symbols, `main`,
  `init`, `hblank` and `vblank` are not exported. Keep a resident module's
  global private with `static` or hidden visibility.
- Every module must link startup code (`std_init.s` or `main/ipx_init.s`);
  otherwise it fails with `module has no init code`. Define
  `MODULE_NO_INIT` to opt out.
- Static constructors and destructors (C++ global objects,
  `__attribute__((constructor))`) fail to link instead of being silently
  skipped.
- Cartridge linker symbols use the common names (`_data_org` is now
  `_RAM_DATA_ORIGIN`, and so on). In the IP and SP, `_TEXT_ORIGIN` and
  `_TEXT_LENGTH` cover the code only; the whole image is `_ROM_ORIGIN` and
  `_ROM_LENGTH`.
- Cartridges: the link fails if the ROM exceeds 4 MB, or if variables reach
  the stack (`INIT_SSP`, optional `STACK_SIZE`) or, in Mode 1, the system
  vector table.
- Sub CPU `EXVEC_*` (`sub/memmap.def.h`) name the jump target in each entry,
  as the Main CPU's do. The cartridge trace vector is `EX_TRACE`.
- `math.h`: the 16-bit divides are `divs16` and `divu16`, returning `Div16`.
- `build/` and `disc/` are created by the build; `make init` is no longer
  needed on a fresh clone. An unset `MEGADEV_PATH` is reported.

### Removed

- libgcc is no longer linked. 64-bit integer and floating point arithmetic
  fail at link time instead of producing code that does not run on a 68000.
- `sub/commsync.s`, `cfg/module_mmd_newwork.ld` and `cfg/module_bin.ld`,
  which did not assemble or were unused.
- `main/cd_exception.s`, replaced by the crash handler.
- `lib/sysfont.1bpp.chr`, an unused copy of the Boot ROM font, which is
  Sega's.
- The `hello_world` example. `docs/boot.md` describes the IP, and the
  `new_project` IP is the annotated reference.

### Fixed

- CD-ROM access loop: a PCM wave RAM DMA destination advanced by 2 KB per
  sector where the hardware counts 4 KB, the Sub CPU's view of the bank, so
  the second sector of a bank overwrote the first's second half.
  `DMAADDR_PCM` masked to 1 KB for the same reason.
- `find_file_c` returned NULL for a file that exists and corrupted the stack
  with an `rts` inside inlined asm; nothing in the kit had called it.
- CD-ROM access loop, found by review: discs with more than one sector of
  root directory entries (about 60 files) had a corrupt directory cache; the
  DMA path mis-predicted the sector frame at every 75th sector and at random
  (uncleared extend flag), and yielded a frame per sector so it could not keep
  up with the drive; a retry on either path rewrote the CDC destination with a
  transfer possibly in flight, spraying a sector into the destination at
  offset 0. The loop now reads 80 sectors by DMA with no retries.
- `to_vram_addr_runtime` returned its argument unchanged: a commented-out
  instruction in its inline assembly had commented out the whole routine.
  Nothing in the kit called it.
- CD-ROM access loop: DMA loads went to offset 0 of the destination. Writing
  the CDC destination resets the DMA address register, and the loop wrote the
  destination after the caller had set the address. The loop now sets the
  register itself, from `filebuff`, once per sector just before the CDC read:
  the 1.00 series Sub BIOS rewrites the destination when a read starts, so
  any earlier write is lost on those consoles. A retried DMA read resumes at
  the next sector instead of starting the file over.
- Boot ROM calls that use A6 no longer corrupt the caller's stack frame.
- `bios_detect_controller` returns the controller type from the correct
  register.
- Cartridges: initialized global variables are copied to RAM. Previously one
  produced a 16 MB ROM and the value was never set.
- Cartridges boot regardless of the order of their source files.
- IP: initialized variables are read from where they are stored.
- Example and template SPs clear only their own zeroed variables. They
  counted bytes while clearing longs, zeroing four times as much Program RAM.
- SP header: the work RAM size is correct.
- Variables from an assembly `.comm` directive are placed with the other
  zeroed variables and cleared.
- `--gc-sections` no longer removes startup code, the cartridge vector table
  or module headers.
- 32-bit `%` no longer crashes a 68000.
- `div()` and `divu()` set the remainder; `div()` divides signed.
- `int_to_*` fixed point conversions no longer always return 0.
- `memset` and `memcpy` handle lengths of 0 and over 64 KB.
- Serial status flags tested the wrong bits; `ext_tx` and `ext_rx` did not
  compile; the serial init macro called macros that do not exist.
- C `hextoa` writes the 0xFF terminator the Boot ROM print routines expect.
- Headers that defined functions or variables no longer cause duplicate
  symbols when included from two files.
- Several inline wrappers that failed to compile when used now compile.
- `cd_exception.s` links (misspelled vector name).
- `vdp.s` and `str_util.s` assemble.
- The `pcm_playback` example builds from a fresh clone.
- Example `clean` targets work.
- The `bram` and `pcm_playback` Sub CPU modules initialize their variables.
- The `bram` example prints its total size and status on the right rows, and
  the total size in full.
- Doxygen configuration: correct input paths, assembly sources included.
- `megadev.make` reports a missing `PROJECT_ID`, `TARGET`, path variable or
  library with its own message; the tab-indented `$(error)` lines made make
  say "recipe commences before first target" instead.
