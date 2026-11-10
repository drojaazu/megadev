#!/usr/bin/env python3
"""mdmon.py [-d DEVICE] [-b BAUD] [-t SECONDS] COMMAND ...

Host side of the kit's debug monitor (lib/main/monitor.h) over the controller
port serial link. Standard library only.

Commands:
  hello                      identify the program (version, target)
  ping [TEXT]                echo test
  rd ADDR COUNT [-o FILE]    read COUNT bytes from ADDR (hex dump, or to FILE)
  rdw ADDR COUNT             read COUNT words from ADDR (word accesses)
  rdp ADDR COUNT             read the word port at ADDR COUNT times (VDP data port etc.)
  vram ADDR COUNT            read COUNT words of VRAM through the VDP ports
  cram                       read the 64 CRAM entries
  vramw ADDR HEXWORDS        write words to VRAM through the VDP ports
  wr ADDR HEXBYTES           write bytes, e.g. wr 0xFF0000 01020304
  wrw ADDR HEXWORDS          write words, e.g. wrw 0xFF0000 1234ABCD
  bench                      time a 1 KB read
  status                     stopped?, trace mode?, PC and vector of the stop
  regs                       registers of the stopped program
  bp ADDR                    set a breakpoint (patched in RAM, trace mode in ROM)
  bc ADDR                    clear a breakpoint
  cont                       continue a stopped program
  step                       execute one instruction and stop
  wait [SECONDS]             poll status until the program stops (default 10 s)
  halt                       stop a running program at its next instruction

Frame: start(A5 to console, 5A back), length, body..., xor checksum of length
and body. First body byte is the command (or status in a reply).
Exit: 0 ok, 2 no reply, 3 bad reply, 4 usage.
"""
import os, select, struct, sys, termios, time

START_CMD, START_REPLY = 0xA5, 0x5A
CMD = {'hello': 1, 'ping': 2, 'rd': 3, 'wr': 4, 'rdw': 5, 'wrw': 6, 'status': 7, 'regs': 8, 'bp': 9, 'bc': 10, 'cont': 11, 'step': 12, 'setregs': 13, 'rdp': 14, 'vdprd': 15, 'vdpwr': 16, 'halt': 17}
STATUS = {0: 'ok', 1: 'bad frame', 2: 'bad command', 3: 'bad argument', 4: 'not stopped', 5: 'no breakpoint slot / not found'}
VECTOR = {47: 'breakpoint', 9: 'step/trace'}
TARGET = {1: 'Mega Drive', 2: 'Mega CD', 3: 'Mega CD Mode 1'}
DATA_MAX = 128
BAUDS = {1200: termios.B1200, 2400: termios.B2400, 4800: termios.B4800}

