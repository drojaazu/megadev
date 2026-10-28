/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/bram_buffers.c
 * @brief Storage for the Backup RAM work buffers and call results
 */

#include <sub/bram.h>

u8 bram_work_buff[0x640];
u8 bram_string_buff[12];
BrmstatRes brmstat_results;
BrmserchRes brmserch_results;
BrmreadRes brmread_results;
