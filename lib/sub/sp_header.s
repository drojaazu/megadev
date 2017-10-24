/**
 * \file sub/sp_header.h
 * \brief SP header definitions
 */

.section .header
sp_header:
  .asciz  "MAIN       "
  .word   0x0100					// version
  .word   0								// type
  .long   0								// ptr to next module
  .long   _sp_text_len				  	// module size - size of text section
  .long   sp_jmptbl - sp_header		// offset to jump table
  .long   _sp_ram_len						  // work ram size - size of data + bss

sp_jmptbl:
  .word		sp_init - sp_jmptbl
  .word		sp_main - sp_jmptbl
  .word		sp_int2 - sp_jmptbl
  .word		sp_user - sp_jmptbl
  .word		0
