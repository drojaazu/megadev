/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file cdrom.def.h
 * @brief CD-ROM file access definitions
 */

#ifndef MEGADEV__SUB_CDROM_DEF_H
#define MEGADEV__SUB_CDROM_DEF_H

/*
 * Access Operations
 * Place one of these values in `access_op` to trigger the process
 */

/**
 * @def CDROM_IDLE
 * @brief No access operation
 *
 * @note
 * This should be set whenever there is no disc IO to be done
 */
#define CDROM_IDLE 0

/**
 * @def CDROM_LOAD_FILE_LIST
 * @brief Load and cache the root directort file listing
 */
#define CDROM_LOAD_FILE_LIST 1

/**
 * @def CDROM_LOAD_CDC_DMA
 * @brief Load file to Word RAM via DMA
 */
#define CDROM_LOAD_CDC_DMA 2

/**
 * @def CDROM_LOAD_CDC
 * @brief Load file to Sub CPU memory space via CDC copy
 */
#define CDROM_LOAD_CDC 3

/**
 * @def CDROM_LOAD_PRG_DMA
 * @brief Load file to PRG RAM via DMA
 */
#define CDROM_LOAD_PRG_DMA 4

/**
 * @def CDROM_LOAD_PCM_DMA
 * @brief Load file to PCM memory via DMA
 */
#define CDROM_LOAD_PCM_DMA 5

/*
	CD-ROM Access Operation Result
	These values indicate the final disposition of an access operation
*/

/**
 * @def CDROM_RESULT_OK
 * @brief No problems during the process
 */
#define CDROM_RESULT_OK 0x64

/**
 * @def CDROM_RESULT_LOAD_FAIL
 * @brief Error in core load_data_sub routine
 */
#define CDROM_RESULT_LOAD_FAIL 0xff9c

/**
 * @def CDROM_RESULT_FILE_LIST_FAIL
 * @brief Error occurred when trying to load file list from directory
 */
#define CDROM_RESULT_FILE_LIST_FAIL 0xffff

/**
 * @def CDROM_RESULT_NOT_FOUND
 * @brief Filename not found on load attempt
 */
#define CDROM_RESULT_NOT_FOUND 0xfffe

#endif