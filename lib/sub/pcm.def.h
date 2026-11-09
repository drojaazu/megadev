/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file pcm.def.h
 * @brief PCM audio definitions
 */

#pragma once

#define PCMREG_ENVELOPE 0xFF0001 ///< Envelope (volume) of the selected channel
#define PCMREG_PAN \
  0xFF0003 ///< Panning of the selected channel: low nibble left, high nibble right
#define PCMREG_FADE_LO 0xFF0005 ///< Frequency delta, low byte
#define PCMREG_FADE_HI 0xFF0007 ///< Frequency delta, high byte
#define PCMREG_LOOP_LO 0xFF0009 ///< Loop start address, low byte
#define PCMREG_LOOP_HI 0xFF000B ///< Loop start address, high byte
#define PCMREG_START 0xFF000D ///< Start address, high byte (the low byte is 0)
#define PCMREG_CTRL \
  0xFF000F ///< Control: bit 7 sounding, bit 6 selects channel (1) or wave bank (0), low bits the channel or bank
#define PCMREG_CH_DISABLE                                    \
  0xFF0011 ///< Channel on/off, one bit per channel, 1 = off

#define PCM_RAM                                                       \
  0xFF2000 ///< Wave RAM window: the selected 4 KB bank, on odd bytes

#define PCM_PLAY_CH1_LO                                                  \
  0xFF0021 ///< Channel 1 current playback address, low byte (read only)
#define PCM_PLAY_CH1_HI                                                   \
  0xFF0023 ///< Channel 1 current playback address, high byte (read only)
#define PCM_PLAY_CH2_LO                                                  \
  0xFF0025 ///< Channel 2 current playback address, low byte (read only)
#define PCM_PLAY_CH2_HI                                                   \
  0xFF0027 ///< Channel 2 current playback address, high byte (read only)
#define PCM_PLAY_CH3_LO                                                  \
  0xFF0029 ///< Channel 3 current playback address, low byte (read only)
#define PCM_PLAY_CH3_HI                                                   \
  0xFF002B ///< Channel 3 current playback address, high byte (read only)
#define PCM_PLAY_CH4_LO                                                  \
  0xFF002D ///< Channel 4 current playback address, low byte (read only)
#define PCM_PLAY_CH4_HI                                                   \
  0xFF002F ///< Channel 4 current playback address, high byte (read only)
#define PCM_PLAY_CH5_LO                                                  \
  0xFF0031 ///< Channel 5 current playback address, low byte (read only)
#define PCM_PLAY_CH5_HI                                                   \
  0xFF0033 ///< Channel 5 current playback address, high byte (read only)
#define PCM_PLAY_CH6_LO                                                  \
  0xFF0035 ///< Channel 6 current playback address, low byte (read only)
#define PCM_PLAY_CH6_HI                                                   \
  0xFF0037 ///< Channel 6 current playback address, high byte (read only)
#define PCM_PLAY_CH7_LO                                                  \
  0xFF0039 ///< Channel 7 current playback address, low byte (read only)
#define PCM_PLAY_CH7_HI                                                   \
  0xFF003B ///< Channel 7 current playback address, high byte (read only)
#define PCM_PLAY_CH8_LO                                                  \
  0xFF003D ///< Channel 8 current playback address, low byte (read only)
#define PCM_PLAY_CH8_HI                                                   \
  0xFF003F ///< Channel 8 current playback address, high byte (read only)
