/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bram.c
 * @brief Storage for the Backup RAM work buffers
 *
 * @details
 * These buffers are shared with the BIOS across BRAM calls and must therefore
 * exist exactly once in the program. They were previously tentative
 * definitions in bram.h, which gave every translation unit including that
 * header its own copy -- 1,612 bytes of duplicated .bss, and a link failure
 * under -fno-common (the default since GCC 10). See SPEC.md INV-9.
 *
 * Add this file to your project's sources when you use the BRAM API, the same
 * way sub/pcm.s is added for PCM playback:
 *
 *     $(DISC_PATH)/yourmodule.smd: \
 *         sub/bram.c \
 *         ...
 */

#include <sub/bram.h>

/**
 * @var bram_work_buff
 * @brief Scratch buffer used by the BIOS BRAM calls
 * @note After bram_init, this data must not be modified -- it is used by the
 * other BRAM system calls.
 */
u8 bram_work_buff[0x640];

/**
 * @var bram_string_buff
 * @brief Buffer for the BIOS-generated display strings
 */
u8 bram_string_buff[12];

/**
 * @var brmstat_results
 * @brief Results of the most recent bram_stat call
 */
BrmstatRes brmstat_results;

/**
 * @var brmserch_results
 * @brief Results of the most recent bram_search call
 */
BrmserchRes brmserch_results;

/**
 * @var brmread_results
 * @brief Results of the most recent bram_read call
 */
BrmreadRes brmread_results;
