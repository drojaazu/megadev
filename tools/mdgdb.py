#!/usr/bin/env python3
"""mdgdb.py [-d DEVICE] [-b BAUD] [-p PORT] [-v]

GDB remote serial protocol server for the kit's debug monitor (main/monitor.h).
Listens on TCP PORT (default 1234), one client at a time, and translates GDB
packets into monitor frames over the controller port serial link. Standard
library only; uses mdmon.py from the same directory.

    m68k gdb: (gdb) target remote :1234

Supported: g G p P m M, c s (and vCont c/s), Z0 z0 Z1 z1 (software breakpoints
via the monitor: patched in RAM, trace mode in ROM), ? k D, Ctrl-C while
running (monitor HALT), qSupported, qAttached, qC, qfThreadInfo, target
description with the plain 68000 register set (no FPU), QStartNoAckMode.
Memory and register reads while the program runs are served live.
"""
import os, select, socket, struct, sys, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mdmon
from mdmon import CMD, Monitor

TARGET_XML = b'''<?xml version="1.0"?>
<!DOCTYPE target SYSTEM "gdb-target.dtd">
<target version="1.0">
  <architecture>m68k</architecture>
  <feature name="org.gnu.gdb.m68k.core">
    <reg name="d0" bitsize="32"/><reg name="d1" bitsize="32"/>
    <reg name="d2" bitsize="32"/><reg name="d3" bitsize="32"/>
    <reg name="d4" bitsize="32"/><reg name="d5" bitsize="32"/>
    <reg name="d6" bitsize="32"/><reg name="d7" bitsize="32"/>
    <reg name="a0" bitsize="32" type="data_ptr"/><reg name="a1" bitsize="32" type="data_ptr"/>
    <reg name="a2" bitsize="32" type="data_ptr"/><reg name="a3" bitsize="32" type="data_ptr"/>
    <reg name="a4" bitsize="32" type="data_ptr"/><reg name="a5" bitsize="32" type="data_ptr"/>
    <reg name="fp" bitsize="32" type="data_ptr"/><reg name="sp" bitsize="32" type="data_ptr"/>
    <reg name="ps" bitsize="32"/><reg name="pc" bitsize="32" type="code_ptr"/>
  </feature>
</target>
'''

def log(*a): print(*a, file=sys.stderr, flush=True)

