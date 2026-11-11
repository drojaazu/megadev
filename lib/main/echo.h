/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file echo.h
 * @brief Echo sound driver: music and sound effects on the YM2612 and PSG
 *
 * @details
 * Echo runs on the Z80 and plays streams in its own format (ESF) with FM
 * (EIF), PSG envelope (EEF) and PCM sample (EWF) instruments. This is Echo's
 * own C API with its function names, so Echo's documentation applies; see
 * docs/sound.md for using it with the kit. Echo is by Javier Degirolmo and is
 * distributed under the zlib license (lib/main/echo/LICENSE).
 *
 * The Z80 fetches streams and instruments itself, through its window onto
 * 68000 memory, for as long as they play. On the Mega Drive that window
 * reaches cartridge ROM but not Main Work RAM, so all Echo data must be in
 * cartridge ROM: cartridges and Mode 1 programs. Mega CD disc programs are not
 * supported yet.
 *
 * Every function takes the Z80 bus for a moment with interrupts masked, and
 * may be called from the main program or from an interrupt handler.
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

/** @brief Commands understood by Echo (echo_send_command and friends) */
enum {
  ECHO_CMD_NONE,       ///< No command
  ECHO_CMD_LOADLIST,   ///< Load the instrument list
  ECHO_CMD_PLAYSFX,    ///< Play a sound effect
  ECHO_CMD_STOPSFX,    ///< Stop the sound effect
  ECHO_CMD_PLAYBGM,    ///< Play background music
  ECHO_CMD_STOPBGM,    ///< Stop the background music
  ECHO_CMD_RESUMEBGM,  ///< Resume paused background music
  ECHO_CMD_SETPCMRATE, ///< Set the PCM playback rate
  ECHO_CMD_PAUSEBGM,   ///< Pause the background music
  ECHO_CMD_SETSTEREO,  ///< Switch between stereo and mono
};

/** @brief Status flag: a sound effect is playing */
#define ECHO_STAT_SFX 0x0001
/** @brief Status flag: background music is playing */
#define ECHO_STAT_BGM 0x0002
/** @brief Status flag: PCM playback is in progress */
#define ECHO_STAT_PCM 0x2000
/** @brief Status flag: Echo has not finished the events from echo_play_direct */
#define ECHO_STAT_DIRBUSY 0x4000
/** @brief Status flag: Echo has not taken the last command yet */
#define ECHO_STAT_BUSY 0x8000

/**
 * @brief Upload Echo to the Z80, load the instrument list and start it
 * @param list Pointers to the EIF, EEF and EWF data of each instrument, in
 * instrument number order, ended by NULL. The data must be in cartridge ROM.
 * @details Resets the Z80 and the YM2612. Call once, before any other Echo
 * function.
 */
void echo_init(void const * const * list);

/**
 * @brief Play background music
 * @param esf The ESF stream, in cartridge ROM
 */
void echo_play_bgm(void const * esf);

/** @brief Stop the background music */
void echo_stop_bgm(void);

/** @brief Pause the background music */
void echo_pause_bgm(void);

/** @brief Resume background music paused by echo_pause_bgm */
void echo_resume_bgm(void);

/**
 * @brief Play a sound effect
 * @param esf The ESF stream, in cartridge ROM. It takes the channels it locks
 * from the music until it ends.
 */
void echo_play_sfx(void const * esf);

/** @brief Stop the sound effect */
void echo_stop_sfx(void);

/**
 * @brief Inject events into the music for the next tick
 * @param esf ESF events, ended by 0xFF; copied, so it may be in RAM
 */
void echo_play_direct(void const * esf);

/**
 * @brief Set the global volume of every channel
 * @param vol 0 (quietest) to 255 (loudest); PCM is muted below 64
 */
void echo_set_volume(uint8_t vol);

/**
 * @brief Set the global volume of each channel
 * @param vol_list 16 values: 8 FM (0 loudest, 0x7F quietest), 4 PSG (0 loudest,
 * 0x0F quietest), PCM on (1) or off (0), 3 reserved
 */
void echo_set_volume_ex(uint8_t const * vol_list);

/**
 * @brief Set the PCM playback rate
 * @param rate YM2612 timer A value; the default, 4, plays at about 10.6 kHz
 */
void echo_set_pcm_rate(uint8_t rate);

/**
 * @brief Switch between stereo and mono
 * @param enable Non-zero for stereo, zero for mono
 */
void echo_set_stereo(int enable);

/**
 * @brief Get Echo's status
 * @return ECHO_STAT_* flags; a command still queued is already reflected
 */
uint16_t echo_get_status(void);

/**
 * @brief Get the user flags, which streams set and clear with events 0xFA, 0xFB
 * @return The flags
 */
uint8_t echo_get_flags(void);

/**
 * @brief Set user flags
 * @param flags The flags to set; the others are left as they are
 */
void echo_set_flags(uint8_t flags);

/**
 * @brief Clear user flags
 * @param flags The flags to clear; the others are left as they are
 */
void echo_clear_flags(uint8_t flags);

/**
 * @brief Send a command without a parameter
 * @param cmd An ECHO_CMD_* command
 */
void echo_send_command(uint8_t cmd);

/**
 * @brief Send a command with an address parameter
 * @param cmd An ECHO_CMD_* command
 * @param addr The address, in cartridge ROM
 */
void echo_send_command_addr(uint8_t cmd, void const * addr);

/**
 * @brief Send a command with a byte parameter
 * @param cmd An ECHO_CMD_* command
 * @param byte The parameter
 */
void echo_send_command_byte(uint8_t cmd, uint8_t byte);

/** @brief FM channel volume for each step of echo_set_volume (index vol >> 2) */
extern uint8_t const echo_fm_vol_table[0x40];

/** @brief PSG channel volume for each step of echo_set_volume (index vol >> 2) */
extern uint8_t const echo_psg_vol_table[0x40];

#ifdef __cplusplus
}
#endif
