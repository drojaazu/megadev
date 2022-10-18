/**
 * @file
 * @brief Simple exception handler
 * @details Intended for the Main CPU
 *
 * @note printval.s is expected to be compiled into the final assembly in which
 * this is iused!
 */

#ifndef MEGADEV__MAIN_EXCEPTION_S
#define MEGADEV__MAIN_EXCEPTION_S

#include "macros.s"
#include "main/bootlib_def.h"
#include "main/memmap_def.h"
#include "main/io_def.h"
#include "main/vdp_def.h"

.section .text

// TODO this is embarrasing, make this better
// - save stack frame
// - reset vdp to a known state
// - clear VRAM
// - load font
// - *precalculate the text positions*
// - display text

/**
 * @fn install_handlers
 * Repoints the exception vectors in the system jump table to our text display
 * routines
 */
FUNC install_handlers
	move.l ex_addr_err, _MADRERR+2
	move.l ex_zero_div, _MDIVERR+2
	move.l ex_chk_inst, _MONKERR+2
	move.l ex_trapv, _MTRPERR+2
  move.l ex_priv_viol, _MSPVERR+2
	move.l ex_trace, _MTRACE+2
	move.l ex_line1010, _MNOCOD0+2
	move.l ex_line1111, _MNOCOD1+2
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
	// todo: should probably reset VDP first
	jbsr _BLIB_CLEAR_TABLES
	jbsr _BLIB_LOAD_FONT_DEFAULTS
	move.w #0, (_BLIB_FONT_TILE_BASE)

	// set color to white
	move.l #0xC0020000, (_VDP_CTRL)
	move.w #0x0eee, (_VDP_DATA)
	
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

1:jbsr _BLIB_UPDATE_INPUTS
  and.b #PAD_START_MSK, _BLIB_JOY1_PRESS
	beq 1b

	jmp _BLIB_RESET

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
strADDRERR: .ascii "ADDRESS ERROR\xff"
.align 2
/*
strILLEGAL: .ascii "ILLEGAL INSTRUCTION\xff"
.align 2
*/
strZERODIV: .ascii "ZERO DIVIDE\xff"
.align 2
strCHKINST: .ascii "CHK INSTRUCTION\xff"
.align 2
strTRAPV:   .ascii "TRAPV\xff"
.align 2
strPRIVV:   .ascii "PRIVELEGE VIOLATION\xff"
.align 2
strTRACE:   .ascii "TRACE\xff"
.align 2
strL1010:   .ascii "LINE 1010 EMULATOR\xff"
.align 2
strL1111:   .ascii "LINE 1111 EMULATOR\xff"
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