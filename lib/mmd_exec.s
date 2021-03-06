/**
 * \file mmd_exec_main.s
 */

#include "macros.s"
#include "main/main_def.h"

.section .text

/**
 * \brief Setup and run a loaded MMD formatted module
 * \note This expects the module to be already loaded at the start of 2M
 * Word RAM
 */
FUNC MMD_EXEC
	WAIT_2M
	movea.l	_MAIN_2M_BASE+8, a0	//get MMD entry point
	move.l	_MAIN_2M_BASE+2, d0	//get MMD data destination
	beq			1f				//if no destination, skip the copy
	movea.l	d0, a2		//put destination in a2
	lea			_MAIN_2M_BASE+0x100, a1	//start of MMD Data section in a1
	move.w	_MAIN_2M_BASE+6, d7		//size of MMD Data in d7
0:move.l	(a1)+, (a2)+			//copy MMD Data to destination
	dbf			d7, 0b
1:move.l	_MAIN_2M_BASE+0xc, d0	//set HINT vector if provided
	beq			2f
	move.l	d0, _MLEVEL4+2
2:move.l  _MAIN_2M_BASE+0x10, d0
	beq     3f
	move.l  d0, _MLEVEL6+2
3:btst #6, _MAIN_2M_BASE  // if bit 6 is set, return 2m to sub 
	beq 4f
	GRANT_2M
4:jmp			(a0)  // jump to the module entry
