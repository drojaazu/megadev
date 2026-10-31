/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file mode1.s
 * @brief Mode 1 support: system vector table and Sub CPU bring-up
 *
 * @details
 * Main CPU only. Requires kos_cmp.s in the same program.
 */

#include "macro.s"
#include "main/memmap.def.h"
#include "main/gate_arr.def.h"
#include "main/gate_arr.macro.s"
#include "main/bios.def.h"
#include "main/mode1.def.h"

#define SEGA_SIGNATURE 0x53454741 /* "SEGA" */
#define JMP_ABS_L 0x4EF9

.section .text

/**
 * @fn mode1_detect
 * @brief Check for a Mega CD
 * @param[out] D0.l 1 if the Boot ROM is present at BOOT_ROM, 0 otherwise
 */
SUB mode1_detect
  moveq   #0, d0
  cmpi.l  #SEGA_SIGNATURE, (BOOT_ROM + 0x100)
  bne.s   0f
  moveq   #1, d0
0:rts

/**
 * @fn mode1_init_vectors
 * @brief Build the system vector table at 0xFFFD00
 * @clobber d0-d1/a0
 *
 * @details
 * The Boot ROM builds this table during a Mega CD boot. In Mode 1 that code
 * never runs, so the cartridge builds it: thirty `jmp xxx.l` entries. The
 * first 28 are exception vectors; the reset entry points at the cartridge
 * entry point and the others at an `rte`. The last two, BIOS_VBLANK_USER and
 * the Backup RAM manager entry, are subroutines called by the Boot ROM
 * library and point at an `rts`. Install handlers by writing their addresses
 * to the EXVEC_* locations. md_init.s calls this before enabling interrupts.
 */
SUB mode1_init_vectors
  lea     (EXVEC_RESET - 2), a0
  move.w  #JMP_ABS_L, d0
  move.w  d0, (a0)+
  move.l  #INIT_PC, (a0)+
  moveq   #(27 - 1), d1
0:move.w  d0, (a0)+
  move.l  #mode1_ex_null, (a0)+
  dbra    d1, 0b
  moveq   #(2 - 1), d1
1:move.w  d0, (a0)+
  move.l  #mode1_sub_null, (a0)+
  dbra    d1, 1b
  rts

mode1_ex_null:
  rte

mode1_sub_null:
  rts

/**
 * @fn mode1_find_font
 * @brief Locate the Boot ROM's 1bpp ASCII font
 * @param[out] A1.l Pointer to the font (96 tiles), or 0 if not found
 * @clobber d0-d1
 *
 * @details
 * BIOS_LOAD_FONT references the font by an address that only holds when the
 * Boot ROM runs at 0, so in Mode 1 the font must be loaded with
 * BIOS_LOAD_1BPP_TILES instead. Its location differs per Boot ROM revision;
 * this reads it out of the BIOS_LOAD_FONT routine itself.
 */
SUB mode1_find_font
  lea     BIOS_LOAD_FONT, a1
  move.w  2(a1), d0         // bra.w displacement
  lea     2(a1,d0.w), a1    // routine start
  moveq   #16 - 1, d1
0:cmpi.w  #0x43F9, (a1)     // lea xxx.l, a1
  beq.s   1f
  addq.l  #2, a1
  dbra    d1, 0b
  suba.l  a1, a1
  rts
1:move.l  2(a1), d0
  lea     BOOT_ROM, a1
  adda.l  d0, a1
  rts

/**
 * @fn mode1_load_sub
 * @brief Bring up the Sub CPU with the given SP
 * @param[in] A0.l Pointer to the SP binary
 * @param[in] D0.l Size of the SP binary in bytes
 * @param[out] D0.l MODE1_OK or a MODE1_ERR_* value
 * @clobber d1/a0-a1
 *
 * @details
 * Follows the Boot ROM's own sequence: hold the Sub CPU in reset and take its
 * bus, select Program RAM bank 0 in 2M mode, decompress the Sub BIOS into
 * Program RAM, place the SP at 0x6000, write protect the BIOS, release reset,
 * release the bus. The Sub BIOS image is found by scanning the Boot ROM for
 * the loader call that unpacks it to the Program RAM window, since its
 * location differs per revision. Returns once the Sub side comm flags read
 * zero, which is how the Boot ROM knows the Sub BIOS is running.
 */
