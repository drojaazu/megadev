/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file runtime/monitor.c
 * @brief Debug monitor over the controller port serial link (main/monitor.h)
 *
 * @details
 * Main CPU only. The receive side is a ring filled by monitor_rx_isr from the
 * level 2 interrupt; monitor_poll consumes it byte by byte through a small
 * state machine and answers each complete frame. Breakpoints and stepping
 * enter through main/monitor_break.s, which calls monitor_trace_hit and
 * monitor_stopped.
 */

#include <cpu.h>
#include <main/comm.h>
#include <main/memmap.def.h>
#include <main/monitor.h>
#include <types.h>

volatile uint16_t monitor_rx_dropped;
volatile uint16_t monitor_frames;
volatile uint16_t monitor_errors;
volatile uint8_t  monitor_is_stopped;

CrashState monitor_snapshot __attribute__((aligned(4)));

static volatile uint8_t rx_ring[MONITOR_RX_RING];
static volatile uint8_t rx_head; // written by the interrupt
static volatile uint8_t rx_tail; // written by monitor_poll

// frame being assembled
static uint8_t body[MONITOR_BODY_MAX];
static uint8_t body_len;  // expected body length
static uint8_t body_have; // bytes of body received so far
static uint8_t checksum;
static enum { WAIT_START, WAIT_LENGTH, IN_BODY, WAIT_CHECKSUM } state;

// breakpoints
// Layout shared with main/monitor_break.s: 8 bytes per slot, mode at offset 6
typedef struct {
  uint32_t addr;
  uint16_t saved; // the instruction word under a patch
  uint8_t  mode;  // 0 free, MONITOR_BP_PATCH, MONITOR_BP_TRACE
  uint8_t  pad;
} Breakpoint;
Breakpoint monitor_bps[MONITOR_BP_SLOTS];
#define bps monitor_bps

// read by the trace entry's fast path (main/monitor_break.s)
volatile uint8_t monitor_trace_armed; // number of trace mode breakpoints
volatile uint8_t monitor_trace_slow;  // a step or a re-arm is pending

#define TRAP15_OPCODE 0x4E4F
#define SR_TRACE      0x8000

// resume bookkeeping
static uint8_t  resume_action; // 0 none, 1 continue, 2 step
static uint8_t  step_pending;  // the next trace stop is a single step
static uint8_t  rearm_pending; // re-patch this breakpoint at the next trace
static uint32_t rearm_addr;

void monitor_init(void)
{
  rx_head = rx_tail = 0;
  monitor_rx_dropped = monitor_frames = monitor_errors = 0;
  monitor_is_stopped = 0;
  state = WAIT_START;
  for (uint8_t i = 0; i < MONITOR_BP_SLOTS; ++i)
    bps[i].mode = 0;
  resume_action = step_pending = rearm_pending = 0;
  monitor_trace_armed = monitor_trace_slow = 0;
  init_ext_port();
}

void monitor_install(void)
{
  extern void monitor_trap_entry(void);
  extern void monitor_trace_entry(void);
  *(void (**)(void)) EXVEC_TRAPF = monitor_trap_entry;
  *(void (**)(void)) EXVEC_TRACE = monitor_trace_entry;
}

void monitor_rx_isr(void)
{
  uint8_t const status = ext_status();
  if (! (status & SCTRL_RX_READY_MASK))
    return;
  uint8_t const c = ext_read_byte();
  uint8_t const next = (uint8_t) ((rx_head + 1) & (MONITOR_RX_RING - 1));
  if (next == rx_tail) {
    ++monitor_rx_dropped;
    return;
  }
  rx_ring[rx_head] = c;
  rx_head = next;
}

// ---- replies ---------------------------------------------------------------

static void reply_begin(uint8_t status, uint8_t payload_len)
{
  uint8_t const len = (uint8_t) (payload_len + 1);
  ext_tx(MONITOR_START_REPLY);
  ext_tx(len);
  ext_tx(status);
  checksum = (uint8_t) (len ^ status);
}

static void reply_byte(uint8_t b)
{
  ext_tx(b);
  checksum ^= b;
}

