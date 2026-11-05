/**
 * Mode 1 example: Sub CPU program
 *
 * Alive signals on every INT2: COMCMD0 is copied to COMSTAT0 and COMSTAT2
 * counts the interrupts. Commands from the Main CPU arrive in COMCMD1 and are
 * answered in COMSTAT1 as (command << 8) | result; the Main CPU acknowledges
 * by clearing COMCMD1, and COMSTAT1 is cleared in turn. The disc work happens
 * here on command because BIOS_DRIVE_INIT waits for a disc: a cartridge must
 * keep running without one.
 *
 *   SP_CMD_DRIVE     drive init, Word RAM to 2M, cache the root directory
 *                    (COMSTAT4: Sub BIOS status word after drive init)
 *   SP_CMD_LOAD      DATA.BIN;1 to Word RAM by Sub CPU copy, then hand Word
 *                    RAM to the Main CPU (COMSTAT3: file size in bytes)
 *   SP_CMD_LOAD_DMA  the same by CDC DMA, at Word RAM offset SP_DMA_OFFSET
 *
 * The result byte is the access loop's: CDROM_RESULT_OK or an error.
 */
#include <macro.s>
#include <sub/bios.def.h>
#include <sub/cdrom.def.h>
#include <sub/cdrom.macro.s>
#include <sub/gate_arr.def.h>
#include <sub/memmap.def.h>
#include <sub/sub.macro.s>
#include "shared.h"

.section .text

// the CD-ROM access loop (docs/cdrom.md)
#include "sub/cdrom.s"
.section .text

GLABEL sp_init
  INIT_ACC_LOOP
  GA_SUB_CLEAR_COMM_REGS
  jbsr    exvec_install   // Sub CPU crash handler (sub/exvec.h)
  rts

// The access loop must be pumped last: it returns to the BIOS by itself.
GLABEL sp_int2
  move.w  (GA_SUB_REG_COMCMD0), d0
  move.w  d0, (GA_SUB_REG_COMSTAT0)
  addq.w  #1, (GA_SUB_REG_COMSTAT2)
  PROCESS_ACC_LOOP

// Called repeatedly by the Sub BIOS. One command per call, non-blocking
// between commands.
GLABEL sp_main
  move.w  (GA_SUB_REG_COMCMD1), d0
  bne     0f
  clr.w   (GA_SUB_REG_COMSTAT1)    // acknowledged: ready for the next command
  rts
0:tst.w   (GA_SUB_REG_COMSTAT1)
  beq     1f
  rts                              // answered, waiting for the acknowledge
1:cmpi.w  #SP_CMD_DRIVE, d0
  beq     cmd_drive
  cmpi.w  #SP_CMD_LOAD, d0
  beq     cmd_load
  cmpi.w  #SP_CMD_LOAD_DMA, d0
  beq     cmd_load_dma
  move.w  #SP_RESULT_BAD_CMD, d1
  bra     cmd_done

cmd_drive:
  lea     drv_init_tracklist, a0
  BIOS_CALL #BIOS_DRIVE_INIT
2:BIOS_CALL #BIOS_GET_STATUS       // wait for the TOC read to finish
  move.w  (BIOS_CD_STATUS).w, d0
  move.w  d0, (GA_SUB_REG_COMSTAT4)
  andi.w  #0xF000, d0
  bne     2b
  // Word RAM in 2M mode, owned by the Sub CPU
  andi.b  #~(GA_SUB_RETURN_2M_MASK | GA_SUB_WORDRAM_LAYOUT_MASK), (GA_SUB_REG_MEMMODE)
  move.w  #CDROM_LOAD_FILE_LIST, access_op
  WAIT_FOR_ACC_OP
  move.w  d0, d1
  bra     cmd_done

cmd_load:
  lea     data_filename, a0
  lea     WORD_RAM_2M, a1
  jbsr    load_file_sub
  bra     cmd_load_end

cmd_load_dma:
  move.l  #SP_DMA_OFFSET, filebuff   // offset within Word RAM
  lea     data_filename, a0
  move.l  a0, filename
  move.w  #CDROM_LOAD_CDC_DMA, access_op
  WAIT_FOR_ACC_OP

cmd_load_end:
  move.w  d0, d1
  move.w  filesize + 2, (GA_SUB_REG_COMSTAT3)
  // hand Word RAM to the Main CPU
3:bset    #GA_SUB_RETURN_2M_POS, (GA_SUB_REG_MEMMODE)
  btst    #GA_SUB_RETURN_2M_POS, (GA_SUB_REG_MEMMODE)
  beq     3b

cmd_done:
  move.w  (GA_SUB_REG_COMCMD1), d0
  lsl.w   #8, d0
  move.b  d1, d0
  move.w  d0, (GA_SUB_REG_COMSTAT1)
  rts

GLABEL sp_user
  rts

.section .rodata
drv_init_tracklist:
  .byte 1, 0xFF
data_filename:
  .asciz "DATA.BIN;1"
.align 2

#include "sub/exvec_sub.s"
