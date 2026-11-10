#!/usr/bin/env python3
"""prolink.py [-d DEVICE] COMMAND ...

Host side for the Mega Everdrive PRO over its USB virtual serial port
(/dev/ttyACM*, STM32 CDC "Mega_EverDrive"). Standard library only. Protocol as
in krikzz's megalink source (mega-ed-pub/megalink, 2025): 4-byte command
framing '+', '+'^FF, cmd, cmd^FF; big-endian arguments; status reply A5 xx.

Commands:
  status                 MCU status (0 = ok)
  reset [soft|hard|off]  console reset through the cartridge (default: soft pulse)
  load ROM [NAME] [--hard]  reset the console (soft, or hard with --hard), write ROM into cartridge PSRAM, run it
  memrd ADDR LEN FILE    read cartridge PI memory (ROM 0, SRAM 0x1000000, ...)
  memwr ADDR FILE        write cartridge PI memory
  disc IMAGE [NAME]      copy a disc image to sd:usb-games/ on the card, verify it, and ask the
                         menu to start it (menus that predate the command ignore it: pick the
                         file in the menu's usb-games folder then)
  sdput FILE SDPATH      copy a file to the card and verify it, e.g. sdput game.iso usb-games/game.iso
  sdcrc FILE SDPATH      compare a local file with one on the card (size and CRC-32)
Exit: 0 ok, 2 no answer, 3 protocol error, 4 usage.
"""
import os, select, struct, sys, termios, time, zlib

CMD_STATUS, CMD_MEM_RD, CMD_MEM_WR, CMD_HOST_RST = 0x10, 0x19, 0x1A, 0x29
CMD_F_FOPN, CMD_F_FWR, CMD_F_FCLOSE, CMD_F_FINFO, CMD_F_FCRC, CMD_F_DIR_MK = 0xC9, 0xCC, 0xCE, 0xD0, 0xD1, 0xD2
FAT_READ, FAT_WRITE, FAT_CREATE_ALWAYS = 0x01, 0x02, 0x08
ACK_BLOCK = 1024  # the card side acknowledges every 1 KB of a file write with a zero byte
ADDR_ROM, ADDR_CFG, ADDR_FIFO = 0x0000000, 0x1800000, 0x1810000
SD_HOME = 'usb-games'  # where megalink puts games sent over USB
RST_OFF, RST_SOFT, RST_HARD = 0, 1, 2
MAX_ROM = 0xF80000

def log(*a): print(*a, file=sys.stderr, flush=True)

