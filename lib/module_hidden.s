/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file module_hidden.s
 * @brief Keep a module's entry points out of the symbols it exports
 *
 * @details
 * Linked into every module by megadev.make. Every module defines main, and
 * may define init and the hblank and vblank handlers named in an MMD header,
 * so these must not be exported to modules that link against this one. The
 * weak, hidden references below give each of them hidden visibility when it
 * is defined, without requiring it to be. The section is discarded by the
 * module linker scripts and adds nothing to the module.
 */

.section .megadev.hidden, "", @progbits
.weak main
.hidden main
.long main
.weak init
.hidden init
.long init
.weak hblank
.hidden hblank
.long hblank
.weak vblank
.hidden vblank
.long vblank
