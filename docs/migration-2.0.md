# Migrating to Megadev 2.0

This guide lists every change in Megadev 2.0 that requires a change to a project written against 1.x. Each entry gives what was renamed or removed, what replaces it, and any change in behavior. Entries are added as the changes land, so this file is complete for whatever version of `develop` it ships with.

# Contents

- File names
- Gate array names
- Gate array register model
- Serial control and VDP fields
- Sub CPU system call names
- C library subset
- Module startup code
- Linker script symbols
- Resident modules

---

# File names

## Assembly macro files use the `.macro.s` suffix

Every macro-only assembly file now ends in `.macro.s`; the plural `.macros.s` is gone, and the generic `macros.s` is now `macro.s`. Update the include lines in your sources:

| Was | Now |
|---|---|
| `macros.s` | `macro.s` |
| `init.macros.s` | `init.macro.s` |
| `system.macros.s` | `system.macro.s` |
| `main/comm.macros.s` | `main/comm.macro.s` |
| `main/gate_arr.macros.s` | `main/gate_arr.macro.s` |
| `main/io.macros.s` | `main/io.macro.s` |
| `main/mmd.macros.s` | `main/mmd.macro.s` |
| `main/vdp.macros.s` | `main/vdp.macro.s` |
| `main/z80.macros.s` | `main/z80.macro.s` |

The contents of the files are unchanged.

# Gate array names

## Every gate array name carries its CPU side

The Main CPU and Sub CPU see the gate array at different addresses, and 1.x used the same names for both views, so a program that included both headers got whichever definitions came first. Every name in `main/gate_arr.*` and `sub/gate_arr.*` now carries the side immediately after the `GA`/`ga` prefix. The rule:

| Kind | 1.x | 2.0 Main CPU | 2.0 Sub CPU |
|---|---|---|---|
| Constants | `GA_REG_COMCMD0`, `GA_MASK_DMNA`, `GA_BIT_RETURN_2M` | `GA_MAIN_REG_COMCMD0`, `GA_MAIN_MASK_DMNA`, ... | `GA_SUB_REG_COMCMD0`, `GA_SUB_MASK_DMNA`, ... |
| Register pointer type | `ga_reg` | `ga_main_reg` | `ga_sub_reg` |
| Register accessors | `ga_reg_comcmd0`, `ga_reg_memmode` | `ga_main_reg_comcmd0`, `ga_main_reg_memmode` | `ga_sub_reg_comcmd0`, `ga_sub_reg_memmode` |

Apply it mechanically: insert `MAIN_` or `SUB_` after `GA_`, and `main_` or `sub_` after `ga_`, according to which CPU the source file is built for. Values and addresses are unchanged.

The inline functions and assembly macros had no prefix at all and gain the full one:

| 1.x (both sides) | 2.0 Main CPU | 2.0 Sub CPU |
|---|---|---|
| `wait_2m()` | `ga_main_wait_2m()` | `ga_sub_wait_2m()` |
| `grant_2m()` | `ga_main_grant_2m()` | `ga_sub_grant_2m()` |
| `clear_comm_regs()` | `ga_main_clear_comm_regs()` | `ga_sub_clear_comm_regs()` |
| `set_1m()`, `set_2m()` | `ga_main_set_1m()`, `ga_main_set_2m()` | (Main only) |
| `reset_ga()` | (Sub only) | `ga_sub_reset()` |
| `WAIT_2M`, `GRANT_2M`, `CLEAR_COMM_REGS` | `GA_MAIN_WAIT_2M`, `GA_MAIN_GRANT_2M`, `GA_MAIN_CLEAR_COMM_REGS` | `GA_SUB_WAIT_2M`, `GA_SUB_GRANT_2M`, `GA_SUB_CLEAR_COMM_REGS` |
| `RESET_GA` | `GA_MAIN_RESET` | (Main only) |

Names outside the `GA_`/`ga_` families in these headers (`BIT_GA_REG_*`, `MSK_GA_REG_*`, `CDCMODE_*BITS`, `MEMMODE_*_MSK`) are replaced by the register model described below.

## Register accessors are values, not pointers

