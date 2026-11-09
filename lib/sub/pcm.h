/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file pcm.h
 * @brief PCM audio playback utilities
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "pcm.def.h"
#include "types.h"

#define pcmreg_envelope                                                  \
  ((volatile uint8_t *) PCMREG_ENVELOPE) ///< @copybrief PCMREG_ENVELOPE
#define pcmreg_pan ((volatile uint8_t *) PCMREG_PAN) ///< @copybrief PCMREG_PAN
#define pcmreg_fade_lo                                                 \
  ((volatile uint8_t *) PCMREG_FADE_LO) ///< @copybrief PCMREG_FADE_LO
#define pcmreg_fade_hi                                                 \
  ((volatile uint8_t *) PCMREG_FADE_HI) ///< @copybrief PCMREG_FADE_HI
#define pcmreg_loop_lo                                                 \
  ((volatile uint8_t *) PCMREG_LOOP_LO) ///< @copybrief PCMREG_LOOP_LO
#define pcmreg_loop_hi                                                 \
  ((volatile uint8_t *) PCMREG_LOOP_HI) ///< @copybrief PCMREG_LOOP_HI
#define pcmreg_start                                               \
  ((uint8_t volatile *) PCMREG_START) ///< @copybrief PCMREG_START
#define pcmreg_ctrl                                              \
  ((volatile uint8_t *) PCMREG_CTRL) ///< @copybrief PCMREG_CTRL
#define pcmreg_ch_disable                                                    \
  ((volatile uint8_t *) PCMREG_CH_DISABLE) ///< @copybrief PCMREG_CH_DISABLE

/**
 * @def WAVEBANK
 * @brief Generate the PCM control register setting to select a given
 * wave bank
 * @param wb desired wave bank (0 to 15)
 */
#define WAVEBANK(wb) (0x80 | (wb & 0xF))

/**
 * @def CHANNEL
 * @brief Generate the PCM control register setting to select a given channel
 * @param ch channel id (1 to 8)
 */
#define CHANNEL(ch) (0xC0 | ((ch - 1) & 7))

/**
 * @struct PcmChannelSettings
 * @brief PCM channel register values
 */
typedef struct PcmChannelSettings {
  uint8_t envelope;      ///< Envelope (volume), 0 to 0xFF
  uint8_t panning;       ///< Panning: low nibble left, high nibble right
  uint8_t freq_lo;       ///< Frequency delta, low byte
  uint8_t freq_hi;       ///< Frequency delta, high byte
  uint8_t loop_point_lo; ///< Loop start address, low byte
  uint8_t loop_point_hi; ///< Loop start address, high byte
  uint8_t start_point;   ///< Start address, high byte
} PcmChannelSettings;

/**
 * @brief Clear all PCM RAM
 */
static inline void pcm_clear_ram_c()
{
  asm volatile(
    "\
		move.l a6, -(sp) \n\
		jsr pcm_clear_ram \n\
		move.l (sp)+, a6 \n\
		"
    :
    :
    : "d0", "d1", "d2");
};

/**
 * @brief Configure a PCM channel
 * @param ctrlregChannel Channel index
 * @param settings Pointer to PcmChannelSettings
 */
static inline void pcm_config_channel_c(
  uint8_t const ctrlregChannel, PcmChannelSettings const * settings)
{
  register uint8_t  d0_channel asm("d0") = ctrlregChannel;
  register uint32_t a5_settings asm("a5") = (uint32_t) settings;

  asm volatile(
    "\
		move.l a6, -(sp) \n\
		jsr pcm_config_channel \n\
		move.l (sp)+, a6 \n\
		"
    : "+d"(d0_channel), "+a"(a5_settings)
    : "d"(d0_channel), "a"(a5_settings));
};

#ifdef __cplusplus
}
#endif
