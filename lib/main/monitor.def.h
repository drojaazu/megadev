/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file monitor.def.h
 * @brief Debug monitor protocol constants (main/monitor.h)
 *
 * @details
 * Frames in both directions:
 *
 *     start, length, body..., checksum
 *
 * `start` is MONITOR_START_CMD for host to console and MONITOR_START_REPLY
 * for console to host. `length` counts the body bytes (1 to
 * MONITOR_BODY_MAX). The first body byte is the command, or the status in a
 * reply. `checksum` is the XOR of `length` and every body byte. A frame with a
 * bad checksum or an unknown command is answered with MONITOR_ST_BAD_FRAME or
 * MONITOR_ST_BAD_CMD and an otherwise empty body. Multi-byte values are big
 * endian, as on the 68000.
 */
#pragma once

/** @brief Protocol version reported by HELLO */
#define MONITOR_VERSION 1

/** @brief First byte of a frame from the host to the console */
#define MONITOR_START_CMD 0xA5
/** @brief First byte of a frame from the console to the host */
#define MONITOR_START_REPLY 0x5A

/** @brief Longest body (command or status plus payload) in a frame */
#define MONITOR_BODY_MAX 132

/** @brief Longest payload a READ returns and a WRITE accepts, in bytes */
#define MONITOR_DATA_MAX 128

// Commands, host to console. Body layout after the command byte:
/** @brief none; reply: 'M','D','V', version, target, flags */
#define MONITOR_CMD_HELLO 0x01
/** @brief any bytes; reply: the same bytes */
#define MONITOR_CMD_PING 0x02
/** @brief addr(4), count(1); reply: count bytes */
#define MONITOR_CMD_READ 0x03
/** @brief addr(4), data...; reply: none */
#define MONITOR_CMD_WRITE 0x04
/** @brief addr(4), count(1): count words, word reads; reply: 2*count bytes */
#define MONITOR_CMD_READW 0x05
/** @brief addr(4), words...: word writes; reply: none */
#define MONITOR_CMD_WRITEW 0x06

// Status, console to host
/** @brief Command done; payload as the command describes */
#define MONITOR_ST_OK 0x00
/** @brief checksum or length error */
#define MONITOR_ST_BAD_FRAME 0x01
/** @brief unknown command */
#define MONITOR_ST_BAD_CMD 0x02
/** @brief count too large, odd address for a word access */
#define MONITOR_ST_BAD_ARG 0x03

// HELLO target byte
/** @brief HELLO target: Mega Drive cartridge */
#define MONITOR_TARGET_MEGADRIVE 0x01
/** @brief HELLO target: Mega CD disc program */
#define MONITOR_TARGET_MEGACD 0x02
/** @brief HELLO target: Mode 1 cartridge */
#define MONITOR_TARGET_MEGACD_MODE1 0x03

/** @brief STATUS: flags, PC, vector; reply: flags(1), pc(4), vector(2) */
#define MONITOR_CMD_STATUS 0x07
/** @brief REGS: the stopped CPU state; reply: 80 bytes, CrashState without the stack copy */
#define MONITOR_CMD_REGS 0x08
/** @brief BP_SET: addr(4); reply: mode(1), MONITOR_BP_PATCH or MONITOR_BP_TRACE */
#define MONITOR_CMD_BP_SET 0x09
/** @brief BP_CLEAR: addr(4); reply: none */
#define MONITOR_CMD_BP_CLEAR 0x0A
/** @brief CONT: resume a stopped program; reply: none */
#define MONITOR_CMD_CONT 0x0B
/** @brief STEP: execute one instruction and stop again; reply: none */
#define MONITOR_CMD_STEP 0x0C
/** @brief SETREGS: 80 bytes replacing the stopped CPU state; reply: none */
#define MONITOR_CMD_SETREGS 0x0D
/** @brief READP: addr(4), count(1): read one word port count times; reply: 2*count bytes */
#define MONITOR_CMD_READP 0x0E
/**
 * @brief VDPRD: command(4), count(1): write a VDP read command, then read count
 * words from the data port, atomically; reply: 2*count bytes
 */
#define MONITOR_CMD_VDPRD 0x0F
/** @brief VDPWR: command(4), words...: write a VDP command, then the words to the data port, atomically; reply: none */
#define MONITOR_CMD_VDPWR 0x10
/** @brief HALT: stop a running program at its next instruction; reply: none */
#define MONITOR_CMD_HALT 0x11

/** @brief Status: the program is not stopped (CONT, STEP, REGS need a stop) */
#define MONITOR_ST_NOT_STOPPED 0x04
/** @brief Status: no free breakpoint slot, or address not found */
#define MONITOR_ST_NO_SLOT 0x05

/** @brief STATUS flag: the program is stopped in the monitor */
#define MONITOR_FLAG_STOPPED 0x01
/** @brief STATUS flag: trace mode is on (a breakpoint in ROM is armed) */
#define MONITOR_FLAG_TRACING 0x02

/** @brief Breakpoint by TRAP #15 written over the instruction (code in RAM) */
#define MONITOR_BP_PATCH 1
/** @brief Breakpoint by single-stepping and comparing the PC (code in ROM) */
#define MONITOR_BP_TRACE 2

/** @brief Number of breakpoint slots */
#define MONITOR_BP_SLOTS 8

/** @brief Exception number reported for a breakpoint (TRAP #15) */
#define MONITOR_VECTOR_TRAP 47
/** @brief Exception number reported for a step or trace stop */
#define MONITOR_VECTOR_TRACE 9

/** @brief Receive ring size, power of two */
#define MONITOR_RX_RING 256
