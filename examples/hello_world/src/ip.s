
.section .text

#include <macros.s>
#include <system.macros.s>
#include <main/memmap.def.h>
#include <main/bios.def.h>
#include <main/vdp.macros.s>
#include <main/gate_arr.def.h>
#include <main/vdp.def.h>
#include <main/io.def.h>

/*
  Very simple Hello World example
  We'll load the BIOS font and display some text
*/

  // First, disable all interrupts while we get things set up
  DISABLE_INTERRUPTS

  // clear Work RAM so there are no nasty surprises in memory (such as leftovers
  // from the "Produced by or Under License From" screen)
  moveq    #0, d0
  move.l   #_BSS_LENGTH, d1
  lea      _BSS_ORIGIN, a0
  bra 1f
0:move.l   d0, (a0)+
1:dbra     d1, 0b

  jbsr     BIOS_LOAD_DEFAULT_VDPREGS
  jbsr     BIOS_CLEAR_VRAM
  jbsr     BIOS_CLEAR_COMM

  move     #0, (BIOS_VINT_HANDLER_FLAGS)
  move.l   #BIOS_VINT_HANDLER, (EXVEC_LEVEL6)

  /*
    Now we'll load the internal Boot ROM font into the VDP with the default
    settings
  */
  jbsr BIOS_LOAD_FONT_DEFAULTS
  
  // The font uses palette entry #1, so we'll manually set that to white
  move.l   #0xC0020000, (VDP_CTRL)
  move.w   #0x0EEE, (VDP_DATA)

  // Call our utility function to get the x/y position of a tile
  move.w   #0x0205, d0
  jbsr     plane_xy_pos

  lea str_hello, a1
  jbsr     BIOS_PRINT

  // we can finally turn the display output back on now that everything is
  // prepared
  jbsr     BIOS_VDP_DISP_ENABLE

  // and restore interrupts
  ENABLE_INTERRUPTS

loop:
  jbsr     BIOS_VINT_WAIT_DEFAULT
  // Inputs are updated as part of the default vint wait subroutine
  // so we can assume the input value is current
  and.b    #PAD_START, BIOS_JOY1_HIT
	beq      loop
	
  jmp      BIOS_RESET

SUB plane_xy_pos
1:move.w   (BIOS_PLANE_WIDTH_CACHE), d1  // d1 - tiles per row 
  move.w   d0, d2  // d0 - x/y offset (upper/lower bytes of the word)
  lsr.w    #8, d2  // d2 has x pos
  and.w    #0xFF, d0  // filter d0 so it only has y pos
  mulu     d1, d0
  // d0 is now y pos * tiles per row
  // add x pos
  add.b    d2, d0
  // multiply by 2 for tilemap entry size
  lsl.l    #1, d0
  // TODO: make this dynamic
  add.w    #0xC000, d0

  // convert the address in d0 to vdp_ptr format
  TO_VDPPTR
  // and set the VRAM write operation flags
  or.l     #VRAM_W, d0
  rts

.section .rodata
str_hello:
.ascii "Hello World!\xff"

