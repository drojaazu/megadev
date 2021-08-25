/**
 * \file pcm.h
 * \brief PCM audio playback utilities
 */

#ifndef MEGADEV__PCM_H
#define MEGADEV__PCM_H

#include "types.h"

/**
 * \def WAVEBANK
 * \brief Generate the PCM control register setting to select a given
 * wave bank
 * \param wb desired wave bank (0 to 15)
 */
#define WAVEBANK(wb) (0x80 | (wb & 0xf))

/**
 * \def CHANNEL
 * \brief Generate the PCM control register setting to select a given channel
 * \param cb channel id (1 to 8)
 */
#define CHANNEL(cb) (0xc0 | ((cb - 1) & 7))

typedef struct PcmChannelSettings {
  u8 envelope;
  u8 panning;
  u8 frequencyHi;
  u8 frequencyLo;
  u8 loopPointHi;
  u8 loopPointLo;
  u8 start;
} PcmChannelSettings;

static inline void clear_pcm_ram_c() {
  asm volatile(R"(
    move.l a6, -(sp)
    jsr clearPcmRam
    move.l (sp)+, a6)"
               :
               :
               : "d0", "d1", "d2");
};

static inline void config_pcm_channels_c(u8 const ctrlregChannel,
                                         PcmChannelSettings const * settings) {
  register u8 d0_channel asm("d0") = ctrlregChannel;
  register u32 a5_settings asm("a5") = (u32)settings;

  asm volatile(R"(
    move.l a6, -(sp)
    jsr configPcmChannels
    move.l (sp)+, a6)"
               :
               : "d"(d0_channel), "a"(a5_settings));
};

#endif
