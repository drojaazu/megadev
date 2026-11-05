# Changelog

## 2.0.0 (unreleased)

2.0 renames much of the hardware definition layer. Projects written against
1.x need changes; `docs/migration-2.0.md` lists every rename with its
replacement.

### Added

- **Mode 1 support**: drive the Mega CD from a cartridge. Set
  `TARGET:=MEGACD_MODE1`. `main/mode1.h` detects the Mega CD, unpacks the Sub
  CPU BIOS from the Boot ROM, starts the Sub CPU with your program and builds
  the system vector table. Tested in BlastEm with 18 Boot ROM versions. See
  `docs/mode1.md` and the `mode1` example.
- **C library subset**: `string.h`, `stdlib.h` (`abs`, `div`, `atoi`,
  `strtol`, `rand`, `qsort`, `bsearch` and others), `stdio.h` (`snprintf`,
  `vsnprintf`, integer conversions only), `ctype.h`, and `assert.h`, which
  reports failures over the serial port. Only the functions a program uses
  are linked.
- **Compiler runtime**: `memcpy`, `memmove`, `memset`, `memcmp`, and 32-bit
  multiply, divide and modulo helpers, which GCC calls on its own. Linked into
  every program automatically.
- **Crash handler** (`main/exvec.h`, `sub/exvec.h`): on an exception, shows
  the CPU state on screen and over the serial port. Main CPU on cartridges,
  in Mode 1 and on the Mega CD; Sub CPU crashes are reported by the Main CPU.
  `EXVEC_FONT` selects the kit's font or the Boot ROM's. See
  `docs/crash_handler.md`.
  Every example and the `new_project` template install it.
- **Debug text without the Boot ROM** (`main/text.h`): `vdp_load_1bpp_tiles`
  loads a 1bpp font, `text_print` prints a C string at a `VDP_PLANE_XY`
  position. Includes `font8x8`, a public domain 8x8 ASCII font, for the Mega
  Drive alone. `str_to_bios` converts a C string for `bios_print`.
  The examples print with it: C strings, `snprintf` and `font8x8`.
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
