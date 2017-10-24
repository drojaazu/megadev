/*
	This is the initial Sub CPU side code
  This example is too simple to have anything going on here!
	Check out one of the other examples for something more substantial.
*/

#include "macros.s"
#include "sub/bios_def.h"
#include "sub/sub_macros.s"

.section .text

GLABEL sp_init
	CLEAR_COMM_REGS
  rts

GLABEL sp_int2
GLABEL sp_main
GLABEL sp_user
  rts