Every `ga_main_reg_*` and `ga_sub_reg_*` accessor now names the register itself, so a register is read and written like a variable: `ga_main_reg_comcmd0 = 2;` and `x = ga_sub_reg_comstat1;`. In 1.x most accessors were pointers and needed a `*`. Remove the `*` at each use. Where a pointer to the register is wanted, take its address: `&ga_main_reg_comcmd0`. The generated code is identical.

# Gate array register model

Every gate array register field now follows `<SUBSYSTEM>_<FIELD>_<ASPECT>` with `_POS`, `_MASK` and, for fields wider than one bit, `_WIDTH`; see the manual, Register Field Naming. The 1.x spellings `GA_BIT_*`, `GA_MASK_*`, `BIT_GA_REG_*` and `MSK_GA_REG_*` are gone. Side prefixes are shown for the Sub CPU; the Main CPU is the same with `MAIN`.

## Flags and fields

| 1.x | 2.0 |
|---|---|
| `GA_BIT_DMNA`, `GA_MASK_DMNA`, `BIT_GA_REG_DMNA` | `GA_SUB_DMNA_POS`, `GA_SUB_DMNA_MASK` |
| `GA_BIT_RETURN_2M`, `GA_MASK_RETURN_2M`, `BIT_GA_REG_RET` | `GA_SUB_RETURN_2M_POS`, `GA_SUB_RETURN_2M_MASK` |
| `GA_MASK_WORDRAM_LAYOUT`, `BIT_GA_REG_MODE` | `GA_SUB_WORDRAM_LAYOUT_POS`, `GA_SUB_WORDRAM_LAYOUT_MASK` |
| `GA_LED_R`, `GA_LED_G` | `GA_SUB_LED_R_MASK`, `GA_SUB_LED_G_MASK` (positions 0 and 1 of `GA_SUB_REG_LED`) |
| `GA_MASK_VERSION` | `GA_SUB_VERSION_MASK` |
| `BIT_GA_REG_INTn`, `MSK_GA_REG_INTn` | `GA_SUB_INTn_POS`, `GA_SUB_INTn_MASK` |
| `GA_BIT_STAMPSIZE_*`, `GA_MASK_STAMPSIZE_*` | `GA_SUB_STAMPSIZE_*_POS`, `GA_SUB_STAMPSIZE_*_MASK` |
| `GA_BIT_CDCMODE_DSR` (6), `GA_BIT_CDCMODE_EDT` (7), `CDCMODE_CABITS`, `CDCMODE_DDBITS` | `GA_SUB_CDCMODE_DSR_POS` (14), `GA_SUB_CDCMODE_EDT_POS` (15), `GA_SUB_CDCMODE_CA_MASK`, `GA_SUB_CDCMODE_DD_MASK`. Positions are relative to the 16-bit register; use `FIELD_BYTE`/`FIELD_BPOS` for bit instructions. |
| `GA_MASK_CDCMODE_MAINREAD` and friends | `GA_SUB_CDCMODE_DD_MAINREAD` and friends: values for the DD field, placed with `FIELD_PREP(GA_SUB_CDCMODE_DD, value)` |
| Main: `GA_MASK_SUB_RESET`, `GA_MASK_SUB_REQ`, `GA_BIT_RAISE_INT2`, `GA_MASK_INT2_MASK` | `GA_MAIN_SUB_RESET_MASK`, `GA_MAIN_SUB_REQ_MASK`, `GA_MAIN_RAISE_INT2_POS`, `GA_MAIN_INT2_MASKING_MASK` |
| Main: `GA_MASK_CDC_DEST_*`, `MEMMODE_BK0_MSK`, `MEMMODE_WP0_MSK` | `GA_MAIN_CDC_DEST_*` values, `GA_MAIN_MEMMODE_BANK_MASK`, `GA_MAIN_WP_MASK` |

## Registers that split into bytes

Where a 16-bit register's two bytes hold unrelated settings, 2.0 names each byte. Positions in these registers are relative to the byte, so a `btst` takes `_POS` and the register address directly, with no `+1`.

