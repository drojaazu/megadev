/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/echo_blob.s
 * @brief Echo's Z80 program, uploaded by echo_init (main/echo.h)
 *
 * @details
 * main/echo/prog-z80.bin is Echo 1.65's prebuilt Z80 program from
 * sikthehedgehog/echo commit e326c87 (built/prog-z80.bin, 5120 bytes), altered:
 * its PSG frequency table is retuned to the NTSC clock (main/echo/NOTICE).
 * Echo is distributed under the zlib license: main/echo/LICENSE.
 */

.section .rodata.echo_blob, "a"
.global echo_blob
.global echo_blob_end
echo_blob:
  .incbin "main/echo/prog-z80.bin"
echo_blob_end:
