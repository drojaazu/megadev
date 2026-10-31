/**
 * Mode 1 example: Sub CPU program
 *
 * The Sub BIOS calls sp_main repeatedly once it is up. Each call copies
 * COMCMD0 to COMSTAT0 so the Main CPU can see the Sub CPU is alive.
 */
#include <macro.s>
#include <sub/gate_arr.def.h>

.section .text

GLABEL sp_init
  rts

GLABEL sp_main
  move.w  (GA_SUB_REG_COMCMD0), d0
  move.w  d0, (GA_SUB_REG_COMSTAT0)
  rts

GLABEL sp_int2
GLABEL sp_user
  rts