| 1.x register | 2.0 high byte | 2.0 low byte |
|---|---|---|
| Sub `GA_REG_RESET` 0xFF8000 | `GA_SUB_REG_LED` 0xFF8000 | `GA_SUB_REG_SUBCTRL` 0xFF8001 |
| Sub `GA_REG_MEMMODE` 0xFF8002 | `GA_SUB_REG_WP` 0xFF8002 (read only) | `GA_SUB_REG_MEMMODE` 0xFF8003 |
| Sub `GA_REG_COMFLAGS` 0xFF800E | `GA_SUB_REG_COMFLAGS_MAIN` 0xFF800E (read only) | `GA_SUB_REG_COMFLAGS_SUB` 0xFF800F |
| Sub `GA_REG_STAMPSIZE` 0xFF8058 | `GA_SUB_REG_GFXSTAT` 0xFF8058 (read only, GRON) | `GA_SUB_REG_STAMPSIZE` 0xFF8059 |
| Main `GA_REG_RESET` 0xA12000 | `GA_MAIN_REG_INT2` 0xA12000 | `GA_MAIN_REG_SUBCPU` 0xA12001 |
| Main `GA_REG_MEMMODE` 0xA12002 | `GA_MAIN_REG_WP` 0xA12002 | `GA_MAIN_REG_MEMMODE` 0xA12003 |
| Main `GA_REG_COMFLAGS` 0xA1200E | `GA_MAIN_REG_COMFLAGS_MAIN` 0xA1200E | `GA_MAIN_REG_COMFLAGS_SUB` 0xA1200F (read only) |

`GA_SUB_REG_MEMMODE` and `GA_MAIN_REG_MEMMODE` keep their names but are now one byte at the odd address. Code that wrote them as a word must write the byte: `andi.b #~(GA_SUB_RETURN_2M_MASK | GA_SUB_WORDRAM_LAYOUT_MASK), GA_SUB_REG_MEMMODE`.

Registers with one unused byte are defined at the byte that is used: `GA_SUB_REG_INT3TIMER` 0xFF8031, `GA_SUB_REG_INTMASK` 0xFF8033, `GA_SUB_REG_FONTCOLOR` 0xFF804D, `GA_SUB_REG_IMGBUFVSIZE` 0xFF805D, `GA_SUB_REG_IMGBUFOFFSET` 0xFF8061, and `GA_MAIN_REG_CDCMODE` 0xA12004. Drop any `+1` from accesses to them.

## Other renames

| 1.x | 2.0 |
|---|---|
| `GA_REG_CDDCOMM0` to `GA_REG_CDDCOMM4` | `GA_SUB_REG_CDDSTAT0` to `GA_SUB_REG_CDDSTAT4` (from the drive) |
| `GA_REG_CDDCOMM5` to `GA_REG_CDDCOMM9` | `GA_SUB_REG_CDDCMD0` to `GA_SUB_REG_CDDCMD4` (to the drive) |
| `ga_reg_comflags` (word) | removed; use the two byte accessors |
| `ga_reg_cdchostdata`, `ga_reg_stopwatch` (Main), comm status (Main), comm command (Sub) | read-only accessors; a write is a compile error |
| `GA_SUB_BIT_CDC_*` | removed; the destination values are `GA_SUB_CDCMODE_DD_*` |

Use the `GA_SUB_GRON_MASK` flag of `GA_SUB_REG_GFXSTAT` to wait for a graphics operation; the stamp size register no longer carries it.

# Serial control and VDP fields

The same field scheme applies to the serial control register in `main/io.def.h` and to VDP mode register 4 in `main/vdp.def.h`.

| 1.x | 2.0 |
|---|---|
| `SCTRL_TX_FULL_BIT`, `SCTRL_TX_FULL` | `SCTRL_TX_FULL_POS`, `SCTRL_TX_FULL_MASK`; likewise `RX_READY`, `RX_ERR`, `RX_INT_ENABLE`, `SERIAL_OUT_ENABLE`, `SERIAL_IN_ENABLE` |
| `SCTRL_SERIAL_ENABLE` | `SCTRL_SERIAL_ENABLE_MASK` |
| `SCTRL_BAUD_300` and friends, pre-shifted | unshifted values for the `SCTRL_BAUD` field: write `FIELD_PREP(SCTRL_BAUD, SCTRL_BAUD_300)` |
| `VDP_MASK_WIDTH_40CELL` | `VDP_WIDTH_40CELL_MASK` |
| `VDP_MASK_SHADOW_HIGHLIGHT` | `VDP_SHADOW_HIGHLIGHT_MASK`, with `VDP_SHADOW_HIGHLIGHT_POS` |
| `VDP_MASK_INTERLACE_NONE`, `_NORMAL`, `_DOUBLE`, pre-shifted | unshifted values for the `VDP_INTERLACE` field: write `FIELD_PREP(VDP_INTERLACE, VDP_INTERLACE_DOUBLE)` |

