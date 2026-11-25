/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sound.h
 * @brief Sub CPU: send sound commands to Echo on the Main CPU
 *
 * @details
 * For programs whose game logic runs on the Sub CPU. The Main CPU program
 * must call sound_forward (main/sound.h) every VBLANK; the command word and
 * the comm registers it uses are described in sound.def.h.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <sound.def.h>
#include <sub/gate_arr.def.h>
#include <types.h>

/**
 * @brief The Sub CPU's COMSTAT register that carries sound commands
 */
#define SOUND_SUB_STAT                                                         \
  (*(uint16_t volatile *) (GA_SUB_REG_COMSTAT0 + 2 * SOUND_COMM))

/**
 * @brief The Main CPU's COMCMD register that acknowledges them
 */
#define SOUND_SUB_ACK                                                          \
  (*(uint16_t volatile const *) (GA_SUB_REG_COMCMD0 + 2 * SOUND_COMM))

/**
 * @brief Whether the Main CPU has passed the last command to Echo
 */
static inline bool sound_command_done(void)
{
  return SOUND_SUB_ACK == SOUND_SUB_STAT;
}

/**
 * @brief Send a sound command to the Main CPU
 * @param op The operation, one of SOUND_OP_* (sound.def.h)
 * @param arg Its argument: a BGM or SFX number in the Main CPU's tables, or a
 * volume
 * @details Waits until the previous command has been acknowledged, then
 * returns at once; the Main CPU passes the command to Echo at its next
 * VBLANK. Two commands in a row therefore take at least one frame. Waits
 * forever if the Main CPU program does not call sound_forward.
 */
static inline void sound_command(uint8_t op, uint8_t arg)
{
  uint16_t const prev = SOUND_SUB_STAT;
  while (SOUND_SUB_ACK != prev)
    ;
  SOUND_SUB_STAT = (uint16_t) ((~prev & SOUND_TOGGLE) | ((op & 0x7F) << 8) |
                               arg);
}

#ifdef __cplusplus
}
#endif
