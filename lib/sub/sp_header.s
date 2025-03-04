/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sp_header.s
 * @brief SP header definitions
 */

.section .header
sp_header:
	.asciz  "MAIN       "
	.word   0x0100                  // version
	.word   0	                      // type
	.long   0                       // pointer to next module
	.long   _SP_ROM_LENGTH          // module size - size of ROM section
	.long   sp_jmptbl - sp_header   // offset to jump table
	.long   _SP_RAM_LENGTH          // work ram size - size of RAM section

sp_jmptbl:
	.word		sp_init - sp_jmptbl
	.word		sp_main - sp_jmptbl
	.word		sp_int2 - sp_jmptbl
	.word		sp_user - sp_jmptbl
	.word		0
