/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sound.def.h
 * @brief Sound commands from the Sub CPU to Echo on the Main CPU, shared by
 * both CPUs, C and assembly
 *
 * @details
 * Echo runs on the Z80, which only the Main CPU can reach. A program whose
 * game logic runs on the Sub CPU sends sound commands with sound_command
 * (sub/sound.h); the Main CPU passes them to Echo with sound_forward
 * (main/sound.h) once per VBLANK and acknowledges each one.
 *
 * A command is one word in the Sub CPU's COMSTAT register SOUND_COMM:
 *
 * | |15|14-8|7-0|
 * |:|:|:|:|
 * | |toggle|operation (SOUND_OP_*)|argument|
 *
 * The toggle flips with every command, so the same command twice is two
 * commands. The Main CPU acknowledges by copying the word to its COMCMD
 * register SOUND_COMM. A command is pending while the two differ. Both
 * registers start at 0.
 */

#pragma once

/**
 * @def SOUND_COMM
 * @brief The comm register pair the commands use: COMSTATn from the Sub CPU,
 * COMCMDn back from the Main CPU (0 to 7)
 * @details Define it before the first include of sound.def.h to use another
 * pair. Both CPUs' programs must agree. The Sub CPU crash handler uses
 * COMSTAT5 to COMSTAT7 after a crash.
 */
#ifndef SOUND_COMM
#define SOUND_COMM 4
#endif

/**
 * @defgroup sound_op Sound command operations
 * @brief The operation in a sound command, and what sound_forward does
 * @{
 */
#define SOUND_OP_PLAY_BGM   1 ///< echo_play_bgm(bgm[argument])
#define SOUND_OP_STOP_BGM   2 ///< echo_stop_bgm()
#define SOUND_OP_PAUSE_BGM  3 ///< echo_pause_bgm()
#define SOUND_OP_RESUME_BGM 4 ///< echo_resume_bgm()
#define SOUND_OP_PLAY_SFX   5 ///< echo_play_sfx(sfx[argument])
#define SOUND_OP_STOP_SFX   6 ///< echo_stop_sfx()
#define SOUND_OP_SET_VOLUME 7 ///< echo_set_volume(argument)
/** @} */

/**
 * @def SOUND_TOGGLE
 * @brief The bit of a command word that flips with every command
 */
#define SOUND_TOGGLE 0x8000
