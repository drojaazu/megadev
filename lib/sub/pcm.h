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

#define PCM_ENV			 ((volatile u8 *) _PCM_ENV) ///< @copybrief _PCM_ENV
#define PCM_PAN			 ((volatile u8 *) _PCM_PAN) ///< @copybrief _PCM_PAN
#define PCM_FDL			 ((volatile u8 *) _PCM_FDL) ///< @copybrief _PCM_FDL
#define PCM_FDH			 ((volatile u8 *) _PCM_FDH) ///< @copybrief _PCM_FDH
#define PCM_LSL			 ((volatile u8 *) _PCM_LSL) ///< @copybrief _PCM_LSL
#define PCM_LSH			 ((volatile u8 *) _PCM_LSH) ///< @copybrief _PCM_LSH
#define PCM_ST			 ((u8 volatile *) _PCM_ST) ///< @copybrief _PCM_ST
#define PCM_CTRL		 ((volatile u8 *) _PCM_CTRL) ///< @copybrief _PCM_CTRL
#define PCM_CDISABLE ((volatile u8 *) _PCM_CDISABLE) ///< @copybrief _PCM_CDISABLE

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
typedef struct PcmChannelSettings
{
	u8 envelope;    ///< Envelope (volume), 0 to 0xFF
	u8 panning;     ///< Panning: low nibble left, high nibble right
	u8 frequencyLo; ///< Frequency delta, low byte
	u8 frequencyHi; ///< Frequency delta, high byte
	u8 loopPointLo; ///< Loop start address, low byte
	u8 loopPointHi; ///< Loop start address, high byte
	u8 start;       ///< Start address, high byte
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
	u8 const ctrlregChannel, PcmChannelSettings const * settings)
{
	register u8	 d0_channel asm("d0") = ctrlregChannel;
	register u32 a5_settings asm("a5") = (u32) settings;

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