# Sub CPU system call names

The Sub CPU BIOS, Backup RAM and CD Boot calls have descriptive names. Sega's documentation names remain available as `#define` synonyms at the end of each `.def.h` file and are shown in the reference as "Sega documentation name", so the BIOS Manual stays searchable. Code that used the Sega mnemonics (`MSCPLAY1`, `BRMINIT`, `CBTINIT`, `CDBIOS`, `BURAM`, `CDBOOT`) compiles unchanged; code that used the 1.x Megadev spellings in the middle column must move to the right column.

## BIOS function codes and vectors (`sub/bios.def.h`)

| Sega | 1.x | 2.0 |
|---|---|---|
| `MSCSTOP` | `BIOS_MSC_STOP` | `BIOS_AUDIO_STOP` |
| `MSCPAUSEON` | `BIOS_MSC_PAUSEON` | `BIOS_AUDIO_PAUSE` |
| `MSCPAUSEOFF` | `BIOS_MSC_PAUSEOFF` | `BIOS_AUDIO_RESUME` |
| `MSCSCANFF` | `BIOS_MSC_SCANFF` | `BIOS_AUDIO_SCAN_FORWARD` |
| `MSCSCANFR` | `BIOS_MSC_SCANFR` | `BIOS_AUDIO_SCAN_BACKWARD` |
| `MSCSCANOFF` | `BIOS_MSC_SCANOFF` | `BIOS_AUDIO_SCAN_OFF` |
| `ROMPAUSEON` | `BIOS_ROM_PAUSEON` | `BIOS_CDROM_PAUSE` |
| `ROMPAUSEOFF` | `BIOS_ROM_PAUSEOFF` | `BIOS_CDROM_RESUME` |
| `DRVOPEN` | `BIOS_DRV_OPEN` | `BIOS_DRIVE_OPEN` |
| `DRVINIT` | `BIOS_DRV_INIT` | `BIOS_DRIVE_INIT` |
| `MSCPLAY` | `BIOS_MSC_PLAY` | `BIOS_AUDIO_PLAY` |
| `MSCPLAY1` | `BIOS_MSC_PLAY1` | `BIOS_AUDIO_PLAY_ONCE` |
| `MSCPLAYR` | `BIOS_MSC_PLAYR` | `BIOS_AUDIO_PLAY_REPEAT` |
| `MSCPLAYT` | `BIOS_MSC_PLAYT` | `BIOS_AUDIO_PLAY_FROM_TIME` |
| `MSCSEEK` | `BIOS_MSC_SEEK` | `BIOS_AUDIO_SEEK` |
| `MSCSEEKT` | `BIOS_MSC_SEEKT` | `BIOS_AUDIO_SEEK_TIME` |
| `MSCSEEK1` | `BIOS_MSC_SEEK1` | `BIOS_AUDIO_SEEK_PLAY_ONCE` |
| `ROMREAD` | `BIOS_ROM_READ` | `BIOS_CDROM_READ` |
| `ROMSEEK` | `BIOS_ROM_SEEK` | `BIOS_CDROM_SEEK` |
| `ROMREADN` | `BIOS_ROM_READN` | `BIOS_CDROM_READ_COUNT` |
| `ROMREADE` | `BIOS_ROM_READE` | `BIOS_CDROM_READ_RANGE` |
| `CDBCHK` | `BIOS_CDB_CHK` | `BIOS_CHECK_COMMAND` |
| `CDBSTAT` | `BIOS_CDB_STAT` | `BIOS_GET_STATUS` |
| `CDBTOCWRITE` | `BIOS_CDBTOCWRITE` | `BIOS_TOC_WRITE` |
| `CDBTOCREAD` | `BIOS_CDBTOCREAD` | `BIOS_TOC_READ` |
| `CDBPAUSE` | `BIOS_CDBPAUSE` | `BIOS_SET_PAUSE_TIMEOUT` |
| `FDRSET` | `BIOS_FDR_SET` | `BIOS_FADER_SET` |
| `FDRCHG` | `BIOS_FDR_CHG` | `BIOS_FADER_CHANGE` |
| `CDCSTARTP` | `BIOS_CDC_STARTP` | `BIOS_CDC_START_P` |
| `CDCSTAT` | `BIOS_CDC_STAT` | `BIOS_CDC_STATUS` |
| `CDCREAD` | `BIOS_CDCREAD` | `BIOS_CDC_READ` |
| `CDCTRN` | `BIOS_CDC_TRN` | `BIOS_CDC_TRANSFER` |
| `CDCSETMODE` | `BIOS_CDCSETMODE` | `BIOS_CDC_SET_MODE` |
| `SCDINIT` | `BIOS_SCDINIT` | `BIOS_SUBCODE_INIT` |
| `SCDSTART` | `BIOS_SCDSTART` | `BIOS_SUBCODE_START` |
| `SCDSTOP` | `BIOS_SCDSTOP` | `BIOS_SUBCODE_STOP` |
| `SCDSTAT` | `BIOS_SCDSTAT` | `BIOS_SUBCODE_STATUS` |
| `SCDREAD` | `BIOS_SCDREAD` | `BIOS_SUBCODE_READ` |
| `SCDPQ` | `BIOS_SCDPQ` | `BIOS_SUBCODE_READ_PQ` |
| `SCDPQL` | `BIOS_SCDPQL` | `BIOS_SUBCODE_READ_PQ_LAST` |
| `LEDSET` | `BIOS_LEDSET` | `BIOS_LED_SET` |
| `WONDERREQ` | `BIOS_WONDERREQ` | `BIOS_WONDER_REQUEST` |
| `WONDERCHK` | `BIOS_WONDERCHK` | `BIOS_WONDER_CHECK` |
| `CDBIOS` | `CDBIOS` | `BIOS_CALL_VECTOR` |
| `WAITVSYNC` | `WAITVSYNC` | `BIOS_WAIT_VSYNC` |
| `SETJMPTBL` | `SETJMPTBL` | `BIOS_SET_JUMP_TABLE` |
| `CDSTAT` | `CDSTAT` | `BIOS_CD_STATUS` |
| `BOOTSTAT` | `BOOTSTAT` | `BIOS_BOOT_STATUS` |
| `USERMODE` | `USERMODE` | `BIOS_USER_MODE` |
| `INT2FLAG` | `INT2FLAG` | `BIOS_INT2_FLAG` |
| `_USERCALL0` to `_USERCALL3` | `USERCALL0` to `USERCALL3`, `SP_INIT` to `SP_USER` | `SP_INIT`, `SP_MAIN`, `SP_INT2`, `SP_USER` |

