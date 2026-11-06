/**
 * Mode 1 example: values shared by the Main and Sub CPU programs
 */
#pragma once

// Commands, Main CPU to Sub CPU through COMCMD1; see sp.s
#define SP_CMD_DRIVE 1
#define SP_CMD_LOAD 2
#define SP_CMD_LOAD_DMA 3
#define SP_CMD_LOAD_PRG 4

// Result byte of a command the Sub CPU does not know
#define SP_RESULT_BAD_CMD 0xEE

// Word RAM offset of the DMA load, so it does not overwrite the copy load
#define SP_DMA_OFFSET 0x4000

// Program RAM address of the Program RAM DMA load, clear of the SP and the
// Sub BIOS (bank 1)
#define SP_PRG_OFFSET 0x20000
