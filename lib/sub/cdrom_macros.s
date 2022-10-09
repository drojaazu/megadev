/**
 * @file
 * CD-ROM File Access Wrapper Macros
 */

#ifndef MEGADEV__CD_SUB_CDROM_MACROS_H
#define MEGADEV__CD_SUB_CDROM_MACROS_H

#include "sub/cdrom_def.h"

/**
 * Kickstarts the load process by setting initial mode
 * This should be called in sp_init before VINT has been enabled
 */
.macro INIT_ACC_LOOP
  move.l  #access_op_idle, acc_loop_jump
  move.w  #ACC_OP_IDLE, access_op
.endm

/**
 * MUST be called every INT2 in order to keep the access loop going
 * BREAK: a0
 */
.macro PROCESS_ACC_LOOP
  movea.l  acc_loop_jump, a0  /*load the jump ptr*/
  jmp      (a0)                    /*and pick up where we left off*/
.endm

/**
 * Convenience macro to wait for an access operation to finish
 * OUT:
*   d0 - result code
 * BREAK: d1
 */
.altmacro
.macro WAIT_FOR_ACC_OP
LOCAL loop

loop:
  jsr			 _WAITVSYNC
	jbsr     check_acc_op
	bcs			 loop
.endm

#endif
