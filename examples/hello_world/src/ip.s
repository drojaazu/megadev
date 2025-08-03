

// Boot security block - This must be at the top of your IP!
#include "security.s"

.section .text

/*
  Very simple Hello World example
  We'll load the BIOS font and display some text
*/

#include <macros.s>
#include <main/memmap.def.h>
#include <main/bios.def.h>
#include <main/vdp.macro.s>
#include <main/gate_arr.def.h>
#include <main/vdp.def.h>
#include <main/io.def.h>

ip_entry:
  // First, disable all interrupts while we get things set up
  ori #0x700,sr

  // clear Work RAM so there are no nasty surprises in memory (such as leftovers
  // from the "Produced by or Under License From" screen)
  // of course, since the IP was loaded to the start of Work RAM, we need to
  // clear everything that follows it
  moveq #0, d0
  move.l #_RAM_LENGTH - 1, d7
  lea _RAM_ORIGIN, a0
0:move.b d0, (a0)+
  dbra d7, 0b

  // next we'll take care of some display housekeeping
  // disable VDP display and set Mega Drive video mode
  move.w #(_VDPREG_MODE2 | 0x44), (_VDP_CTRL)

  // set palette entry 0 (background) to black
  move.l #0xC0000000, (_VDP_CTRL)
  move.w #0x0000, (_VDP_DATA)

  // clear out VRAM
  // (note: this does not clear CRAM!)
	// This is a Boot ROM library call that makes use of the VDP register cache
	// Even if you don't plan to use the Boot ROM library, this call is safe
	// to use here as the memory for the register cache is not yet in use
  jbsr _BIOS_CLEAR_VRAM

  // One of the most, if not *the* most important event while the machine is
  // running is the vertical blanking interval, during which time most of the
  // game's logic is processed. The machine generates a vertical blank interrupt
  // (VINT) when this occurs. Since we don't have anything special going on at
  // this point, we can use the built-in handler for VINTs, which takes care of
  // a number of steps for us, including (most importantly) notifying the Sub
  // CPU that a VINT has occurred. The Level 6 interrupt is the VINT, so we
  // set that vector to the handler in the Boot Library.
  // (Note that we set it at _MLEVEL6 + 2; this is because we are actually
  // modifying a jump table, and the first two bytes are the JMP opcode.)
  move.l	#_BIOS_VINT_HANDLER, (_MLEVEL6 + 2)

  // we don't have any significant code on the Sub CPU side for this minimal
  // demo, so we won't be using the Gate Array comm registers to communicate
  // between the CPUs, but it's still good practice to have the registers
  // initialized
  jbsr _BIOS_CLEAR_COMM

  // We'll also use the Boot ROM VDP defaults
  // (these defaults include disabling the display)
  jbsr _BIOS_LOAD_VDPREGS_DEFAULT
  
  // Clear all of VRAM to give a fresh start
  jbsr _BIOS_CLEAR_VRAM

  /*
    Now we'll load the internal Boot ROM font into the VDP with the default
    settings
  */
  jbsr _BIOS_LOAD_FONT_DEFAULTS
  
  // The font uses palette entry #1, so we'll manually set that to white
  move.l #0xC0020000, (_VDP_CTRL)
  move.w #0x0EEE, (_VDP_DATA)

  // Call our utility function to get the x/y position of a tile
  move.w #0x0205, d0
  jbsr nmtbl_xy_pos

  lea str_hello, a1
  jbsr _BIOS_PRINT

  // we can finally turn the display output back on now that everything is
  // prepared
  jbsr _BIOS_VDP_DISP_ENABLE

  // and restore interrupts
  andi #0xF8FF,sr

loop:
  jbsr _BIOS_VINT_HANDLER_WAIT_DEFAULT
  // Inputs are updated as part of the default vint wait subroutine
  // so we can assume the input value is current
  and.b #PAD_START, _BIOS_JOY1_PRESS
	beq loop
	
  jmp _BIOS_RESET

SUB nmtbl_xy_pos
1:move.w (_BIOS_VDP_DEFAULT_PLANE_WIDTH), d1  // d1 - tiles per row 
  move.w d0, d2  // d0 - x/y offset (upper/lower bytes of the word)
  lsr.w #8, d2  // d2 has x pos
  and.w #0xff, d0  // filter d0 so it only has y pos
  mulu d1, d0
  // d0 is now y pos * tiles per row
  // add x pos
  add.b d2, d0
  // multiply by 2 for tilemap entry size
  lsl.l #1, d0
  // TODO: make this dynamic
  add.w #0xc000, d0

  // convert the address in d0 to vdpptr format
  TO_VDPPTR
  // and set the VRAM write operation flags
  or.l #VRAM_W, d0
  rts

.section .rodata
str_hello:
.ascii "Hello World!\xff"
.align 2
