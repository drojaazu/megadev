/**
 * @file
 * @brief Simple exception handler
 * @details Intended for the Main CPU
 *
 * @note printval.s is expected to be compiled into the final assembly in which
 * this is used!
 */

#ifndef MEGADEV__MAIN_EXCEPTION_S
#define MEGADEV__MAIN_EXCEPTION_S

#include "macros.s"
#include "main/bootlib.def.h"
#include "main/memmap.def.h"
#include "main/io.def.h"
#include "main/vdp.def.h"

.section .rodata

#define PLA_ADDR_DEBUG 0xc000
#define PLB_ADDR_DEBUG 0xe000

vdp_layout_debug:
#if VIDEO == PAL
#define VIDEO_SIGNAL_FLAG VR01_PAL_VIDEO
#else
#define VIDEO_SIGNAL_FLAG 0
#endif
.word _VDPREG00_MODE1 | VR00_HICOLOR_ENABLE
.word _VDPREG01_MODE2 | VR01_MD_DISPLAY_ENABLE | VR01_DMA_ENABLE | VR01_VINT_ENABLE | VIDEO_SIGNAL_FLAG | VR01_DISPLAY_ENABLE
.word _VDPREG02_PLA_ADDR | (PLA_ADDR_DEBUG / 0x400)
.word _VDPREG03_WD_ADDR | (0xa00 / 0x400)
.word _VDPREG04_PLB_ADDR | (PLB_ADDR_DEBUG / 0x2000)
.word _VDPREG05_SPR_ADDR | (0xb800 / 0x200)
.word _VDPREG06_SPR_ADDR2
.word _VDPREG07_BGCOLOR
.word _VDPREG0A_HINT_COUNT
.word _VDPREG0B_MODE3 | VR0B_EXTINT_ENABLE
.word _VDPREG0C_MODE4 | VR0C_WIDTH_40CELL
.word _VDPREG0D_HS_ADDR | (0xbc00 / 0x400)
.word _VDPREG0E_PL_ADDR2
.word _VDPREG0F_AUTOINC | 2
.word _VDPREG10_PL_SIZE | VR10_W64 | VR10_H64
.word _VDPREG11_WD_HPOS
.word _VDPREG12_WD_VPOS
.word 0

.section .text

FUNC set_vdp_layout_debug
	move.l #0xC0020000, (_VDP_CTRL)
	move.w #COLOR_WHITE, (_VDP_DATA)
	move.w #COLOR_RED, (_VDP_DATA)
	move.w #COLOR_GREEN, (_VDP_DATA)
	move.w #COLOR_BLUE, (_VDP_DATA)
	jsr _BLIB_CLEAR_VRAM
	lea vdp_layout_debug, a1
	jsr _BLIB_LOAD_VDPREGS
	jsr _BLIB_LOAD_FONT_DEFAULTS
	rts

// TODO
// - save stack frame
// - *precalculate the text positions*

/**
 * @fn install_handlers
 * Repoints the exception vectors in the system jump table to our text display
 * routines
 */
FUNC install_handlers
	move.l #ex_addr_err, _MADRERR+2
	move.l #ex_zero_div, _MDIVERR+2
	move.l #ex_chk_inst, _MONKERR+2
	move.l #ex_trapv, _MTRPERR+2
  move.l #ex_priv_viol, _MSPVERR+2
	move.l #ex_trace, _MTRACE+2
	move.l #ex_line1010, _MNOCOD0+2
	move.l #ex_line1111, _MNOCOD1+2
  rts


// The Main CPU does not set a pointer into the jump table in the Bus Addr
// vector. Instead it is hardcoded to reset the system.
/*
FUNC ex_bus_err
	move.l #strBUSERR, (err_str_ptr)
	jbra ex_group0_stack_copy
*/

FUNC ex_addr_err
	move.l #strADDRERR, (err_str_ptr)
ex_group0_stack_copy:
  adda.l #2, sp  // skip the first word (extrended info)
	move.l (sp)+, (addr_val)
	move.w (sp)+, (op_val)
	move.w (sp)+, (sr_val)
	move.l (sp)+, (pc_val)
	jbra handle_exception

// Address Error and Illegal Instruction share the same user vector.
// This poses a problem since the two are in different exception groups
// meaning the stack information will be incorrect for one of the two.
// We're going to keep the address error code since we feel that is the
// more common of the two.
/*
FUNC ex_ill_inst
	move.l #strILLEGAL, (err_str_ptr)
	jbra handle_exception
*/