static void reply_long(uint32_t v)
{
  reply_byte((uint8_t) (v >> 24));
  reply_byte((uint8_t) (v >> 16));
  reply_byte((uint8_t) (v >> 8));
  reply_byte((uint8_t) v);
}

static void reply_end(void)
{
  ext_tx(checksum);
}

static void reply_status(uint8_t status)
{
  reply_begin(status, 0);
  reply_end();
  if (status != MONITOR_ST_OK)
    ++monitor_errors;
}

static uint32_t read_long(uint8_t const * p)
{
  return ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
         ((uint32_t) p[2] << 8) | p[3];
}

// ---- breakpoints -----------------------------------------------------------

static Breakpoint * bp_find(uint32_t addr)
{
  for (uint8_t i = 0; i < MONITOR_BP_SLOTS; ++i)
    if (bps[i].mode && bps[i].addr == addr)
      return &bps[i];
  return 0;
}

static bool trace_needed(void)
{
  return monitor_trace_armed != 0;
}

static void count_trace_bps(void)
{
  uint8_t n = 0;
  for (uint8_t i = 0; i < MONITOR_BP_SLOTS; ++i)
    if (bps[i].mode == MONITOR_BP_TRACE)
      ++n;
  monitor_trace_armed = n;
}

static void set_slow(void)
{
  monitor_trace_slow = (uint8_t) (step_pending | rearm_pending);
}

/**
 * Set a breakpoint: patch if the word can be written, else trace mode
 * @return the mode, or 0 when no slot is free
 */
static uint8_t bp_set(uint32_t addr)
{
  if (addr & 1)
    return 0;
  Breakpoint * bp = bp_find(addr);
  if (bp)
    return bp->mode;
  for (uint8_t i = 0; i < MONITOR_BP_SLOTS; ++i) {
    if (bps[i].mode)
      continue;
    bp = &bps[i];
    uint16_t volatile * p = (uint16_t volatile *) addr;
    uint16_t const      saved = *p;
    *p = TRAP15_OPCODE;
    if (*p == TRAP15_OPCODE) {
      bp->saved = saved;
      bp->mode = MONITOR_BP_PATCH;
    } else {
      bp->mode = MONITOR_BP_TRACE;
    }
    bp->addr = addr;
    count_trace_bps();
    return bp->mode;
  }
  return 0;
}

static bool bp_clear(uint32_t addr)
{
  Breakpoint * bp = bp_find(addr);
  if (! bp)
    return false;
  // restore the instruction only if the trap is still there: a module loaded
  // over a patched breakpoint owns those bytes now
  if (bp->mode == MONITOR_BP_PATCH) {
    uint16_t volatile * p = (uint16_t volatile *) addr;
    if (*p == TRAP15_OPCODE)
      *p = bp->saved;
  }
  bp->mode = 0;
  if (rearm_pending && rearm_addr == addr) {
    rearm_pending = 0;
    set_slow();
  }
  count_trace_bps();
  return true;
}

// ---- commands --------------------------------------------------------------

