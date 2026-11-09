/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/bram_buffers.c
 * @brief Storage for the Backup RAM work buffers and call results
 */

#include <sub/bram.h>

u8               bram_work_buff[0x640];
u8               bram_string_buff[12];
BramStatusResult bram_status_result;
BramSearchResult bram_search_result;
BramReadResult   bram_read_result;
