#!/usr/bin/env python3
"""blastem.py BLASTEM NM ELF CART DATA_SUM_H WORKDIR CASE=ISO...

make check loop 11: run the mode1 example in BlastEm against each disc and
compare the results it stores in Work RAM with what that disc should give.

BlastEm runs in its debugger under a pty (offscreen video, dummy audio, a
HOME of its own per case), runs FRAMES frames, then the result variables are
read with p/x. Cases run in parallel. Cases and their expected values are in
CASES below; the discs are made by the Makefile.

Maintainer side only, and only when BLASTEM is set: the loop needs a BlastEm
build with a Mega CD Boot ROM (cdbios.bin beside the binary, or
system.scd_bios_* in its default.cfg)."""
import os, pty, re, select, subprocess, sys, threading, time

FRAMES = 1200
TIMEOUT = 240
QUIET = 0.3

VARS = ['mode1_status', 'mode1_disc', 'mode1_load', 'mode1_load_dma',
        'mode1_load_prg', 'mode1_load_sum', 'mode1_load_dma_sum',
        'mode1_load_prg_sum', 'mode1_file_size']

OK, NOT_FOUND, TRUNCATED = 0x164, 0x1FE, 0x1FD


def expect_loads(result, sums):
    e = {'mode1_status': 0x100, 'mode1_load': result, 'mode1_load_dma': result,
         'mode1_load_prg': result}
    if sums:
        e.update(mode1_load_sum=sums[0], mode1_load_dma_sum=sums[0],
                 mode1_load_prg_sum=sums[0], mode1_file_size=sums[1] & 0xFFFF)
    return e


def cases(data_sum, data_size):
    good = (data_sum, data_size)
    return {
        # the example's own disc: every load arrives intact
        'own':     dict(mode1_disc=OK, **expect_loads(OK, good)),
        # no DATA.BIN: not found, three times, and no hang (F-39)
        'nofile':  dict(mode1_disc=OK, **expect_loads(NOT_FOUND, None)),
        # 143 root entries: list truncated, DATA.BIN among the cached (KB-71)
        'longdir': dict(mode1_disc=TRUNCATED, **expect_loads(OK, good)),
    }


def run_blastem(blastem, cart, iso, home, cmds):
    """Run BlastEm's debugger, send cmds one per prompt, return its output"""
    pid, fd = pty.fork()
    if pid == 0:
        os.environ.update(SDL_VIDEODRIVER='offscreen', SDL_AUDIODRIVER='dummy',
                          HOME=home)
        # BlastEm finds cdbios.bin relative to its working directory
        os.chdir(os.path.dirname(blastem))
        os.execv(blastem, ['blastem', '-d', '-o', iso, cart])
    out, pending, t0 = b'', list(cmds), time.time()
    last, fresh, status = time.time(), False, None
    while time.time() - t0 < TIMEOUT:
        r, _, _ = select.select([fd], [], [], 0.1)
        if r:
            try:
                chunk = os.read(fd, 65536)
            except OSError:
                break
            if not chunk:
                break
            out += chunk
            last, fresh = time.time(), True
        elif fresh and out.rstrip(b' ').endswith(b'>') and time.time() - last >= QUIET:
            fresh = False
            if pending:
                os.write(fd, (pending.pop(0) + '\n').encode())
        p, st = os.waitpid(pid, os.WNOHANG)
        if p:
            status = st
            break
    if status is None:
        try:
            os.kill(pid, 9)
            os.waitpid(pid, 0)
        except OSError:
            pass
    return out.decode(errors='replace')


def main(argv):
    blastem, nm, elf, cart, sums_h, work = argv[:6]
    discs = dict(a.split('=', 1) for a in argv[6:])
    syms = {}
    for line in subprocess.run([nm, elf], capture_output=True, text=True,
                               check=True).stdout.splitlines():
        f = line.split()
        if len(f) == 3:
            syms[f[2]] = int(f[0], 16)
    text = open(sums_h).read()
    data_sum = int(re.search(r'DATA_SUM\s+(\S+)', text).group(1), 0)
    data_size = int(re.search(r'DATA_SIZE\s+(\S+)', text).group(1), 0)
    want_all = cases(data_sum, data_size)
    cmds = [f'frames {FRAMES}'] + [f'p/x [{syms[v]:#x}]' for v in VARS] + ['quit']

    results = {}

    def one(name):
        home = os.path.join(work, name)
        os.makedirs(home, exist_ok=True)
        out = run_blastem(os.path.abspath(blastem), os.path.abspath(cart),
                          os.path.abspath(discs[name]), home, cmds)
        open(os.path.join(work, name + '.log'), 'w').write(out)
        got = {}
        for v in VARS:
            m = re.search(r'\[%#x\]: ([0-9a-f]+)' % syms[v], out)
            got[v] = int(m.group(1), 16) if m else None
        results[name] = got

    threads = [threading.Thread(target=one, args=(n,)) for n in discs]
    for t in threads:
        t.start()
    for t in threads:
        t.join()

    fail = 0
    for name in discs:
        want, got = want_all[name], results[name]
        bad = [f'{v} {got[v] if got[v] is None else hex(got[v])}, want {w:#x}'
               for v, w in want.items() if got[v] != w]
        if bad:
            fail = 1
            print(f'FAIL emulate {name}: ' + '; '.join(bad) +
                  f' (log {os.path.join(work, name + ".log")})')
    return fail


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