static void handle_frame(void)
{
  uint8_t const cmd = body[0];
  uint8_t const args = (uint8_t) (body_len - 1);

  switch (cmd) {
    case MONITOR_CMD_HELLO: {
      reply_begin(MONITOR_ST_OK, 6);
      reply_byte('M');
      reply_byte('D');
      reply_byte('V');
      reply_byte(MONITOR_VERSION);
#if TARGET == MEGADRIVE
      reply_byte(MONITOR_TARGET_MEGADRIVE);
#elif TARGET == MEGACD_MODE1
      reply_byte(MONITOR_TARGET_MEGACD_MODE1);
#else
      reply_byte(MONITOR_TARGET_MEGACD);
#endif
      reply_byte(monitor_is_stopped ? MONITOR_FLAG_STOPPED : 0);
      reply_end();
      break;
    }

    case MONITOR_CMD_PING: {
      reply_begin(MONITOR_ST_OK, args);
      for (uint8_t i = 0; i < args; ++i)
        reply_byte(body[1 + i]);
      reply_end();
      break;
    }

    case MONITOR_CMD_READ:
    case MONITOR_CMD_READW: {
      if (args != 5) {
        reply_status(MONITOR_ST_BAD_FRAME);
        return;
      }
      uint32_t const addr = read_long(&body[1]);
      uint8_t const  count = body[5];
      if (cmd == MONITOR_CMD_READ) {
        if (count > MONITOR_DATA_MAX) {
          reply_status(MONITOR_ST_BAD_ARG);
          return;
        }
        uint8_t const volatile * p = (uint8_t const volatile *) addr;
        reply_begin(MONITOR_ST_OK, count);
        for (uint8_t i = 0; i < count; ++i)
          reply_byte(p[i]);
      } else {
        if (count > MONITOR_DATA_MAX / 2 || (addr & 1)) {
          reply_status(MONITOR_ST_BAD_ARG);
          return;
        }
        uint16_t const volatile * p = (uint16_t const volatile *) addr;
        reply_begin(MONITOR_ST_OK, (uint8_t) (count * 2));
        for (uint8_t i = 0; i < count; ++i) {
          uint16_t const w = p[i];
          reply_byte((uint8_t) (w >> 8));
          reply_byte((uint8_t) w);
        }
      }
      reply_end();
      break;
    }

    case MONITOR_CMD_WRITE:
    case MONITOR_CMD_WRITEW: {
      if (args < 5) {
        reply_status(MONITOR_ST_BAD_FRAME);
        return;
      }
      uint32_t const addr = read_long(&body[1]);
      uint8_t const  count = (uint8_t) (args - 4);
      if (cmd == MONITOR_CMD_WRITE) {
        uint8_t volatile * p = (uint8_t volatile *) addr;
        for (uint8_t i = 0; i < count; ++i)
          p[i] = body[5 + i];
      } else {
        if ((count & 1) || (addr & 1)) {
          reply_status(MONITOR_ST_BAD_ARG);
          return;
        }
        uint16_t volatile * p = (uint16_t volatile *) addr;
        for (uint8_t i = 0; i < count / 2; ++i)
          p[i] = (uint16_t) ((body[5 + 2 * i] << 8) | body[6 + 2 * i]);
      }
      reply_status(MONITOR_ST_OK);
      break;
    }

    case MONITOR_CMD_READP: {
      // the same word address read repeatedly: VDP data port after a read
      // command, or any port with a sequence behind it
      if (args != 5) {
        reply_status(MONITOR_ST_BAD_FRAME);
        return;
      }
      uint32_t const addr = read_long(&body[1]);
      uint8_t const  count = body[5];
      if (count > MONITOR_DATA_MAX / 2 || (addr & 1)) {
        reply_status(MONITOR_ST_BAD_ARG);
        return;
      }
      uint16_t const volatile * p = (uint16_t const volatile *) addr;
      reply_begin(MONITOR_ST_OK, (uint8_t) (count * 2));
      for (uint8_t i = 0; i < count; ++i) {
        uint16_t const w = *p;
        reply_byte((uint8_t) (w >> 8));
        reply_byte((uint8_t) w);
      }
      reply_end();
      break;
    }

    case MONITOR_CMD_VDPRD: {
      // VRAM, CRAM or VSRAM through the ports. The command write and the data
      // reads must not be interleaved with the program's own VDP accesses:
      // reading the data port while a write command is set locks the 68000.
      if (args != 5) {
        reply_status(MONITOR_ST_BAD_FRAME);
        return;
      }
      uint32_t const command = read_long(&body[1]);
      uint8_t const  count = body[5];
      if (
        count > MONITOR_DATA_MAX / 2 || (command & 0x40000000) ||
        (command & 0x00000080)) {
        reply_status(MONITOR_ST_BAD_ARG); // not a read command
        return;
      }
      uint16_t       buf[MONITOR_DATA_MAX / 2];
      uint16_t const sr = mask_interrupts();
      *(uint32_t volatile *) 0xC00004 = command;
      for (uint8_t i = 0; i < count; ++i)
        buf[i] = *(uint16_t const volatile *) 0xC00000;
      restore_interrupts(sr);
      reply_begin(MONITOR_ST_OK, (uint8_t) (count * 2));
      for (uint8_t i = 0; i < count; ++i) {
        reply_byte((uint8_t) (buf[i] >> 8));
        reply_byte((uint8_t) buf[i]);
      }
      reply_end();
      break;
    }

    case MONITOR_CMD_VDPWR: {
      if (args < 4 || ((args - 4) & 1)) {
        reply_status(MONITOR_ST_BAD_FRAME);
        return;
      }
      uint32_t const command = read_long(&body[1]);
      uint8_t const  words = (uint8_t) ((args - 4) / 2);
      uint16_t const sr = mask_interrupts();
      *(uint32_t volatile *) 0xC00004 = command;
      for (uint8_t i = 0; i < words; ++i)
        *(uint16_t volatile *) 0xC00000 =
          (uint16_t) ((body[5 + 2 * i] << 8) | body[6 + 2 * i]);
      restore_interrupts(sr);
      reply_status(MONITOR_ST_OK);
      break;
    }

    case MONITOR_CMD_STATUS: {
      uint8_t flags = monitor_is_stopped ? MONITOR_FLAG_STOPPED : 0;
      if (trace_needed())
        flags |= MONITOR_FLAG_TRACING;
      reply_begin(MONITOR_ST_OK, 7);
      reply_byte(flags);
      reply_long(monitor_is_stopped ? monitor_snapshot.pc : 0);
      reply_byte((uint8_t) (monitor_snapshot.vector >> 8));
      reply_byte((uint8_t) monitor_snapshot.vector);
      reply_end();
      break;
    }

    case MONITOR_CMD_REGS: {
      if (! monitor_is_stopped) {
        reply_status(MONITOR_ST_NOT_STOPPED);
        return;
      }
      uint8_t const * p = (uint8_t const *) &monitor_snapshot;
      reply_begin(MONITOR_ST_OK, CRASH_STACK);
      for (uint8_t i = 0; i < CRASH_STACK; ++i)
        reply_byte(p[i]);
      reply_end();
      break;
    }

    case MONITOR_CMD_SETREGS: {
      if (! monitor_is_stopped) {
        reply_status(MONITOR_ST_NOT_STOPPED);
        return;
      }
      if (args != CRASH_STACK) {
        reply_status(MONITOR_ST_BAD_FRAME);
        return;
      }
      uint8_t * p = (uint8_t *) &monitor_snapshot;
      for (uint8_t i = 0; i < CRASH_STACK; ++i)
        p[i] = body[1 + i];
      reply_status(MONITOR_ST_OK);
      break;
    }

    case MONITOR_CMD_BP_SET: {
      if (args != 4) {
        reply_status(MONITOR_ST_BAD_FRAME);
        return;
      }
      uint8_t const mode = bp_set(read_long(&body[1]));
      if (! mode) {
        reply_status(MONITOR_ST_NO_SLOT);
        return;
      }
      reply_begin(MONITOR_ST_OK, 1);
      reply_byte(mode);
      reply_end();
      // a trace mode breakpoint armed while running: start tracing now; a
      // stop would set the bit on resume, but there is no stop to resume from
      if (mode == MONITOR_BP_TRACE && ! monitor_is_stopped)
        asm volatile("ori.w #0x8000, %%sr" ::: "cc");
      break;
    }

    case MONITOR_CMD_BP_CLEAR: {
      if (args != 4) {
        reply_status(MONITOR_ST_BAD_FRAME);
        return;
      }
      reply_status(
        bp_clear(read_long(&body[1])) ? MONITOR_ST_OK : MONITOR_ST_NO_SLOT);
      break;
    }

    case MONITOR_CMD_HALT: {
      if (monitor_is_stopped) {
        reply_status(MONITOR_ST_OK);
        return;
      }
      reply_status(MONITOR_ST_OK);
      // trace the next instruction of the interrupted program and stop there
      step_pending = 1;
      set_slow();
      asm volatile("ori.w #0x8000, %%sr" ::: "cc");
      break;
    }

    case MONITOR_CMD_CONT:
    case MONITOR_CMD_STEP: {
      if (! monitor_is_stopped) {
        reply_status(MONITOR_ST_NOT_STOPPED);
        return;
      }
      resume_action = (cmd == MONITOR_CMD_CONT) ? 1 : 2;
      reply_status(MONITOR_ST_OK);
      break;
    }

    default:
      reply_status(MONITOR_ST_BAD_CMD);
      return;
  }
  ++monitor_frames;
}