def open_port(dev, baud):
    fd = os.open(dev, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    iflag, oflag, cflag, lflag, _, _, cc = termios.tcgetattr(fd)
    cflag &= ~(termios.PARENB | termios.CSTOPB | termios.CSIZE | termios.CRTSCTS)
    cflag |= termios.CS8 | termios.CREAD | termios.CLOCAL
    lflag &= ~(termios.ICANON | termios.ECHO | termios.ECHOE | termios.ISIG | termios.IEXTEN)
    iflag &= ~(termios.IXON | termios.IXOFF | termios.IXANY | termios.ICRNL | termios.INLCR | termios.IGNCR | termios.BRKINT | termios.INPCK | termios.ISTRIP)
    oflag &= ~termios.OPOST
    cc[termios.VMIN] = 0; cc[termios.VTIME] = 0
    termios.tcsetattr(fd, termios.TCSANOW, [iflag, oflag, cflag, lflag, BAUDS[baud], BAUDS[baud], cc])
    termios.tcflush(fd, termios.TCIOFLUSH)
    return fd

def read_exact(fd, n, timeout):
    buf = b''; end = time.time() + timeout
    while len(buf) < n and time.time() < end:
        r, _, _ = select.select([fd], [], [], 0.05)
        if r:
            try: buf += os.read(fd, n - len(buf))
            except BlockingIOError: pass
    return buf

class Monitor:
    def __init__(self, dev, baud, timeout=3.0):
        self.fd = open_port(dev, baud)
        self.byte_time = 10.0 / baud
        self.timeout = timeout

    def transact(self, cmd, payload=b'', expect=None):
        body = bytes([cmd]) + payload
        if len(body) > 132: raise ValueError('frame too long')
        cks = len(body)
        for b in body: cks ^= b
        os.write(self.fd, bytes([START_CMD, len(body)]) + body + bytes([cks]))
        # reply: start, length, status, payload, checksum. A program under
        # trace mode answers slowly, so allow seconds, and skip any late bytes
        # of an earlier reply that a timeout left behind.
        t = self.timeout + self.byte_time * (len(body) + (expect or 0) + 8) * 2
        end = time.time() + t
        skipped = 0
        while True:
            b = read_exact(self.fd, 1, max(0.0, end - time.time()))
            if not b: self.fail(2, f'no reply within {t:.1f} s' + (f' ({skipped} stray bytes skipped)' if skipped else ''))
            if b[0] == START_REPLY: break
            skipped += 1
            if skipped > 300: self.fail(3, 'no frame start in 300 bytes')
        head = read_exact(self.fd, 2, 1.0)
        if len(head) < 2: self.fail(3, 'short reply header')
        length, status = head[0], head[1]
        rest = read_exact(self.fd, length - 1 + 1, 0.5 + self.byte_time * length * 2)
        if len(rest) < length: self.fail(3, f'short reply: {len(rest)} of {length}')
        data, cks = rest[:-1], rest[-1]
        calc = length ^ status
        for b in data: calc ^= b
        if calc != cks: self.fail(3, 'reply checksum mismatch')
        if status != 0: self.fail(3, f'console says: {STATUS.get(status, status)}')
        return data

    def fail(self, code, msg):
        print('mdmon:', msg, file=sys.stderr); sys.exit(code)

    def guard(self, addr, length):
        # reading the PSG or the Z80-side I/O through the 68000 locks the console
        if addr < 0xC00020 and addr + length > 0xC00010:
            self.fail(4, f'refusing to read {addr:08X}..{addr + length:08X}: 0xC00010-0xC0001F locks the console')

    def read_port(self, addr, count):
        out = b''
        while count > 0:
            n = min(DATA_MAX // 2, count)
            out += self.transact(CMD['rdp'], struct.pack('>IB', addr, n), expect=n * 2)
            count -= n
        return out

    def read(self, addr, count, words=False):
        self.guard(addr, count * (2 if words else 1))
        out = b''
        unit = 2 if words else 1
        step = DATA_MAX // unit
        while count > 0:
            n = min(step, count)
            out += self.transact(CMD['rdw' if words else 'rd'], struct.pack('>IB', addr, n), expect=n * unit)
            addr += n * unit; count -= n
        return out

    def write(self, addr, data, words=False):
        step = DATA_MAX
        while data:
            chunk, data = data[:step], data[step:]
            self.transact(CMD['wrw' if words else 'wr'], struct.pack('>I', addr) + chunk)
            addr += len(chunk)

def status_line(m, d=None):
    if d is None: d = m.transact(CMD['status'], expect=7)
    flags, pc, vec = d[0], struct.unpack('>I', d[1:5])[0], struct.unpack('>H', d[5:7])[0]
    if flags & 1:
        return f'stopped at {pc:08X} ({VECTOR.get(vec, vec)})' + (', trace mode armed' if flags & 2 else '')
    return 'running' + (', trace mode armed' if flags & 2 else '')

def print_regs(d):
    vec, sr, pc, acc, info, ir = struct.unpack('>HHIIHH', d[:16])
    regs = struct.unpack('>16I', d[16:80])
    print(f'PC {pc:08X}  SR {sr:04X}  ({VECTOR.get(vec, vec)})')
    for i in range(8):
        print(f'D{i} {regs[i]:08X}   A{i} {regs[8 + i]:08X}')

def hexdump(addr, data):
    for i in range(0, len(data), 16):
        row = data[i:i + 16]
        print(f'{addr + i:08X}  ' + ' '.join(f'{b:02X}' for b in row).ljust(48) + '  ' +
              ''.join(chr(b) if 32 <= b < 127 else '.' for b in row))

def main():
    args = sys.argv[1:]
    dev = '/dev/ttyUSB0'; baud = 4800; timeout = 3.0
    while args and args[0] in ('-d', '-b', '-t'):
        if args[0] == '-d': dev = args[1]
        elif args[0] == '-t': timeout = float(args[1])
        else: baud = int(args[1])
        args = args[2:]
    if not args: print(__doc__); return 4
    m = Monitor(dev, baud, timeout)
    cmd, rest = args[0], args[1:]
    if cmd == 'hello':
        d = m.transact(CMD['hello'], expect=6)
        if d[:3] != b'MDV': print('unexpected hello payload', d.hex()); return 3
        print(f'monitor v{d[3]}, target {TARGET.get(d[4], d[4])}, flags {d[5]:02X}')
    elif cmd == 'ping':
        text = (rest[0] if rest else 'ping').encode('latin-1')
        d = m.transact(CMD['ping'], text, expect=len(text))
        print('ok' if d == text else f'mismatch: {d!r}')
        return 0 if d == text else 3
    elif cmd in ('rd', 'rdw'):
        addr, count = int(rest[0], 0), int(rest[1], 0)
        out = None
        if len(rest) >= 4 and rest[2] == '-o': out = rest[3]
        t0 = time.time()
        data = m.read(addr, count, words=(cmd == 'rdw'))
        dt = time.time() - t0
        if out:
            open(out, 'wb').write(data); print(f'{len(data)} bytes to {out} in {dt:.2f} s', file=sys.stderr)
        else:
            hexdump(addr, data)
    elif cmd == 'rdp':
        addr, count = int(rest[0], 0), int(rest[1], 0)
        hexdump(addr, m.read_port(addr, count))
    elif cmd in ('vram', 'cram', 'vsram'):
        if cmd == 'vram':
            addr, count = int(rest[0], 0), int(rest[1], 0)
        else:
            addr, count = 0, 64 if cmd == 'cram' else 40
        out = b''
        while count > 0:
            n = min(DATA_MAX // 2, count)
            if cmd == 'vram': command = ((addr & 0x3FFF) << 16) | ((addr >> 14) & 3)
            elif cmd == 'cram': command = (addr << 16) | 0x20
            else: command = (addr << 16) | 0x10
            out += m.transact(CMD['vdprd'], struct.pack('>IB', command, n), expect=n * 2)
            addr += n * 2; count -= n
        hexdump(int(rest[0], 0) if cmd == 'vram' else 0, out)
    elif cmd == 'vramw':
        addr, data = int(rest[0], 0), bytes.fromhex(rest[1])
        command = 0x40000000 | ((addr & 0x3FFF) << 16) | ((addr >> 14) & 3)
        m.transact(CMD['vdpwr'], struct.pack('>I', command) + data); print(f'{len(data)} bytes to VRAM {addr:04X}')
    elif cmd in ('wr', 'wrw'):
        addr, data = int(rest[0], 0), bytes.fromhex(rest[1])
        m.write(addr, data, words=(cmd == 'wrw'))
        print(f'{len(data)} bytes written at {addr:08X}')
    elif cmd == 'status':
        print(status_line(m))
    elif cmd == 'regs':
        print_regs(m.transact(CMD['regs'], expect=80))
    elif cmd == 'bp':
        mode = m.transact(CMD['bp'], struct.pack('>I', int(rest[0], 0)), expect=1)[0]
        print({1: 'patched (TRAP #15 in RAM)', 2: 'trace mode (ROM: single-stepping, program runs slowly)'}.get(mode, mode))
    elif cmd == 'bc':
        m.transact(CMD['bc'], struct.pack('>I', int(rest[0], 0))); print('cleared')
    elif cmd == 'halt':
        m.transact(CMD['halt']); time.sleep(0.3); print(status_line(m))
    elif cmd in ('cont', 'step'):
        m.transact(CMD[cmd]); print(cmd)
        if cmd == 'step':
            print(status_line(m))
    elif cmd == 'wait':
        secs = float(rest[0]) if rest else 10.0
        end = time.time() + secs
        while time.time() < end:
            d = m.transact(CMD['status'], expect=7)
            if d[0] & 1:
                print(status_line(m, d)); return 0
            time.sleep(0.2)
        print('still running'); return 2
    elif cmd == 'bench':
        t0 = time.time(); m.read(0x000000, 1024); dt = time.time() - t0
        print(f'1 KB read in {dt:.2f} s = {1024 / dt:.0f} bytes/s')
    else:
        print(__doc__); return 4
    return 0

if __name__ == '__main__':
    sys.exit(main())