class Pro:
    def __init__(self, dev):
        self.fd = os.open(dev, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
        a = termios.tcgetattr(self.fd)
        a[0] = 0; a[1] = 0
        a[2] = (a[2] & ~(termios.PARENB | termios.CSTOPB | termios.CSIZE | termios.CRTSCTS | termios.HUPCL)) | termios.CS8 | termios.CREAD | termios.CLOCAL
        a[3] = 0
        a[6][termios.VMIN] = 0; a[6][termios.VTIME] = 0
        termios.tcsetattr(self.fd, termios.TCSANOW, a)
        termios.tcflush(self.fd, termios.TCIOFLUSH)

    def tx(self, data):
        off = 0
        while off < len(data):
            _, w, _ = select.select([], [self.fd], [], 5.0)
            if not w: self.fail(2, 'write stalled')
            off += os.write(self.fd, data[off:off + 8192])

    def rx(self, n, timeout=2.0):
        buf = b''; end = time.time() + timeout
        while len(buf) < n and time.time() < end:
            r, _, _ = select.select([self.fd], [], [], 0.05)
            if r:
                try: buf += os.read(self.fd, n - len(buf))
                except BlockingIOError: pass
        return buf

    def fail(self, code, msg):
        log('prolink:', msg); sys.exit(code)

    def cmd(self, c, *args):
        self.tx(bytes([0x2B, 0x2B ^ 0xFF, c, c ^ 0xFF]) + b''.join(args))

    def status(self):
        self.cmd(CMD_STATUS)
        r = self.rx(2)
        if len(r) < 2: self.fail(2, 'no status reply')
        if r[0] != 0xA5: self.fail(3, f'unexpected status reply {r.hex()}')
        return r[1]

    def check(self, what):
        st = self.status()
        if st != 0: self.fail(3, f'{what}: status {st:02X}')

    def host_reset(self, mode):
        self.cmd(CMD_HOST_RST, bytes([mode]))

    def mem_wr(self, addr, data):
        self.cmd(CMD_MEM_WR, struct.pack('>II', addr, len(data)), b'\x00')
        self.tx(data)

    def mem_rd(self, addr, length):
        self.cmd(CMD_MEM_RD, struct.pack('>II', addr, length), b'\x00')
        data = self.rx(length, timeout=5.0 + length / 100000)
        if len(data) < length: self.fail(2, f'short read: {len(data)} of {length}')
        return data

    def fifo_wr(self, data):
        self.mem_wr(ADDR_FIFO, data)

    def expect(self, want, what, timeout=5.0):
        got = self.rx(1, timeout)
        if got != want: self.fail(2 if not got else 3, f'{what}: expected {want!r}, got {got!r}')

    def tx_string(self, text):
        b = text.encode('ascii')
        self.tx(struct.pack('>H', len(b)) + b)

    def fifo_string(self, text):
        b = text.encode('ascii')
        self.fifo_wr(struct.pack('>H', len(b)) + b)

    def usb_reset(self):
        """Reset into the cartridge menu with a cleared config, as megalink does before a card copy"""
        self.host_reset(RST_SOFT)
        time.sleep(0.01)
        self.mem_wr(ADDR_CFG, bytes(256))
        self.host_reset(RST_OFF)
        self.expect(b'r', 'menu after reset', timeout=10.0)

    def dir_make(self, path):
        self.cmd(CMD_F_DIR_MK); self.tx_string(path)
        st = self.status()
        if st not in (0, 8): self.fail(3, f'mkdir {path}: status {st:02X}')  # 8: exists

    def file_put(self, data, sd_path):
        """Write data to a file on the card; blocks of ACK_BLOCK, each sent after a zero byte"""
        self.cmd(CMD_F_FOPN, bytes([FAT_CREATE_ALWAYS | FAT_WRITE])); self.tx_string(sd_path)
        self.check(f'open {sd_path}')
        self.cmd(CMD_F_FWR, struct.pack('>I', len(data)))
        t0 = time.time()
        for off in range(0, len(data), ACK_BLOCK):
            ack = self.rx(1, timeout=10.0)
            if ack != b'\x00': self.fail(2 if not ack else 3, f'write ack at {off}: {ack!r}')
            self.tx(data[off:off + ACK_BLOCK])
            if off % (1 << 20) == 0 and off: log(f'  {off >> 20} MB, {off / (time.time() - t0) / 1024:.0f} KB/s')
        self.check('write')
        self.cmd(CMD_F_FCLOSE); self.check('close')
        log(f'{len(data)} bytes to sd:{sd_path} in {time.time() - t0:.1f} s')

    def file_info(self, sd_path):
        """size of a file on the card"""
        self.cmd(CMD_F_FINFO); self.tx_string(sd_path)
        st = self.rx(1, timeout=5.0)
        if st != b'\x00': self.fail(2 if not st else 3, f'info {sd_path}: {st!r}')
        size = struct.unpack('>I', self.rx(4, timeout=5.0))[0]
        self.rx(5, timeout=2.0)  # date, time, attributes
        n = struct.unpack('>H', self.rx(2, timeout=2.0))[0]
        self.rx(n, timeout=2.0)  # name
        return size

    def file_crc(self, sd_path, length):
        """CRC-32 of the first length bytes of a file on the card, computed by the cartridge"""
        self.cmd(CMD_F_FOPN, bytes([FAT_READ])); self.tx_string(sd_path); self.check(f'open {sd_path}')
        self.cmd(CMD_F_FCRC, struct.pack('>II', length, 0))
        st = self.rx(1, timeout=10.0 + length / 200000)
        if st != b'\x00': self.fail(2 if not st else 3, f'crc {sd_path}: {st!r}')
        crc = struct.unpack('>I', self.rx(4, timeout=5.0))[0]
        self.cmd(CMD_F_FCLOSE); self.check('close')
        return crc

    def verify(self, data, sd_path):
        size = self.file_info(sd_path)
        if size != len(data): self.fail(3, f'{sd_path}: {size} bytes on the card, {len(data)} local')
        want = zlib.crc32(data) & 0xFFFFFFFF
        got = self.file_crc(sd_path, size)
        if got != want: self.fail(3, f'{sd_path}: CRC {got:08X} on the card, {want:08X} local')
        log(f'verified sd:{sd_path}: {size} bytes, CRC-32 {got:08X}')

    def disc(self, path, name=None):
        """Copy a disc image to the card and start it, the way megalink's load works for any file"""
        data = open(path, 'rb').read()
        name = name or os.path.basename(path)
        sd_path = f'{SD_HOME}/{name}'
        self.usb_reset()
        self.dir_make(SD_HOME)
        self.file_put(data, sd_path)
        self.verify(data, sd_path)
        self.fifo_wr(b'*t'); self.expect(b'k', 'host test')
        self.fifo_wr(b'*i'); self.fifo_string(sd_path)
        st = self.rx(1, timeout=20.0)
        if not st:
            self.fail(2, f'the menu did not answer the install command; it is on the card as '
                         f'sd:{sd_path}, start it from the menu (newer menus start it from here)')
        if st != b'\x00': self.fail(3, f'install {sd_path}: {st!r}')
        self.fifo_wr(b'*s')
        self.check('start')
        log(f'started {sd_path}')

    def load(self, path, name=None, hard=False):
        data = open(path, 'rb').read()
        if len(data) > MAX_ROM: self.fail(4, 'ROM too big')
        name = name or os.path.basename(path)
        t0 = time.time()
        self.host_reset(RST_HARD if hard else RST_SOFT)
        self.mem_wr(ADDR_ROM, data)
        self.host_reset(RST_OFF)
        self.expect(b'r', 'menu after reset', timeout=10.0)
        self.fifo_wr(b'*t'); self.expect(b'k', 'host test')
        self.fifo_wr(b'*g')
        self.fifo_wr(struct.pack('>I', len(data)))
        self.fifo_wr(struct.pack('>H', len(name) + 4) + b'USB:' + name.encode('ascii'))
        log(f'{len(data)} bytes loaded and started in {time.time() - t0:.1f} s')

def main():
    args = sys.argv[1:]
    dev = '/dev/ttyACM0'
    if args[:1] == ['-d']: dev = args[1]; args = args[2:]
    if not args: print(__doc__); return 4
    p = Pro(dev)
    cmd, rest = args[0], args[1:]
    if cmd == 'status':
        print(f'status {p.status():02X}')
    elif cmd == 'reset':
        mode = rest[0] if rest else 'soft'
        if mode == 'off':
            p.host_reset(RST_OFF)
        else:
            p.host_reset(RST_HARD if mode == 'hard' else RST_SOFT)
            time.sleep(0.2)
            p.host_reset(RST_OFF)
        p.check('reset'); print(f'reset {mode}')
    elif cmd == 'load':
        hard = '--hard' in rest; rest = [r for r in rest if r != '--hard']
        p.load(rest[0], rest[1] if len(rest) > 1 else None, hard)
    elif cmd == 'disc':
        p.disc(rest[0], rest[1] if len(rest) > 1 else None)
    elif cmd == 'sdput':
        data = open(rest[0], 'rb').read()
        p.usb_reset(); p.file_put(data, rest[1]); p.verify(data, rest[1])
    elif cmd == 'sdcrc':
        p.verify(open(rest[0], 'rb').read(), rest[1])
    elif cmd == 'memrd':
        addr, length, out = int(rest[0], 0), int(rest[1], 0), rest[2]
        open(out, 'wb').write(p.mem_rd(addr, length)); print(f'{length} bytes to {out}')
    elif cmd == 'memwr':
        addr, data = int(rest[0], 0), open(rest[1], 'rb').read()
        p.mem_wr(addr, data); p.check('memwr'); print(f'{len(data)} bytes written')
    else:
        print(__doc__); return 4
    return 0

if __name__ == '__main__':
    sys.exit(main())
