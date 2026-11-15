/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/echo_blob.s
 * @brief Echo's Z80 program, uploaded by echo_init (main/echo.h)
 *
 * @details
 * main/echo/prog-z80.bin and main/echo/prog-z80-pal.bin are the prebuilt Z80
 * programs (built/, 5120 bytes each) of Echo 1.68a-dr.1, drojaazu/echo, a fork
 * of Echo 1.68a (sikthehedgehog/echo commit e326c87) whose FM and PSG notes are
 * tuned to the NTSC or the PAL clocks, and whose PSG semitone shifts are kept
 * inside the frequency table (main/echo/NOTICE). VIDEO in the project makefile
 * picks the program.
 * Echo is distributed under the zlib license: main/echo/LICENSE.
 */

.section .rodata.echo_blob, "a"
.global echo_blob
.global echo_blob_end
echo_blob:
#if VIDEO == PAL
  .incbin "main/echo/prog-z80-pal.bin"
#else
  .incbin "main/echo/prog-z80.bin"
#endif
echo_blob_end:
