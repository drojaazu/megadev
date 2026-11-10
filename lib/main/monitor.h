/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file monitor.h
 * @brief Debug monitor over the controller port serial link
 *
 * @details
 * A small command interpreter a host drives over the serial port
 * (main/comm.h): identify the program, read and write memory. The frame format
 * and command set are in monitor.def.h; the host side is `tools/mdmon.py`.
 * `docs/debugging.md` has the rest.
 *
 * Wiring it into a program:
 * - Call monitor_init once after the VDP is up. It configures the port
 *   (init_ext_port) with the receive interrupt enabled.
 * - Enable the external interrupt in VDP mode register 3 (VDP_EXTINT_ENABLE)
 *   and have the level 2 handler call monitor_rx_isr. Bytes arrive faster
 *   than once per frame, and the port holds one, so receive must be in the
 *   interrupt.
 * - Call monitor_poll from the main loop or the VBLANK handler. It parses
 *   whatever complete frames are in the ring and answers them. Replies are
 *   sent with ext_tx, which blocks until the transmitter is free: a 128-byte
 *   read takes about a quarter of a second at 4800 baud.
 *
 * Memory reads and writes are byte accesses unless the word commands are
 * used; the host chooses. Addresses are not checked: reading an unmapped
 * address or writing ROM does whatever the hardware does.
 *
 * Breakpoints and stepping need the TRAP #15 and trace vectors: cartridges
 * include main/monitor_cart.s in init.s (before main/crash_cart.s), Mega CD
 * programs call monitor_install. Both assemble main/monitor_break.s. A
 * breakpoint on code in RAM is a TRAP #15 written over the instruction; on
 * code the CPU cannot write (cartridge ROM) the monitor single-steps the
 * program and compares the PC, which runs it many times slower while armed.
 * While stopped, the monitor loops on the serial ring with the interrupt mask
 * lowered to level 1 so the receive interrupt still arrives; interrupt
 * handlers of the program keep running.
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <main/crash.h>
#include <main/monitor.def.h>
#include <types.h>

/**
 * @brief Configure the serial port and clear the monitor state
 */
void monitor_init(void);

/**
 * @brief Take a received byte into the ring; call from the level 2 handler
 */
void monitor_rx_isr(void);

/**
 * @brief Parse and answer complete frames; call regularly
 * @return number of frames answered
 */
uint16_t monitor_poll(void);

/**
 * @brief Install the TRAP #15 and trace vectors in the system vector table
 * @details Mega CD and Mode 1 programs; cartridges use main/monitor_cart.s.
 */
void monitor_install(void);

/** @brief CPU state at the current stop (CrashState layout) */
extern CrashState monitor_snapshot;

/** @brief Non-zero while the program is stopped in the monitor */
extern volatile uint8_t monitor_is_stopped;

/** @brief Bytes dropped because the ring was full */
extern volatile uint16_t monitor_rx_dropped;

/** @brief Frames answered since monitor_init */
extern volatile uint16_t monitor_frames;

/** @brief Frames rejected (bad checksum, length or command) since monitor_init */
extern volatile uint16_t monitor_errors;

#ifdef __cplusplus
}
#endif