uint16_t monitor_poll(void)
{
  uint16_t answered = 0;
  while (rx_tail != rx_head) {
    uint8_t const c = rx_ring[rx_tail];
    rx_tail = (uint8_t) ((rx_tail + 1) & (MONITOR_RX_RING - 1));

    switch (state) {
      case WAIT_START:
        if (c == MONITOR_START_CMD)
          state = WAIT_LENGTH;
        break;

      case WAIT_LENGTH:
        if (c == 0 || c > MONITOR_BODY_MAX) {
          ++monitor_errors;
          state = WAIT_START;
          break;
        }
        body_len = c;
        body_have = 0;
        checksum = c;
        state = IN_BODY;
        break;

      case IN_BODY:
        body[body_have++] = c;
        checksum ^= c;
        if (body_have == body_len)
          state = WAIT_CHECKSUM;
        break;

      case WAIT_CHECKSUM:
        state = WAIT_START;
        if (c != checksum) {
          reply_status(MONITOR_ST_BAD_FRAME);
          break;
        }
        handle_frame();
        ++answered;
        break;
    }
  }
  return answered;
}

// ---- stops, from monitor_break.s -----------------------------------------

/**
 * Called on every trace exception with the state in monitor_snapshot
 * @return non-zero to stop in the monitor, zero to resume at once
 */
