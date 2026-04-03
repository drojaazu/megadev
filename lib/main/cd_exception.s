/**
 * @file cd_exception.s
 * @brief Simple exception handler for Mega CD games
 * @details Intended for the Main CPU
 *
 * @note str_util.s is expected to be compiled into the final assembly in which
 * this is used!
 */

#ifndef MEGADEV__MAIN_CD_EXCEPTION_S
#define MEGADEV__MAIN_CD_EXCEPTION_S

#include "macros.s"
#include "main/bios.def.h"
#include "main/memmap.def.h"
#include "main/io.def.h"
#include "main/vdp.def.h"

.section .rodata

#define PLA_ADDR_DEBUG 0xC000
#define PLB_ADDR_DEBUG 0xE000

vdp_layout_debug:
#if VIDEO == PAL
#define VIDEO_SIGNAL VDP_PAL_VIDEO
#else
#define VIDEO_SIGNAL 0
#endif

.word VDPREG_MODE1 | VDP_HICOLOR_ENABLE
.word VDPREG_MODE2 | VDP_MD_DISPLAY_MODE | VDP_DMA_ENABLE | VDP_VBLANK_ENABLE | VIDEO_SIGNAL | VDP_DISPLAY_ENABLE
.word VDPREG_PLA_ADDR | (PLA_ADDR_DEBUG / 0x400)
.word VDPREG_WIN_ADDR | (0xA00 / 0x400)
.word VDPREG_PLB_ADDR | (PLB_ADDR_DEBUG / 0x2000)
.word VDPREG_SPR_ADDR | (0xB800 / 0x200)
.word VDPREG_SPR_ADDR2
.word VDPREG_BGCOLOR
.word VDPREG_HBLANK_COUNT
.word VDPREG_MODE3 | VDP_EXTINT_ENABLE
.word VDPREG_MODE4 | VDP_WIDTH_40CELL
.word VDPREG_HS_ADDR | (0xBC00 / 0x400)
.word VDPREG_PL_ADDR2
.word VDPREG_AUTOINC | 2
.word VDPREG_PL_SIZE | VDP_PL_64x64
.word VDPREG_WIN_HPOS
.word VDPREG_WIN_VPOS
.word 0

.section .text

SUB set_vdp_layout_debug
  move.l  #0xC0020000, (VDP_CTRL)
  move.w  #COLOR_WHITE, (VDP_DATA)
  move.w  #COLOR_RED, (VDP_DATA)
  move.w  #COLOR_GREEN, (VDP_DATA)
  move.w  #COLOR_BLUE, (VDP_DATA)
  jsr     BIOS_CLEAR_VRAM
  lea     vdp_layout_debug, a1
  jsr     BIOS_LOAD_VDPREGS
  jsr     BIOS_LOAD_FONT_DEFAULTS
  rts

// TODO
// - save stack frame
// - *precalculate the text positions*

/**
 * @fn install_handlers
 * Repoints the exception vectors in the system jump table to our text display
 * routines
 */
SUB install_handlers
  move.l  #ex_addr_err, EXVEC_ADDRERR+2
  move.l  #ex_zero_div, EXVEC_ZERODIV+2
  move.l  #ex_chk_inst, EXVEC_CHK+2
  move.l  #ex_trapv, EXVEC_TRAPV+2
  move.l  #ex_priv_viol, EXVEC_PRIVERR+2
  move.l  #exEXVEC_TRACE, EXVECEXVEC_TRACE+2
  move.l  #ex_line1010, EXVEC_LINE1010+2
  move.l  #ex_line1111, EXVEC_LINE1111+2
  rts


// The Main CPU does not set a pointer into the jump table in the Bus Addr
// vector. Instead it is hardcoded to reset the system.
/*
SUB ex_bus_err
  move.l  #strBUSERR, (err_str_ptr)
  jbra    ex_group0_stack_copy
*/

SUB ex_addr_err
  move.l  #strADDRERR, (err_str_ptr)
ex_group0_stack_copy:
  adda.l  #2, sp  // skip the first word (extrended info)
  move.l  (sp)+, (addr_val)
  move.w  (sp)+, (op_val)
  move.w  (sp)+, (sr_val)
  move.l  (sp)+, (pc_val)
  jbra    handle_exception

// Address Error and Illegal Instruction share the same user vector.
// This poses a problem since the two are in different exception groups
// meaning the stack information will be incorrect for one of the two.
// We're going to keep the address error code since we feel that is the
// more common of the two.
/*
SUB ex_ill_inst
  move.l  #strILLEGAL, (err_str_ptr)
  jbra    handle_exception
*/

SUB ex_zero_div
  move.l  #strZERODIV, (err_str_ptr)
  jbra    ex_group1_stack_copy

SUB ex_chk_inst
  move.l  #strCHKINST, (err_str_ptr)
  jbra    ex_group1_stack_copy

SUB ex_trapv
  move.l  #strTRAPV, (err_str_ptr)
  jbra    ex_group1_stack_copy

SUB ex_priv_viol
  move.l  #strPRIVV, (err_str_ptr)
  jbra    ex_group1_stack_copy