FUNC ex_zero_div
	move.l #strZERODIV, (err_str_ptr)
	jbra ex_group1_stack_copy

FUNC ex_chk_inst
	move.l #strCHKINST, (err_str_ptr)
	jbra ex_group1_stack_copy

FUNC ex_trapv
	move.l #strTRAPV, (err_str_ptr)
	jbra ex_group1_stack_copy

FUNC ex_priv_viol
	move.l #strPRIVV, (err_str_ptr)
	jbra ex_group1_stack_copy

FUNC ex_trace
	move.l #strTRACE, (err_str_ptr)
	jbra ex_group1_stack_copy

FUNC ex_line1010
	move.l #strL1010, (err_str_ptr)
	jbra ex_group1_stack_copy

FUNC ex_line1111
	move.l #strL1111, (err_str_ptr)

// Spurious, like Bus Addr, is hardcoded to restart the machine
/*
	jbra ex_group1_stack_copy

FUNC ex_spurious
	move.l #strSPUR, (err_str_ptr)
*/

ex_group1_stack_copy:
	move.l #0, (addr_val)
	move.w #0, (op_val)
	move.w (sp)+, (sr_val)
	move.l (sp)+, (pc_val)
//	jbra handle_exception

FUNC handle_exception
	ori #0x700,sr
	jsr set_vdp_layout_debug
	
	// error titles
	move.w #0x0205, d0
	jbsr nmtbl_xy_pos
	movea.l (err_str_ptr), a1
	jbsr _BLIB_PRINT

	// PC=
	move.w #0x0306, d0
	jbsr nmtbl_xy_pos
	lea str_pc, a1
	jbsr _BLIB_PRINT

	// pc val
	move.l (pc_val), d0
	lea str_cache, a0
	jbsr printval_u32
	move.w #0x0806, d0
	jbsr nmtbl_xy_pos
	lea str_cache, a1
	jbsr _BLIB_PRINT

	// SR=
	move.w #0x0307, d0
	jbsr nmtbl_xy_pos
	lea str_sr, a1
	jbsr _BLIB_PRINT

  // sr val
	move.w (sr_val), d0
	lea str_cache, a0
	jbsr printval_u16
	move.w #0x0807, d0
	jbsr nmtbl_xy_pos
	lea str_cache, a1
	jbsr _BLIB_PRINT

	// OP=
	move.w #0x0308, d0
	jbsr nmtbl_xy_pos
	lea str_op, a1
	jbsr _BLIB_PRINT

  // op val
	move.w (op_val), d0
	lea str_cache, a0
	jbsr printval_u16
	move.w #0x0808, d0
	jbsr nmtbl_xy_pos
	lea str_cache, a1
	jbsr _BLIB_PRINT

	// ADDR=
	move.w #0x0309, d0
	jbsr nmtbl_xy_pos
	lea str_addr, a1
	jbsr _BLIB_PRINT

  // addr val
	move.l (addr_val), d0
	lea str_cache, a0
	jbsr printval_u32
	move.w #0x0809, d0
	jbsr nmtbl_xy_pos
	lea str_cache, a1
	jbsr _BLIB_PRINT

	// stop and enable all interrupts (for e.g. serial comm)
	stop #2000

FUNC nmtbl_xy_pos
1:move.w (_BLIB_PLANE_WIDTH), d1  // d1 - tiles per row
	move.w d0, d2  // d0 - x/y offsey (upper/lower bytes of the word)
	lsr.w #8, d2  // d2 has x pos
	and.w #0xff, d0  // filter d0 so it only has y pos
	mulu d1, d0
	# d0 is now y pos * tiles per row
	# add x pos
	add.b d2, d0
	# multiply by 2 for tilemap entry size
	lsl.l #1, d0
	# TODO: make this dynamic
	# or make sure we set the nmtbl base register
	add.w #0xc000, d0

	and.l #0xffff, d0
	lsl.l #2, d0
	lsr.w #2, d0
	or.l #0x4000, d0
	swap d0
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

str_pc:   .ascii "  PC=\xff"
.align 2
str_sr:   .ascii "  SR=\xff"
.align 2
str_addr: .ascii "ADDR=\xff"
.align 2
str_op:  .ascii "  OP=\xff"
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