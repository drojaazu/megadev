/*
  Very simple Hello World example
  We'll load the BIOS font and display some text
*/

#include "macros.s"
#include "main/main_def.h"
#include "main/boot_def.h"
#include "vdp_macros.s"
#include "vdp_def.h"
#include "io_def.h"

.section .text

// the security code for the region must *always* be at the very beginning of
// the IP code
  .incbin "sec_us.bin"

ip_entry:
  // First, disable all interrupts while we get things set up
  ori #0x700,sr

  /*
    Something we need to do very early on is start informing the Sub CPU of 
    vblank interrupts. The VINT handler built into the Boot ROM takes care of
    this, so let's go ahead and point the VINT vector (interrupt level 6) to
    the built in handler.
  */
  move.l	#BOOT_VINT, (_MLEVEL6 + 2)

  move.b #0, (_GA_COMFLAGS)

  // We'll also use the Boot ROM VDP defaults
  // (these defaults include disabling the display)
  jbsr BOOT_LOAD_VDPREGS_DEFAULT
  
  // Clear all of VRAM to give a fresh start
  jbsr BOOT_CLEAR_VRAM

  /*
    Now we'll load the internal Boot ROM font into the VDP with the default
    settings
  */
  jbsr BOOT_LOAD_FONT_DEFAULTS
  
  // The font uses palette entry #1, so we'll manually set that to white
  move.l #0xC0020000, (_VDP_CTRL)
  move.w #0x0EEE, (_VDP_DATA)

  // Call our utility function to get the x/y position of a tile
  move.w #0x0205, d0
  jbsr nmtbl_xy_pos

  lea str_hello, a1
  jbsr BOOT_PRINT

  // And finally enable the display
  jbsr BOOT_VDP_DISP_ENABLE

  // and restore interrupts
  andi #0xF8FF,sr

loop:
  jbsr BOOT_VINT_WAIT_DEFAULT
  // Inputs are updated as part of the default vint wait subroutine
  // so we can assume the input value is current
  and.b #PAD_START_MSK, _JOY1_PRESS
	beq loop
	
  jmp BOOT_ENTRY

FUNC nmtbl_xy_pos
1:move.w (_PLANE_WIDTH), d1  // d1 - tiles per row 
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
