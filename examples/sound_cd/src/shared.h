// Definitions shared by the Main CPU and Sub CPU programs: commands, their
// parameters and the comm registers they use.
//
//   COMCMD0/COMSTAT0  a command from the Main CPU and its acknowledgment
//   COMCMD1           the command's parameter
//   COMCMD2           the buttons held on pad 1, every frame (active high)
//   COMCMD4/COMSTAT4  sound commands from the Sub CPU (SOUND_COMM, sound.def.h)
//   COMSTAT1          the Sub CPU's state, for the Main CPU to show

#ifndef SHARED_H
#define SHARED_H

#define CMD_LOAD_FILE 1 // load file COMCMD1 into Word RAM and hand it over
#define CMD_START     2 // the Main CPU has Echo running: start playing

#define FILE_MAIN 0

// COMSTAT1: what the Sub CPU has the music doing
#define STATE_STOPPED 1
#define STATE_PLAYING 2
#define STATE_PAUSED  3

// The Main CPU's tables for sound_set_tables
#define BGM_SONG   0
#define SFX_BEEP   0
#define SFX_CHIME  1

#endif
