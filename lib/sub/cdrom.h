/**
 * \file sub/cdrom.h
 * \brief C wrappers for CD-ROM file access
 */

#ifndef MEGADEV__CD_SUB_CDROM_H
#define MEGADEV__CD_SUB_CDROM_H

#include "sub/cdrom_def.h"
#include "types.h"

/**
 * \var file_buff
 * \brief Pointer to the buffer to store the output file
 * \note This is only used with the Sub CPU Direct Load operation!
 * For the DMA operations, use the GA_DMAADDR register to specify a
 * destination buffer.
 */
extern u8 const* filebuff;

/**
 * \var filename
 * \brief Pointer to the zero-terminated filename string
 * \details The filename is in 8.3 format, made up of ISO9660 d characters, and
 * includes the version suffix.
 */
extern char const* filename;

/**
 * \var access_op
 * \brief Place an access operation value in this variable to begin
 * a load process
 */
extern volatile u16 access_op;

/**
 * \var access_op_result
 * \brief Contains the status of an access operation after it has completed
 */
extern volatile u16 access_op_result;

extern void *acc_loop_jump;

extern void init_acc_loop();

static inline u16 load_file(u8 const this_acc_op, char const * this_filename, u8 * this_filebuff) {
  access_op = this_acc_op;
  filename = this_filename;
  filebuff = this_filebuff;

  do {
    bios_waitvsync();
  } while (access_op != ACC_OP_IDLE);

  return access_op_result;
}

// TODO file info struct!
static inline void get_file_info_c(u8 *filename) {
  register u32 a0_filename asm("a0") = (u32)filename;
  register u32 a0_fileinfo asm("a0");
  asm(R"(
  jsr get_file_info
  bcc 1f
  rts
  // if not found set "null" on the return ptr
1:lea #0,a0
  rts
  )"
	  :
		"=a"(a0_fileinfo)
		:
		"a"(a0_filename)
    :
		"d0","d1","a2"
	);
}

/**
 * Grant 2M Word RAM access to the Main CPU and wait for confirmation
 */
static inline void process_acc_loop() {
  asm(R"(
  movea.l  acc_loop_jump, a0
  jmp      (a0)  
	)"
	);
}


#endif