SUB mode1_load_sub
  movem.l d2-d7/a2-a6, -(sp)
  movea.l a0, a2            // SP source
  move.l  d0, d7            // SP size

  cmpi.l  #SEGA_SIGNATURE, (BOOT_ROM + 0x100)
  bne     9f
  moveq   #MODE1_ERR_SP_SIZE, d0
  cmpi.l  #MODE1_SP_MAX_SIZE, d7
  bhi     8f

  // hold the Sub CPU in reset and request its bus
  lea     GA_MAIN_REG_SUBCPU, a5
  move.b  #GA_MAIN_SUB_REQ_MASK, (a5)
0:btst    #GA_MAIN_SUB_REQ_POS, (a5)
  beq.s   0b
0:btst    #GA_MAIN_SUB_RESET_POS, (a5)
  bne.s   0b

  // no write protection; bank 0, 2M mode, keep DMNA
  move.b  #0, (GA_MAIN_REG_WP)
  move.b  (GA_MAIN_REG_MEMMODE), d0
  andi.b  #GA_MAIN_DMNA_MASK, d0
  move.b  d0, (GA_MAIN_REG_MEMMODE)

  // find `lea src.l, a0 / lea 0x20000.l, a1` in the Boot ROM loader
  lea     (BOOT_ROM + 0x200), a0
  move.w  #((0x8000 - 0x200) / 2) - 1, d1
1:cmpi.w  #0x41F9, (a0)
  bne.s   2f
  cmpi.w  #0x43F9, 6(a0)
  bne.s   2f
  cmpi.l  #PRG_RAM_1M - BOOT_ROM, 8(a0)
  bne.s   2f
  move.l  2(a0), d2
  cmpi.l  #BOOT_ROM_SIZE, d2
  bcs.s   3f
2:addq.l  #2, a0
  dbra    d1, 1b
  moveq   #MODE1_ERR_NO_LOADER, d0
  bra     8f

  // unpack the Sub BIOS to Program RAM 0
3:lea     BOOT_ROM, a0
  adda.l  d2, a0
  lea     MODE1_PRG_RAM, a1
  jbsr    Kos_Decomp

  // place the SP
  movea.l a2, a0
  lea     MODE1_SP_DEST, a1
  move.l  d7, d1
  addq.l  #1, d1
  lsr.l   #1, d1
  beq.s   5f
  subq.l  #1, d1
4:move.w  (a0)+, (a1)+
  dbra    d1, 4b
5:move.b  #MODE1_PRG_RAM_WP, (GA_MAIN_REG_WP)

  // release reset, then the bus
  move.b  #(GA_MAIN_SUB_REQ_MASK | GA_MAIN_SUB_RESET_MASK), (a5)
6:btst    #GA_MAIN_SUB_RESET_POS, (a5)
  beq.s   6b
  move.b  #GA_MAIN_SUB_RESET_MASK, (a5)
6:btst    #GA_MAIN_SUB_REQ_POS, (a5)
  bne.s   6b

  // give the Sub BIOS a moment, clear the comm registers, wait for it
  move.l  #0x8000, d1
6:subq.l  #1, d1
  bne.s   6b
  GA_MAIN_CLEAR_COMM_REGS
  move.l  #MODE1_SUB_TIMEOUT, d1
7:tst.b   (GA_MAIN_REG_COMFLAGS_SUB)
  beq.s   10f
  subq.l  #1, d1
  bne.s   7b
  moveq   #MODE1_ERR_SUB_TIMEOUT, d0
  bra     8f

9:moveq   #MODE1_ERR_NO_MEGACD, d0
  bra     8f
10:moveq  #MODE1_OK, d0
8:movem.l (sp)+, d2-d7/a2-a6
  rts