class Session:
    def __init__(self, mon, conn, verbose):
        self.mon, self.conn, self.verbose = mon, conn, verbose
        self.noack = False
        self.buf = b''
        # A GDB built without expat never fetches target.xml and assumes its
        # default m68k layout with an FPU: d0-d7 a0-a7 ps pc, then fp0-fp7 as
        # 96-bit values and fpcontrol fpstatus fpiaddr. Pad with zeros then.
        self.tdesc_sent = False

    # ---- packet layer --------------------------------------------------------
    def recv_packet(self, timeout=None):
        """Return the next packet payload, b'\\x03' for an interrupt, or None on close."""
        end = None if timeout is None else time.time() + timeout
        while True:
            # acks and naks arrive only between packets: drop them there, never
            # inside a body (a '+' is legal inside, e.g. "swbreak+")
            while self.buf[:1] in (b'+', b'-'):
                self.buf = self.buf[1:]
            i = self.buf.find(b'$')
            if self.buf[:1] == b'\x03':
                self.buf = self.buf[1:]; return b'\x03'
            if i >= 0:
                j = self.buf.find(b'#', i)
                if j >= 0 and len(self.buf) >= j + 3:
                    body, cks = self.buf[i + 1:j], self.buf[j + 1:j + 3]
                    self.buf = self.buf[j + 3:]
                    ok = int(cks, 16) == sum(body) & 0xFF
                    if not self.noack: self.conn.sendall(b'+' if ok else b'-')
                    if ok:
                        if self.verbose: log('<-', body[:80])
                        return self.unescape(body)
                    continue
            if b'\x03' in self.buf and i < 0:
                k = self.buf.find(b'\x03'); self.buf = self.buf[k + 1:]; return b'\x03'
            if end is not None and time.time() >= end: return b''
            r, _, _ = select.select([self.conn], [], [], 0.1 if end else 1.0)
            if not r: continue
            data = self.conn.recv(4096)
            if not data: return None
            self.buf += data

    @staticmethod
    def unescape(body):
        out = bytearray(); i = 0
        while i < len(body):
            if body[i] == 0x7D: out.append(body[i + 1] ^ 0x20); i += 2
            else: out.append(body[i]); i += 1
        return bytes(out)

    def send(self, payload):
        if isinstance(payload, str): payload = payload.encode()
        pkt = b'$' + payload + b'#' + f'{sum(payload) & 0xFF:02x}'.encode()
        if self.verbose: log('->', payload[:80])
        for _ in range(5):
            self.conn.sendall(pkt)
            if self.noack: return
            a = self.conn.recv(1)
            if a in (b'+', b''): return

    # ---- target state ----------------------------------------------------------
    def status(self):
        d = self.mon.transact(CMD['status'], expect=7)
        return d[0], struct.unpack('>I', d[1:5])[0]

    def regs(self):
        d = self.mon.transact(CMD['regs'], expect=80)
        vec, sr, pc = struct.unpack('>HHI', d[:8])
        da = struct.unpack('>16I', d[16:80])
        return list(da) + [sr, pc], d

    def set_regs(self, values, raw):
        # values: d0-d7, a0-a7, ps, pc
        new = bytearray(raw)
        struct.pack_into('>H', new, 2, values[16] & 0xFFFF)
        struct.pack_into('>I', new, 4, values[17])
        struct.pack_into('>16I', new, 16, *values[:16])
        self.mon.transact(CMD['setregs'], bytes(new))

    def stop_reply(self):
        flags, pc = self.status()
        return 'S05'

    def wait_stop(self):
        """Continue has been issued: poll until stopped, or the client sends Ctrl-C."""
        while True:
            flags, pc = self.status()
            if flags & 1:
                return self.stop_reply()
            p = self.recv_packet(timeout=0.15)
            if p == b'\x03':
                self.mon.transact(CMD['halt'])
                for _ in range(40):
                    time.sleep(0.1)
                    flags, pc = self.status()
                    if flags & 1: break
                return self.stop_reply()
            if p is None:
                return None

    # ---- commands ----------------------------------------------------------------
    def handle(self, p):
        c = p[:1]
        if p.startswith(b'qSupported'):
            self.send('PacketSize=400;QStartNoAckMode+;qXfer:features:read+;swbreak+;hwbreak+')
        elif p == b'QStartNoAckMode':
            self.send('OK'); self.noack = True
        elif p.startswith(b'qXfer:features:read:target.xml:'):
            off, ln = (int(x, 16) for x in p.split(b':')[-1].split(b','))
            chunk = TARGET_XML[off:off + ln]
            self.tdesc_sent = True
            self.send(b'l' + chunk if off + ln >= len(TARGET_XML) else b'm' + chunk)
        elif p == b'qAttached': self.send('1')
        elif p == b'qC': self.send('QC1')
        elif p == b'qfThreadInfo': self.send('m1')
        elif p == b'qsThreadInfo': self.send('l')
        elif p == b'qTStatus' or p.startswith(b'qSymbol') or p == b'qOffsets' or p.startswith(b'qL') or p == b'vMustReplyEmpty':
            self.send('')
        elif c == b'H': self.send('OK')
        elif p == b'?':
            flags, pc = self.status()
            if not flags & 1:
                # GDB attaches to a running program: stop it first
                self.mon.transact(CMD['halt'])
                for _ in range(40):
                    time.sleep(0.1)
                    flags, pc = self.status()
                    if flags & 1: break
            self.send('S05')
        elif p == b'g':
            flags, pc = self.status()
            if not flags & 1: self.send('E01'); return
            vals, _ = self.regs()
            out = ''.join(f'{v & 0xFFFFFFFF:08x}' for v in vals)
            if not self.tdesc_sent:
                out += '0' * (8 * 24 + 8 * 3)   # fp0-fp7 (96 bit each) + fpcontrol fpstatus fpiaddr
            self.send(out)
        elif c == b'G':
            vals = [int(p[1 + 8 * i:9 + 8 * i], 16) for i in range(18)]
            _, raw = self.regs(); self.set_regs(vals, raw); self.send('OK')
        elif c == b'p':
            n = int(p[1:], 16)
            flags, pc = self.status()
            if not flags & 1: self.send('E01'); return
            vals, _ = self.regs()
            if n < 18: self.send(f'{vals[n] & 0xFFFFFFFF:08x}')
            elif n < 26: self.send('0' * 24)   # fp registers, absent on a 68000
            else: self.send('00000000')
        elif c == b'P':
            n, v = p[1:].split(b'=')
            n, v = int(n, 16), int(v, 16)
            if n >= 18: self.send('OK'); return True
            vals, raw = self.regs(); vals[n] = v; self.set_regs(vals, raw); self.send('OK')
        elif c == b'm':
            addr, ln = (int(x, 16) for x in p[1:].split(b','))
            try:
                self.mon.guard(addr, ln)
                data = self.mon.read(addr, ln)
                self.send(data.hex())
            except SystemExit:
                self.send('E02')
        elif c == b'M':
            hdr, hexdata = p[1:].split(b':')
            addr, ln = (int(x, 16) for x in hdr.split(b','))
            self.mon.write(addr, bytes.fromhex(hexdata.decode())); self.send('OK')
        elif c == b'X':
            self.send('')  # binary writes not supported: GDB falls back to M
        elif c in (b'Z', b'z'):
            kind, addr, _ = p[1:].split(b',')
            if kind not in (b'0', b'1'): self.send(''); return
            addr = int(addr, 16)
            try:
                if c == b'Z': self.mon.transact(CMD['bp'], struct.pack('>I', addr), expect=1)
                else: self.mon.transact(CMD['bc'], struct.pack('>I', addr))
                self.send('OK')
            except SystemExit:
                self.send('E03')
        elif p == b'c' or p == b'vCont;c' or p == b'vCont;c:1':
            self.mon.transact(CMD['cont'])
            r = self.wait_stop()
            if r: self.send(r)
        elif p == b's' or p == b'vCont;s' or p == b'vCont;s:1':
            self.mon.transact(CMD['step'])
            for _ in range(50):
                flags, pc = self.status()
                if flags & 1: break
                time.sleep(0.05)
            self.send('S05')
        elif p == b'vCont?': self.send('vCont;c;s')
        elif c == b'c' or c == b's':
            self.send('E01')  # continue/step at an address: not supported
        elif p == b'k' or p.startswith(b'vKill'):
            self.send('OK'); return False
        elif c == b'D':
            # detach: let the program run
            flags, pc = self.status()
            if flags & 1: self.mon.transact(CMD['cont'])
            self.send('OK'); return False
        else:
            self.send('')
        return True

    def run(self):
        while True:
            p = self.recv_packet()
            if p is None: return
            if p == b'\x03' or p == b'': continue
            if self.handle(p) is False: return

def main():
    args = sys.argv[1:]
    dev, baud, port, verbose = '/dev/ttyUSB0', 4800, 1234, False
    while args:
        if args[0] == '-d': dev = args[1]; args = args[2:]
        elif args[0] == '-b': baud = int(args[1]); args = args[2:]
        elif args[0] == '-p': port = int(args[1]); args = args[2:]
        elif args[0] == '-v': verbose = True; args = args[1:]
        else: print(__doc__); return 4
    mon = Monitor(dev, baud, timeout=3.0)
    mon.fail = lambda code, msg: (_ for _ in ()).throw(SystemExit(msg))
    srv = socket.socket(); srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('', port)); srv.listen(1)
    log(f'mdgdb: monitor on {dev} at {baud}, listening on :{port}')
    while True:
        conn, peer = srv.accept()
        conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        log(f'mdgdb: client {peer}')
        try:
            Session(mon, conn, verbose).run()
        except SystemExit as e:
            log(f'mdgdb: monitor error: {e}')
        except (BrokenPipeError, ConnectionResetError):
            pass
        conn.close(); log('mdgdb: client gone')

if __name__ == '__main__':
    sys.exit(main())
