/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sound.h
 * @brief Main CPU: pass the Sub CPU's sound commands to Echo
 *
 * @details
 * For programs whose game logic runs on the Sub CPU and sends sound commands
 * with sound_command (sub/sound.h). The Main CPU registers its music and
 * effects with sound_set_tables and calls sound_forward once per VBLANK. The
 * command word and the comm registers it uses are described in sound.def.h.
 * Echo must have been started with echo_init (main/echo.h).
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <sound.def.h>
#include <types.h>

/**
 * @brief Set the music and effects that command arguments number
 * @param bgm Echo streams for SOUND_OP_PLAY_BGM, by number
 * @param bgm_count Entries in bgm
 * @param sfx Echo streams for SOUND_OP_PLAY_SFX, by number
 * @param sfx_count Entries in sfx
 * @details The tables are kept by pointer, not copied. A number past the end
 * of its table is acknowledged and ignored.
 */
void sound_set_tables(void const * const * bgm, uint8_t bgm_count,
  void const * const * sfx, uint8_t sfx_count);

/**
 * @brief Pass a pending sound command from the Sub CPU to Echo
 * @details Call once per VBLANK, from the VBLANK handler or the main loop.
 * Handles at most one command and acknowledges it; does nothing when none is
 * pending.
 */
void sound_forward(void);

#ifdef __cplusplus
}
#endif
