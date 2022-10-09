/**
 * @file pcm.h
 * @brief PCM audio playback utilities
 */

#ifndef MEGADEV__PCM_H
#define MEGADEV__PCM_H

#include "types.h"

/**
 * @def WAVEBANK
 * @brief Generate the PCM control register setting to select a given
 * wave bank
 * @param wb desired wave bank (0 to 15)
 */
#define WAVEBANK(wb) (0x80 | (wb & 0xf))

/**
 * @def CHANNEL
 * @brief Generate the PCM control register setting to select a given channel
 * @param cb channel id (1 to 8)
 */
#define CHANNEL(ch) (0xc0 | ((ch - 1) & 7))

/**
 * @struct PcmChannelSettings
 * @brief PCM channel register values
 */
typedef struct PcmChannelSettings
{
	u8 envelope;
	u8 panning;
	u8 frequencyLo;
	u8 frequencyHi;
	u8 loopPointLo;
	u8 loopPointHi;
	u8 start;
} PcmChannelSettings;

/**
 * @fn pcm_clear_ram_c
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
 * @fn pcm_config_channel_c
 * @brief Configure a PCM channel
 * @param ctrlregChannel Channel index
 * @param settings Pointer to PcmChannelSettings
 */
static inline void pcm_config_channel_c (u8 const ctrlregChannel, PcmChannelSettings const * settings)
{
	register u8 d0_channel asm("d0") = ctrlregChannel;
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

#endif