SUB exEXVEC_TRACE
  move.l  #strTRACE, (err_str_ptr)
  jbra    ex_group1_stack_copy

SUB ex_line1010
  move.l  #strL1010, (err_str_ptr)
  jbra    ex_group1_stack_copy

SUB ex_line1111
  move.l  #strL1111, (err_str_ptr)

// Spurious, like Bus Addr, is hardcoded to restart the machine
/*
  jbra    ex_group1_stack_copy

SUB ex_spurious
  move.l  #strSPUR, (err_str_ptr)
*/

ex_group1_stack_copy:
  move.l  #0, (addr_val)
  move.w  #0, (op_val)
  move.w  (sp)+, (sr_val)
  move.l  (sp)+, (pc_val)
//	jbra  handle_exception

SUB handle_exception
  ori     #0x700,sr
  jsr     set_vdp_layout_debug
  
  // error titles
  move.w  #0x0205, d0
  jbsr    nmtbl_xy_pos
  movea.l (err_str_ptr), a1
  jbsr    BIOS_PRINT

  // PC=
  move.w  #0x0306, d0
  jbsr    nmtbl_xy_pos
  lea     str_pc, a1
  jbsr    BIOS_PRINT

  // pc val
  move.l  (pc_val), d0
  lea     str_cache, a0
  jbsr    hextoa32
  move.w  #0x0806, d0
  jbsr    nmtbl_xy_pos
  lea     str_cache, a1
  jbsr    BIOS_PRINT

  // SR=
  move.w  #0x0307, d0
  jbsr    nmtbl_xy_pos
  lea     str_sr, a1
  jbsr    BIOS_PRINT

  // sr val
  move.w  (sr_val), d0
  lea     str_cache, a0
  jbsr    hextoa16
  move.w  #0x0807, d0
  jbsr    nmtbl_xy_pos
  lea     str_cache, a1
  jbsr    BIOS_PRINT

  // OP=
  move.w  #0x0308, d0
  jbsr    nmtbl_xy_pos
  lea     str_op, a1
  jbsr    BIOS_PRINT

  // op val
  move.w  (op_val), d0
  lea     str_cache, a0
  jbsr    hextoa16
  move.w  #0x0808, d0
  jbsr    nmtbl_xy_pos
  lea     str_cache, a1
  jbsr    BIOS_PRINT

  // ADDR=
  move.w  #0x0309, d0
  jbsr    nmtbl_xy_pos
  lea     str_addr, a1
  jbsr    BIOS_PRINT

  // addr val
  move.l  (addr_val), d0
  lea     str_cache, a0
  jbsr    hextoa32
  move.w  #0x0809, d0
  jbsr    nmtbl_xy_pos
  lea     str_cache, a1
  jbsr    BIOS_PRINT

  // stop and enable all interrupts (for e.g. serial comm)
  stop    #2000

SUB nmtbl_xy_pos
1:move.w  (BIOS_VDP_DEFAULT_PLANE_WIDTH), d1  // d1 - tiles per row
  move.w  d0, d2  // d0 - x/y offsey (upper/lower bytes of the word)
  lsr.w   #8, d2  // d2 has x pos
  and.w   #0xFF, d0  // filter d0 so it only has y pos
  mulu    d1, d0
  # d0 is now y pos * tiles per row
  # add x pos
  add.b   d2, d0
  # multiply by 2 for tilemap entry size
  lsl.l   #1, d0
  # TODO: make this dynamic
  # or make sure we set the nmtbl base register
  add.w   #0xC000, d0
  and.l   #0xFFFF, d0
  lsl.l   #2, d0
  lsr.w   #2, d0
  or.l    #0x4000, d0
  swap    d0
  rts

.section .rodata

/*
strBUSERR:  .ascii "BUS ERROR\xff"
.align 2
*/
strADDRERR: .ascii "ADDR ERROR\xff"
.align 2
/*
strILLEGAL: .ascii "ILLEGAL INST\xff"
.align 2
*/
strZERODIV: .ascii "ZERO DIVIDE\xff"
.align 2
strCHKINST: .ascii "CHK INST\xff"
.align 2
strTRAPV:   .ascii "TRAPV\xff"
.align 2
strPRIVV:   .ascii "PRIV VIOLATION\xff"
.align 2
strTRACE:   .ascii "TRACE\xff"
.align 2
strL1010:   .ascii "LINE 1010\xff"
.align 2
strL1111:   .ascii "LINE 1111\xff"
.align 2
/*
strSPUR:    .ascii "SPURIOUS\xff"
.align 2
*/

str_pc:     .ascii "  PC=\xff"
.align 2
str_sr:     .ascii "  SR=\xff"
.align 2
str_addr:   .ascii "ADDR=\xff"
.align 2
str_op:     .ascii "  OP=\xff"
.align 2

.section .bss
err_str_ptr: .long 0
addr_val: .long 0
op_val: .word 0
sr_val: .word 0
pc_val: .long 0
str_cache: .space 9
.align 2


#endif
