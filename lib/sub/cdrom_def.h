/**
 * \file sub/cdrom_def.h
 * \brief CD-ROM file access definitions
 */

#ifndef MEGADEV__CD_SUB_CDROM_DEF_H
#define MEGADEV__CD_SUB_CDROM_DEF_H

/*
 * Access Operations
 * Place one of these values in `access_op` to trigger the process
 */

/**
 * \def ACC_OP_IDLE
 * \brief No access operation
 * \note This should be set whenever there is no disc IO to be done
 */
#define ACC_OP_IDLE 0

/**
 * \def ACC_OP_LOAD_DIR
 * \brief Load and cache the root directort file listing
 */
#define ACC_OP_LOAD_DIR 1

/**
 * \def ACC_OP_LOAD_CDC_DMA
 * \brief Load file to Word RAM via DMA
 */
#define ACC_OP_LOAD_CDC_DMA 2

/**
 * \def ACC_OP_LOAD_CDC
 * \brief Load file to Sub CPU memory space via CDC copy
 */
#define ACC_OP_LOAD_CDC 3

/**
 * \def ACC_OP_LOAD_PRG_DMA
 * \brief Load file to PRG RAM via DMA
 */
#define ACC_OP_LOAD_PRG_DMA 4

/**
 * \def ACC_OP_LOAD_PCM_DMA
 * \brief Load file to PCM memory via DMA
 */
#define ACC_OP_LOAD_PCM_DMA 5

/*
  CD-ROM Access Operation Result
  These values indicate the final disposition of an access operation
*/

/**
 * \def RESULT_OK
 * \brief No problems during the process
 */
#define RESULT_OK 0x64

/**
 * \def RESULT_LOAD_FAIL
 * \brief Error in core load_data_sub routine
 */
#define RESULT_LOAD_FAIL 0xff9c

/**
 * \def RESULT_DIR_FAIL
 * \brief Error occurred when trying to load file list from directory
 */
#define RESULT_DIR_FAIL 0xffff

/**
 * \def RESULT_NOT_FOUND
 * \brief Filename not found on load attempt
 */
#define RESULT_NOT_FOUND 0xfffe

#endif