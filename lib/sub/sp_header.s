/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sp_header.s
 * @brief SP header definitions
 */

.section .header
sp_header:
  // The Sub BIOS runs the SP only if its name starts with MAIN (or bytes 8-11
  // are SYS, SUB or DAT and a zero); any other name is skipped silently.
  // Retail discs use "MAIN       " or "MEGA_CD_SYS". See docs/boot.md.
  //       ###########
  .ascii  "MAIN       "
  .byte   0                       // flag
  .word   0x0100                  // version
  .word   0	                      // type
  .long   0                       // pointer to next module
  .long   _ROM_LENGTH             // module size - size of ROM section
  .long   sp_jmptbl - sp_header   // offset to jump table
  .long   _RAM_LENGTH             // work ram size - size of RAM section

sp_jmptbl:
  .word   sp_init - sp_jmptbl
  .word   sp_main - sp_jmptbl
  .word   sp_int2 - sp_jmptbl
  .word   sp_user - sp_jmptbl
  .word   0