`BIOS_CDC_START`, `BIOS_CDC_STOP`, `BIOS_CDC_ACK` and `BIOS_LED_*` keep their names.

## BIOS C wrappers (`sub/bios.h`)

| 1.x | 2.0 |
|---|---|
| `bios_msc_stop()` | `bios_audio_stop()` |
| `bios_msc_pauseon()` | `bios_audio_pause()` |
| `bios_msc_pauseoff()` | `bios_audio_resume()` |
| `bios_msc_scanff()` | `bios_audio_scan_forward()` |
| `bios_msc_scanfr()` | `bios_audio_scan_backward()` |
| `bios_msc_scanoff()` | `bios_audio_scan_off()` |
| `bios_msc_play()` | `bios_audio_play()` |
| `bios_msc_play1()` | `bios_audio_play_once()` |
| `bios_msc_playr()` | `bios_audio_play_repeat()` |
| `bios_msc_playt()` | `bios_audio_play_from_time()` |
| `bios_msc_seek()` | `bios_audio_seek()` |
| `bios_msc_seekt()` | `bios_audio_seek_time()` |
| `bios_msc_seek1()` | `bios_audio_seek_play_once()` |
| `bios_rom_read()` | `bios_cdrom_read()` |
| `bios_rom_readn()` | `bios_cdrom_read_count()` |
| `bios_rom_reade()` | `bios_cdrom_read_range()` |
| `bios_rom_seek()` | `bios_cdrom_seek()` |
| `bios_rom_pauseon()` | `bios_cdrom_pause()` |
| `bios_rom_pauseoff()` | `bios_cdrom_resume()` |
| `bios_drv_open()` | `bios_drive_open()` |
| `bios_cdb_chk()` | `bios_check_command()` |
| `bios_cdb_stat()` | `bios_get_status()` |
| `bios_fdr_set()` | `bios_fader_set()` |
| `bios_fdr_chg()` | `bios_fader_change()` |
| `bios_cdc_stat()` | `bios_cdc_status()` |
| `bios_cdcread()` | `bios_cdc_read()` |
| `bios_cdc_trn()` | `bios_cdc_transfer()` |
| `bios_waitvsync()` | `bios_wait_vsync()` |