uint16_t monitor_trace_hit(void)
{
  if (rearm_pending) {
    Breakpoint * bp = bp_find(rearm_addr);
    if (bp && bp->mode == MONITOR_BP_PATCH)
      *(uint16_t volatile *) rearm_addr = TRAP15_OPCODE;
    rearm_pending = 0;
    set_slow();
  }
  if (step_pending) {
    step_pending = 0;
    set_slow();
    return 1;
  }
  Breakpoint const * bp = bp_find(monitor_snapshot.pc);
  if (bp && bp->mode == MONITOR_BP_TRACE)
    return 1;
  // keep tracing only while a trace-mode breakpoint is armed
  if (! trace_needed())
    monitor_snapshot.sr &= (uint16_t) ~SR_TRACE;
  return 0;
}

/**
 * Command loop while stopped; returns when the host continues or steps
 */
void monitor_stopped(void)
{
  // a patched breakpoint traps with the PC past the TRAP word
  if (monitor_snapshot.vector == MONITOR_VECTOR_TRAP) {
    Breakpoint const * bp = bp_find(monitor_snapshot.pc - 2);
    if (bp && bp->mode == MONITOR_BP_PATCH)
      monitor_snapshot.pc -= 2;
  }
  monitor_is_stopped = 1;
  resume_action = 0;

  // let the serial receive interrupt (level 2) through even when stopped
  // inside a higher level handler
  uint16_t const sr = mask_interrupts();
  restore_interrupts((uint16_t) ((sr & ~0x0700) | 0x0100));
  while (! resume_action)
    monitor_poll();
  restore_interrupts(sr);

  // leaving: a patched breakpoint at the resume address is lifted for one
  // instruction and re-armed at the trace that follows
  Breakpoint const * bp = bp_find(monitor_snapshot.pc);
  bool               trace = trace_needed();
  if (bp && bp->mode == MONITOR_BP_PATCH) {
    *(uint16_t volatile *) bp->addr = bp->saved;
    rearm_pending = 1;
    rearm_addr = bp->addr;
    trace = true;
  }
  if (resume_action == 2) {
    step_pending = 1;
    trace = true;
  }
  set_slow();
  if (trace)
    monitor_snapshot.sr |= SR_TRACE;
  else
    monitor_snapshot.sr &= (uint16_t) ~SR_TRACE;
  monitor_is_stopped = 0;
}