## Backup RAM (`sub/bram.def.h`, `sub/bram.h`)

| Sega | 1.x | 2.0 |
|---|---|---|
| `BURAM` | `BURAM` | `BRAM_CALL_VECTOR` |
| `BRMINIT` | `BRMINIT` | `BRAM_INIT` |
| `BRMSTAT` | `BRMSTAT` | `BRAM_STATUS` |
| `BRMSERCH` | `BRMSERCH` | `BRAM_SEARCH` |
| `BRMREAD` | `BRMREAD` | `BRAM_READ` |
| `BRMWRITE` | `BRMWRITE` | `BRAM_WRITE` |
| `BRMDEL` | `BRMDEL` | `BRAM_DELETE` |
| `BRMFORMAT` | `BRMFORMAT` | `BRAM_FORMAT` |
| `BRMDIR` | `BRMDIR` | `BRAM_DIRECTORY` |
| `BRMVERIFY` | `BRMVERIFY` | `BRAM_VERIFY` |
| `` | `BRM__UNKNOWN0` | `BRAM_UNKNOWN0` |
| `` | `BRM__UNKNOWN1` | `BRAM_UNKNOWN1` |

| 1.x | 2.0 |
|---|---|
| `bram_brminit` | `bram_init` |
| `bram_brmstat` | `bram_status` |
| `bram_brmserch` | `bram_search` |
| `bram_brmread` | `bram_read` |
| `bram_brmwrite` | `bram_write` |
| `bram_brmdel` | `bram_delete` |
| `bram_brmdir` | `bram_directory` |
| `bram_brmformat` | `bram_format` |
| `bram_brmverify` | `bram_verify` |
| `BrminitRes` | `BramInitResult` |
| `BrmstatRes` | `BramStatusResult` |
| `BrmserchRes` | `BramSearchResult` |
| `BrmreadRes` | `BramReadResult` |
| `BrmverifyStatus` | `BramVerifyStatus` |
| `brmstat_results` | `bram_status_result` |
| `brmserch_results` | `bram_search_result` |
| `brmread_results` | `bram_read_result` |

## CD Boot (`sub/boot.def.h`, formerly `sub/cdboot.def.h`)

| Sega | 1.x | 2.0 |
|---|---|---|
| `CDBOOT` | `CDBOOT` | `BOOT_CALL_VECTOR` |
| `CBTINIT` | `CBTINIT` | `BOOT_INIT` |
| `CBTINT` | `CBTINT` | `BOOT_INT` |
| `CBTOPENDISC` | `CBTOPENDISC` | `BOOT_OPEN_DISC` |
| `CBTOPENSTAT` | `CBTOPENSTAT` | `BOOT_OPEN_STATUS` |
| `CBTCHKDISC` | `CBTCHKDISC` | `BOOT_CHECK_DISC` |
| `CBTCHKSTAT` | `CBTCHKSTAT` | `BOOT_CHECK_STATUS` |
| `CBTIPDISC` | `CBTIPDISC` | `BOOT_LOAD_IP` |
| `CBTIPSTAT` | `CBTIPSTAT` | `BOOT_LOAD_IP_STATUS` |
| `CBTSPDISC` | `CBTSPDISC` | `BOOT_LOAD_SP` |
| `CBTSPSTAT` | `CBTSPSTAT` | `BOOT_LOAD_SP_STATUS` |

## Assembly call macros (`sub/sub.macro.s`)

| 1.x | 2.0 |
|---|---|
| `BIOSCALL #code` | `BIOS_CALL #code` |
| `BURAM #code` | `BRAM_CALL #code` |
| `CDBOOT #code` | `BOOT_CALL #code` |

# C library subset

Megadev now provides a freestanding subset of the C standard library (`string.h`, `stdlib.h`, `stdio.h`, `ctype.h`, `assert.h`); see the Standard Library section of `dev_in_c.md`. A standard name has standard semantics, which changes three 1.x functions:

| 1.x | 2.0 |
|---|---|
| `strcpy` in `memory.h`, returning `void` | `strcpy` in `string.h`, returning the destination. Include `string.h`. |
| `strcmp` in `str_util.h`, returning `bool` (true when equal) | `strcmp` in `string.h`, returning `int` (0 when equal). Code that tested `if (strcmp(a, b))` for equality must test `== 0`. |
| `div`, `divu` and `div_t` in `math.h`, 32-by-16 hardware divides with 16-bit results | `divs16`, `divu16` and `Div16` in `math.h`. `div` and `div_t` in `stdlib.h` are the standard functions. |
| `abs`, `abs16` in `math.h` | `abs` and `labs` in `stdlib.h` |

`strcmpt`, the comparison with a caller-supplied terminator, keeps its name.

`types.h` now defines `size_t`, `ptrdiff_t` and the `intN_t` types from the compiler's own type macros. On the 68000 they are the same sizes as before; `int32_t` is `int` rather than `long`, which only matters to code that printed or cast them by name.

# Module startup code

## Every module must link startup code

The module linker scripts (`cfg/module_mmd.ld`, `cfg/module_smd.ld`) now reject a module with no startup code, with the error `module has no init code`. In 1.x such a module linked, started at whichever function the compiler placed first, and left its global variables uninitialized.

Add `std_init.s` to the module's prerequisites, or `main/ipx_init.s` for a self-loading module. A module that does its own RAM setup defines `MODULE_NO_INIT` instead:

```c
#include <macro.h>
GLOBAL_SYM(MODULE_NO_INIT, 1);
```

# Linker script symbols

Every linker script now defines the same layout symbols, listed in `docs/dev_in_c.md` under Layout symbols. All lengths are in bytes.

| 1.x | 2.0 |
|---|---|
| Cartridge `_rom_org`, `_rom_len` | `_ROM_ORIGIN`, `_ROM_LENGTH` |
| Cartridge `_text_org`, `_text_len` | `_TEXT_ORIGIN`, `_TEXT_LENGTH` |
| Cartridge `_data_org`, `_data_len` | `_RAM_DATA_ORIGIN`, `_RAM_DATA_LENGTH` |
| Cartridge `_bss_org`, `_bss_len` | `_BSS_ORIGIN`, `_BSS_LENGTH` |
| Cartridge `_ram_len` | `_RAM_LENGTH` |
| Cartridge `_text_end`, `_rom_end`, `_data_end`, `_bss_end`, `_ram_org` | Removed; add the length to the origin |
| IP and SP `_TEXT_ORIGIN`, `_TEXT_LENGTH` | Now cover the code only. The whole image, including the security block or SP header and read-only data, is `_ROM_ORIGIN`, `_ROM_LENGTH`. |
| IP and SP `_RAM_DATA_LENGTH` | Now bytes, not longs |
| IP and SP `_RAM_ORIGIN` | `_RAM_DATA_ORIGIN` |
| Module `_ROM_LENGTH_LOOPSZ` | Removed |

# Resident modules

## Duplicate names are errors

A module that lists a resident module among its prerequisites links against the resident module's exports instead of the whole module, and `-z muldefs` is no longer used. A global function or variable defined in both modules now fails the link with `multiple definition`; in 1.x the module's own definition silently won. Rename one, make it `static`, or hide the resident module's copy with `__attribute__((visibility("hidden")))`.

Layout symbols, linker script symbols, `main`, `init`, `hblank` and `vblank` are not exported and need no change. `GLOBAL_SYM` and the assembly `GLOBAL` macro now make their symbols hidden.
