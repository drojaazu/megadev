# [ M E G A D E V ]   a Sega Mega CD devkit
#
# Maintainer tooling for the megadev repository itself.
#
# This file is NOT part of the kit a project uses. Projects include
# $(MEGADEV_PATH)/megadev.make and never read this Makefile, so nothing here
# can collide with a project's own targets. Everything a project sees lives in
# megadev.make.
#
# Targets:
#   make check   Verification gate. Must pass before a branch merges.
#   make docs    Generate the reference documentation into docs/html.
#   make clean   Remove the build output the gate produced.

MEGADEV_PATH := $(CURDIR)
export MEGADEV_PATH

PROJECTS := $(wildcard examples/*/) new_project/
HEADERS  := $(shell find lib -name '*.h' | sort)

# Hardware targets a header must be valid for (see lib/build.def.h).
TARGETS := MEGACD MEGACD_MODE1 MEGADRIVE

CC := $(M68K_PREFIX)gcc
CHECK_DIR := build-check

# The flags megadev.make gives a project, minus the project-specific header
# strings, which no library header depends on.
CHECK_FLAGS := -m68000 -imacros build.def.h -Ilib -Wa,-Ilib \
	-DPROJECT_ID=check -DREGION=US -DVIDEO=NTSC \
	-fno-builtin -Wall -Wextra -Wno-main -Wa,--register-prefix-optional

ASM_SOURCES := $(shell find lib -name '*.s' | sort)

# Assembly files that by contract cannot assemble on their own. Each entry
# needs a reason. This list may only shrink.
#   lib/cd_boot.s    .incbin of the project's built ip.bin and sp.bin
#   lib/md_header.s  needs the project's fixed-length header strings; any
#                    default would hide a string that overruns its field.
#                    Exercised by the cart example build in loop 1.
ASM_STANDALONE_EXCLUDE := lib/cd_boot.s lib/md_header.s
ASM_STANDALONE := $(filter-out $(ASM_STANDALONE_EXCLUDE),$(ASM_SOURCES))

RUNTIME_SRC := $(wildcard lib/runtime/*.s lib/runtime/*.c)

# Project and header used by the incremental rebuild check.
INCR_PROJECT := examples/gfx
INCR_HEADER  := lib/main/vdp.h

# Headers whose inline functions do not yet compile when instantiated. Each
# entry needs a reason. This list may only shrink.
INLINE_EXCLUDE :=

# The optimization flags projects build with by default; some inline asm is
# only valid under them.
INLINE_FLAGS := -O1 -fomit-frame-pointer

# Compile-time assertions about the hardware definitions (check/asserts).
ASSERT_C := $(wildcard check/asserts/*.c)
ASSERT_S := $(wildcard check/asserts/*.s)

# Host compiler for the runtime library test; the runtime sources are plain C.
HOST_CC ?= cc

.PHONY: check check-projects check-headers check-asm check-link check-stack check-init check-sections check-init check-odr check-incremental check-inline check-asserts check-host docs clean $(PROJECTS)

check: check-projects check-headers check-asm check-link check-stack check-init check-sections check-odr check-incremental check-inline check-asserts check-host
	@printf '\ncheck: all passed\n'

# ---------------------------------------------------------------------------
# Loop 1: every example and the project template builds from clean, through
# its own makefile, exactly as a user project would.
# ---------------------------------------------------------------------------
check-projects: $(PROJECTS)

$(PROJECTS):
	@printf '== build %s\n' '$@'
	@$(MAKE) -s -C '$@' clean >/dev/null
	@$(MAKE) -s -C '$@' >/dev/null
	@id=$$($(MAKE) -s -C '$@' print-PROJECT_ID); \
	for f in $$($(MAKE) -s -C '$@' print-DISC_CONTENTS) $$(cd '$@' && ls $$id.iso $$id.cart 2>/dev/null); do \
	  if [ ! -s "$@/$$f" ]; then printf 'FAIL %s: %s missing or empty\n' '$@' "$$f"; exit 1; fi; \
	done
	@# A cartridge header describes the ROM it is in, recomputed here
	@# independently of tools/romfix.sh.
	@id=$$($(MAKE) -s -C '$@' print-PROJECT_ID); f='$@'/$$id.cart; \
	if [ -f "$$f" ]; then \
	  sz=$$(stat -c %s "$$f"); \
	  [ $$((sz % 2)) = 0 ] || { printf 'FAIL %s: odd size %s\n' "$$f" $$sz; exit 1; }; \
	  end=$$(od -An -tx1 -j $$((0x1A4)) -N4 "$$f" | tr -d ' \n'); \
	  [ "$$end" = "$$(printf '%08x' $$((sz - 1)))" ] || { printf 'FAIL %s: ROM end %s, size %s\n' "$$f" $$end $$sz; exit 1; }; \
	  sum=$$(od -An -v -tu2 --endian=big -j 512 "$$f" | awk '{for (i = 1; i <= NF; i++) s += $$i} END {printf "%04x", s % 65536}'); \
	  hdr=$$(od -An -tx1 -j $$((0x18E)) -N2 "$$f" | tr -d ' \n'); \
	  [ "$$sum" = "$$hdr" ] || { printf 'FAIL %s: header checksum %s, computed %s\n' "$$f" $$hdr $$sum; exit 1; }; \
	fi

# ---------------------------------------------------------------------------
# Loop 2: every library header compiles on its own, for every hardware target.
# Catches a header that only works when something else was included first.
# ---------------------------------------------------------------------------
check-headers:
	@mkdir -p $(CHECK_DIR)
	@fail=0; \
	for h in $(HEADERS); do \
	  for t in $(TARGETS); do \
	    printf '#include <%s>\n' "$${h#lib/}" > $(CHECK_DIR)/header.c; \
	    if ! $(CC) $(CHECK_FLAGS) -DTARGET=$$t -fsyntax-only -Werror \
	         $(CHECK_DIR)/header.c 2> $(CHECK_DIR)/header.err; then \
	      printf 'FAIL header %s (%s)\n' "$$h" "$$t"; cat $(CHECK_DIR)/header.err; fail=1; \
	    fi; \
	  done; \
	done; \
	n=$$(echo $(HEADERS) | wc -w); \
	[ $$fail = 0 ] && printf '== headers: %s compile alone (%s)\n' "$$n" "$(TARGETS)" || exit 1

# ---------------------------------------------------------------------------
# Loop 3: every library assembly file assembles on its own. Catches files no
# project uses.
# ---------------------------------------------------------------------------
check-asm:
	@mkdir -p $(CHECK_DIR)
	@fail=0; \
	for s in $(ASM_STANDALONE); do \
	  if ! $(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -Wa,--bitwise-or \
	       -x assembler-with-cpp -c "$$s" -o $(CHECK_DIR)/asm.o 2> $(CHECK_DIR)/asm.err; then \
	    printf 'FAIL asm %s\n' "$$s"; cat $(CHECK_DIR)/asm.err; fail=1; \
	  fi; \
	done; \
	n=$$(echo $(ASM_STANDALONE) | wc -w); x=$$(echo $(ASM_STANDALONE_EXCLUDE) | wc -w); \
	[ $$fail = 0 ] && printf '== asm: %s assemble alone, %s excluded by contract\n' "$$n" "$$x" || exit 1

# ---------------------------------------------------------------------------
# Loop 4: every call the compiler emits on its own resolves at link time.
# A translation unit that forces large struct copies and 32-bit arithmetic
# is linked against lib/runtime alone; no symbol may remain undefined. 64-bit
# and floating point arithmetic, which lib/runtime does not provide, must stop
# the link.
# ---------------------------------------------------------------------------
check-link:
	@rm -rf $(CHECK_DIR)/runtime && mkdir -p $(CHECK_DIR)/runtime
	@printf '%s\n' \
	  'typedef struct { char b[400]; } big;' \
	  'big g; unsigned long u; long s; int initialized = 1;' \
	  'void copy(big * d, big const * s) { *d = *s; }' \
	  'void zero(big * d) { *d = (big){0}; }' \
	  'int same(big const * a, big const * b) { return __builtin_memcmp(a, b, sizeof *a); }' \
	  'void shift(big * d) { __builtin_memmove(d->b + 1, d->b, sizeof d->b - 1); }' \
	  'void math(void) { u = u * u; u = u / 7; u = u % 7; s = s * s; s = s / 7; s = s % 7; }' \
	  'int main(void) { return 0; }' \
	  > $(CHECK_DIR)/link.c
	@$(CC) $(CHECK_FLAGS) -DTARGET=MEGADRIVE -O2 -c $(CHECK_DIR)/link.c -o $(CHECK_DIR)/link.o
	@for s in $(RUNTIME_SRC); do \
	  case "$$s" in \
	    *.s) $(CC) $(CHECK_FLAGS) -DTARGET=MEGADRIVE -Wa,--bitwise-or -x assembler-with-cpp \
	           -c "$$s" -o $(CHECK_DIR)/runtime/$$(basename $$s).o || exit 1 ;; \
	    *.c) $(CC) $(CHECK_FLAGS) -DTARGET=MEGACD \
	           -c "$$s" -o $(CHECK_DIR)/runtime/$$(basename $$s).o || exit 1 ;; \
	  esac; \
	done
	@$(M68K_PREFIX)ar rcs $(CHECK_DIR)/libmegadev.a $(CHECK_DIR)/runtime/*.o
	@$(M68K_PREFIX)ld -nostdlib -z noexecstack -Lcfg -T cfg/md_cart.ld $(CHECK_DIR)/link.o \
	  $(CHECK_DIR)/libmegadev.a -o $(CHECK_DIR)/link.elf
	@if $(M68K_PREFIX)nm -u $(CHECK_DIR)/link.elf | grep .; then \
	  printf 'FAIL link: undefined symbols above\n'; exit 1; fi
	@printf '== link: compiler-emitted calls resolve (%s)\n' \
	  "$$($(M68K_PREFIX)nm -u $(CHECK_DIR)/link.o | awk '{print $$2}' | tr '\n' ' ')"
	@# Disassembled as a strict 68000, any instruction from a later CPU shows
	@# up as raw data words.
	@if $(M68K_PREFIX)objdump -d -m m68k:68000 $(CHECK_DIR)/link.elf | grep -E '\.(short|word|long)'; then \
	  printf 'FAIL link: instructions above are not 68000 instructions\n'; exit 1; fi
	@printf '== link: runtime and compiler helpers are 68000 code\n'
	@lma=$$($(M68K_PREFIX)objdump -h $(CHECK_DIR)/link.elf | awk '$$2==".data"{print $$5}'); \
	  if [ -z "$$lma" ] || [ $$((0x$$lma)) -ge $$((0x400000)) ]; then \
	    printf 'FAIL link: cartridge .data has no load address in ROM (%s)\n' "$$lma"; exit 1; fi
	@printf '== link: cartridge initialized data is stored in ROM\n'
	@printf '%s\n' \
	  'long long l; float f;' \
	  'int main(void) { l = l * l; f = f + f; return 0; }' \
	  > $(CHECK_DIR)/nolink.c
	@$(CC) $(CHECK_FLAGS) -DTARGET=MEGADRIVE -O2 -c $(CHECK_DIR)/nolink.c -o $(CHECK_DIR)/nolink.o
	@if $(M68K_PREFIX)ld -nostdlib -z noexecstack -Lcfg -T cfg/md_cart.ld $(CHECK_DIR)/nolink.o \
	  $(CHECK_DIR)/libmegadev.a -o $(CHECK_DIR)/nolink.elf 2> $(CHECK_DIR)/nolink.err; then \
	  printf 'FAIL link: 64-bit and floating point arithmetic linked\n'; exit 1; fi
	@for h in __muldi3 __addsf3; do \
	  grep -q "undefined reference to .$$h" $(CHECK_DIR)/nolink.err || { \
	    printf 'FAIL link: expected an undefined %s\n' "$$h"; cat $(CHECK_DIR)/nolink.err; exit 1; }; \
	done
	@printf '== link: 64-bit and floating point arithmetic stop the link\n'

# ---------------------------------------------------------------------------
# The stack-setting init files load STACK_TOP just before entering main, and
# the linker scripts reject a stack that does not fit.
# ---------------------------------------------------------------------------
STACK_LD = $(M68K_PREFIX)ld -nostdlib -z noexecstack -Lcfg \
	--defsym=MMD_DEST=0 \
	--defsym=MODULE_ROM_ORIGIN=0xFF0000 --defsym=MODULE_ROM_LENGTH=0x4000 \
	--defsym=MODULE_RAM_ORIGIN=0xFFC000 --defsym=MODULE_RAM_LENGTH=0x1000

check-stack:
	@mkdir -p $(CHECK_DIR)
	@printf '.global main\nmain: rts\n.bss\n.space 0x1000\n' > $(CHECK_DIR)/stack_main.s
	@$(CC) -m68000 -c $(CHECK_DIR)/stack_main.s -o $(CHECK_DIR)/stack_main.o
	@for s in std_init_stack main/ipx_init_stack; do \
	  o=$(CHECK_DIR)/stack_$$(basename $$s).o; e=$$o.elf; \
	  $(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -x assembler-with-cpp -c lib/$$s.s -o $$o || exit 1; \
	  if $(STACK_LD) -T cfg/module_mmd.ld $$o $(CHECK_DIR)/stack_main.o -o $$e 2>/dev/null; then \
	    printf 'FAIL stack: %s linked without STACK_TOP\n' $$s; exit 1; fi; \
	  $(STACK_LD) --defsym=STACK_TOP=0xFFC000 --defsym=STACK_SIZE=0x1000 -T cfg/module_mmd.ld $$o $(CHECK_DIR)/stack_main.o -o $$e || exit 1; \
	  $(M68K_PREFIX)objdump -d --no-show-raw-insn -j .rom $$e | grep -A1 'moveal #16760832,%sp' | grep -q 'jmp' || { \
	    printf 'FAIL stack: %s does not set sp just before entering main\n' $$s; exit 1; }; \
	done
	@o=$(CHECK_DIR)/stack_std_init_stack.o; \
	for c in '0xFFC001 0 even' '0xFFC800 0 RAM' '0xFF0002 0 code' '0xFFD000 0x1800 RAM' '0xFF2000 0x2000 code' '0 0xFFF0 RAM' '0 0x1000 jump' '0xFFFD80 0 jump'; do \
	  set -- $$c; \
	  if $(STACK_LD) --defsym=STACK_TOP=$$1 --defsym=STACK_SIZE=$$2 -T cfg/module_mmd.ld $$o $(CHECK_DIR)/stack_main.o \
	       -o $(CHECK_DIR)/stack_bad.elf 2> $(CHECK_DIR)/stack_bad.err; then \
	    printf 'FAIL stack: top %s size %s linked\n' $$1 $$2; exit 1; fi; \
	  grep -q "$$3" $(CHECK_DIR)/stack_bad.err || { \
	    printf 'FAIL stack: top %s size %s: expected "%s"\n' $$1 $$2 $$3; cat $(CHECK_DIR)/stack_bad.err; exit 1; }; \
	done
	@# Projects define STACK_TOP in an object, not on the command line.
	@printf '.global STACK_TOP\n.equ STACK_TOP, 0xFFC800\n' > $(CHECK_DIR)/stack_sym.s
	@$(CC) -m68000 -c $(CHECK_DIR)/stack_sym.s -o $(CHECK_DIR)/stack_sym.o
	@if $(STACK_LD) -T cfg/module_mmd.ld $(CHECK_DIR)/stack_std_init_stack.o $(CHECK_DIR)/stack_main.o \
	     $(CHECK_DIR)/stack_sym.o -o $(CHECK_DIR)/stack_bad.elf 2>/dev/null; then \
	  printf 'FAIL stack: a STACK_TOP defined in an object was not checked\n'; exit 1; fi
	@$(STACK_LD) --defsym=STACK_TOP=0 --defsym=STACK_SIZE=0x24C -T cfg/module_mmd.ld $(CHECK_DIR)/stack_std_init_stack.o \
	  $(CHECK_DIR)/stack_main.o -o $(CHECK_DIR)/stack_top.elf
	@# Cartridges: the ROM fits cartridge space, RAM stays below the stack, and
	@# in Mode 1 below the system vector table.
	@printf '.global main\nmain: rts\n.space 0x400000\n' > $(CHECK_DIR)/cart_big.s
	@$(CC) -m68000 -c $(CHECK_DIR)/cart_big.s -o $(CHECK_DIR)/cart_big.o
	@if $(STACK_LD) -T cfg/md_cart.ld $(CHECK_DIR)/cart_big.o -o $(CHECK_DIR)/cart_bad.elf 2> $(CHECK_DIR)/cart_bad.err; then \
	  printf 'FAIL stack: a cartridge larger than 4 MB linked\n'; exit 1; fi
	@grep -q "not within region .rom" $(CHECK_DIR)/cart_bad.err || { printf 'FAIL stack: expected a rom region overflow\n'; cat $(CHECK_DIR)/cart_bad.err; exit 1; }
	@printf '.global main\nmain: rts\n.bss\n.space 0xFE00\n' > $(CHECK_DIR)/cart_ram.s
	@$(CC) -m68000 -c $(CHECK_DIR)/cart_ram.s -o $(CHECK_DIR)/cart_ram.o
	@if $(STACK_LD) --defsym=INIT_SSP=0xFFF000 -T cfg/md_cart.ld $(CHECK_DIR)/cart_ram.o -o $(CHECK_DIR)/cart_bad.elf 2> $(CHECK_DIR)/cart_bad.err; then \
	  printf 'FAIL stack: cartridge RAM overlapping the stack linked\n'; exit 1; fi
	@grep -q 'overlaps the stack' $(CHECK_DIR)/cart_bad.err || { printf 'FAIL stack: expected the stack overlap error\n'; cat $(CHECK_DIR)/cart_bad.err; exit 1; }
	@if $(STACK_LD) --defsym=INIT_SSP=0 --defsym=mode1_init_vectors=0x200 -T cfg/md_cart.ld $(CHECK_DIR)/cart_ram.o -o $(CHECK_DIR)/cart_bad.elf 2> $(CHECK_DIR)/cart_bad.err; then \
	  printf 'FAIL stack: Mode 1 cartridge RAM over the system vector table linked\n'; exit 1; fi
	@grep -q 'Mode 1: ' $(CHECK_DIR)/cart_bad.err || { printf 'FAIL stack: expected the Mode 1 vector table error\n'; cat $(CHECK_DIR)/cart_bad.err; exit 1; }
	@$(STACK_LD) --defsym=INIT_SSP=0 -T cfg/md_cart.ld $(CHECK_DIR)/cart_ram.o -o $(CHECK_DIR)/cart_ok.elf
	@printf '== stack: init files set STACK_TOP; a stack at 0 links; the linker rejects 9 bad module stacks and 3 bad cartridges\n'

# A module links only with init code, or with MODULE_NO_INIT defined.
check-init:
	@mkdir -p $(CHECK_DIR)
	@printf '.global main\nmain: rts\n' > $(CHECK_DIR)/init_main.s
	@$(CC) -m68000 -c $(CHECK_DIR)/init_main.s -o $(CHECK_DIR)/init_main.o
	@$(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -x assembler-with-cpp -c lib/std_init.s -o $(CHECK_DIR)/init_std.o
	@for ld in module_mmd module_smd; do \
	  if $(STACK_LD) -T cfg/$$ld.ld $(CHECK_DIR)/init_main.o -o $(CHECK_DIR)/init.elf 2> $(CHECK_DIR)/init.err; then \
	    printf 'FAIL init: %s linked a module without init code\n' $$ld; exit 1; fi; \
	  grep -q 'no init code' $(CHECK_DIR)/init.err || { \
	    printf 'FAIL init: %s: expected "no init code"\n' $$ld; cat $(CHECK_DIR)/init.err; exit 1; }; \
	  $(STACK_LD) -T cfg/$$ld.ld $(CHECK_DIR)/init_std.o $(CHECK_DIR)/init_main.o -o $(CHECK_DIR)/init.elf || exit 1; \
	  $(STACK_LD) --defsym=MODULE_NO_INIT=1 -T cfg/$$ld.ld $(CHECK_DIR)/init_main.o -o $(CHECK_DIR)/init.elf || exit 1; \
	done
	@printf '#include <macro.h>\nGLOBAL_SYM(MODULE_NO_INIT, 1);\n' > $(CHECK_DIR)/init_optout.c
	@$(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -c $(CHECK_DIR)/init_optout.c -o $(CHECK_DIR)/init_optout.o
	@$(STACK_LD) -T cfg/module_smd.ld $(CHECK_DIR)/init_optout.o $(CHECK_DIR)/init_main.o -o $(CHECK_DIR)/init.elf || { \
	  printf 'FAIL init: MODULE_NO_INIT from GLOBAL_SYM in C was not honored\n'; exit 1; }
	@printf '== init: modules link only with init code or MODULE_NO_INIT\n'

# Input sections the scripts must handle: an assembly common symbol (.comm)
# lands inside the zeroed region of every script, and a static constructor or
# destructor stops the link.
check-sections: check-init
	@mkdir -p $(CHECK_DIR)
	@printf '.global main\nmain: rts\n.comm common_buf, 64\n' > $(CHECK_DIR)/sections_comm.s
	@$(CC) -m68000 -c $(CHECK_DIR)/sections_comm.s -o $(CHECK_DIR)/sections_comm.o
	@for c in 'module_mmd _BSS_ORIGIN _BSS_LENGTH' 'module_smd _BSS_ORIGIN _BSS_LENGTH' 'ip _BSS_ORIGIN _BSS_LENGTH' \
	          'sp _BSS_ORIGIN _BSS_LENGTH' 'md_cart _bss_org _bss_len'; do \
	  set -- $$c; e=$(CHECK_DIR)/sections_$$1.elf; \
	  $(STACK_LD) --defsym=MODULE_NO_INIT=1 -T cfg/$$1.ld $(CHECK_DIR)/sections_comm.o -o $$e 2>/dev/null || { \
	    printf 'FAIL sections: %s did not link\n' $$1; exit 1; }; \
	  sym() { $(M68K_PREFIX)nm $$e | awk -v n=$$1 '$$3 == n { print "0x" $$1 }'; }; \
	  buf=$$(sym common_buf); org=$$(sym $$2); len=$$(sym $$3); \
	  [ $$((buf)) -ge $$((org)) ] && [ $$((buf + 64)) -le $$((org + len)) ] || { \
	    printf 'FAIL sections: %s: common symbol at %s is outside the zeroed region %s + %s\n' $$1 $$buf $$org $$len; exit 1; }; \
	done
	@printf 'int v;\n__attribute__((constructor)) static void c(void) { v = 1; }\n__attribute__((destructor)) static void d(void) { v = 0; }\nint main(void) { return v; }\n' > $(CHECK_DIR)/sections_ctor.c
	@$(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -O1 -c $(CHECK_DIR)/sections_ctor.c -o $(CHECK_DIR)/sections_ctor.o
	@for ld in module_mmd module_smd ip sp md_cart; do \
	  if $(STACK_LD) --defsym=MODULE_NO_INIT=1 -T cfg/$$ld.ld $(CHECK_DIR)/sections_ctor.o -o $(CHECK_DIR)/sections_ctor.elf \
	       2> $(CHECK_DIR)/sections_ctor.err; then \
	    printf 'FAIL sections: %s linked a static constructor\n' $$ld; exit 1; fi; \
	  grep -q 'constructors and destructors are not supported' $(CHECK_DIR)/sections_ctor.err || { \
	    printf 'FAIL sections: %s: expected the constructor error\n' $$ld; cat $(CHECK_DIR)/sections_ctor.err; exit 1; }; \
	done
	@# The cartridge vector table starts the ROM whatever the object order.
	@printf '.global main\nmain: rts\n' > $(CHECK_DIR)/sections_main.s
	@printf '.section .vectors, "ax"\n.long 0xFFFFFC00\n.long main\n' > $(CHECK_DIR)/sections_vec.s
	@$(CC) -m68000 -c $(CHECK_DIR)/sections_main.s -o $(CHECK_DIR)/sections_main.o
	@$(CC) -m68000 -c $(CHECK_DIR)/sections_vec.s -o $(CHECK_DIR)/sections_vec.o
	@$(STACK_LD) -T cfg/md_cart.ld $(CHECK_DIR)/sections_main.o $(CHECK_DIR)/sections_vec.o -o $(CHECK_DIR)/sections_vec.elf
	@$(M68K_PREFIX)objcopy -O binary $(CHECK_DIR)/sections_vec.elf $(CHECK_DIR)/sections_vec.bin
	@[ "$$(od -An -tx1 -N4 $(CHECK_DIR)/sections_vec.bin | tr -d ' \n')" = fffffc00 ] || { \
	  printf 'FAIL sections: the vector table does not start the cartridge ROM when linked after other code\n'; exit 1; }
	@# --gc-sections keeps what nothing references but the hardware needs.
	@$(STACK_LD) --gc-sections -T cfg/module_mmd.ld $(CHECK_DIR)/init_std.o $(CHECK_DIR)/init_main.o -o $(CHECK_DIR)/sections_gc.elf
	@$(M68K_PREFIX)nm $(CHECK_DIR)/sections_gc.elf | grep -q ' T init$$' || { \
	  printf 'FAIL sections: --gc-sections removed module init code\n'; exit 1; }
	@$(STACK_LD) --gc-sections -T cfg/md_cart.ld $(CHECK_DIR)/sections_main.o $(CHECK_DIR)/sections_vec.o -o $(CHECK_DIR)/sections_gc.elf
	@$(M68K_PREFIX)objdump -h $(CHECK_DIR)/sections_gc.elf | awk '$$2 == ".text" { exit !(strtonum("0x" $$3) >= 8) }' || { \
	  printf 'FAIL sections: --gc-sections removed the cartridge vector table\n'; exit 1; }
	@printf '== sections: common symbols are zeroed, static constructors stop the link, cartridge vectors start the ROM, --gc-sections keeps entry code\n'

# ---------------------------------------------------------------------------
# Loop 5: every library header can be included by two translation units that
# are linked together. A header that defines storage or a non-static function
# produces a duplicate symbol here.
# ---------------------------------------------------------------------------
check-odr:
	@mkdir -p $(CHECK_DIR)
	@fail=0; \
	for h in $(HEADERS); do \
	  for t in $(TARGETS); do \
	    printf '#include <%s>\nint one(void) { return 1; }\n' "$${h#lib/}" > $(CHECK_DIR)/odr1.c; \
	    printf '#include <%s>\nint one(void); int main(void) { return one(); }\n' "$${h#lib/}" > $(CHECK_DIR)/odr2.c; \
	    if ! ( $(CC) $(CHECK_FLAGS) -DTARGET=$$t -fno-common -c $(CHECK_DIR)/odr1.c -o $(CHECK_DIR)/odr1.o \
	        && $(CC) $(CHECK_FLAGS) -DTARGET=$$t -fno-common -c $(CHECK_DIR)/odr2.c -o $(CHECK_DIR)/odr2.o \
	        && $(M68K_PREFIX)ld -r $(CHECK_DIR)/odr1.o $(CHECK_DIR)/odr2.o -o $(CHECK_DIR)/odr.o ) \
	        2> $(CHECK_DIR)/odr.err; then \
	      printf 'FAIL odr %s (%s)\n' "$$h" "$$t"; cat $(CHECK_DIR)/odr.err; fail=1; \
	    fi; \
	  done; \
	done; \
	n=$$(echo $(HEADERS) | wc -w); \
	[ $$fail = 0 ] && printf '== odr: %s headers link from two translation units\n' "$$n" || exit 1

# ---------------------------------------------------------------------------
# Loop 6: header dependency tracking works. Touching a library header must
# recompile something and relink the module; a second make must do nothing.
# ---------------------------------------------------------------------------
check-incremental:
	@$(MAKE) -s -C $(INCR_PROJECT) >/dev/null
	@sleep 1; touch $(INCR_HEADER)
	@out=$$($(MAKE) -C $(INCR_PROJECT) 2>&1 | sed 's/\x1b\[[0-9;]*m//g'); \
	echo "$$out" | grep -q 'Compiling' || { printf 'FAIL incremental: touching %s recompiled nothing\n' '$(INCR_HEADER)'; exit 1; }; \
	echo "$$out" | grep -q 'Linking' || { printf 'FAIL incremental: touching %s did not relink\n' '$(INCR_HEADER)'; exit 1; }
	@$(MAKE) -q -C $(INCR_PROJECT) >/dev/null 2>&1 || { printf 'FAIL incremental: second make is not a no-op\n'; exit 1; }
	@printf '== incremental: %s rebuilds %s and settles\n' '$(INCR_HEADER)' '$(INCR_PROJECT)'

# ---------------------------------------------------------------------------
# Loop 7: every static inline function in every header compiles when it is
# instantiated. A header compile alone never parses a body nothing calls;
# this takes the address of each function so the compiler must.
# ---------------------------------------------------------------------------
check-inline:
	@mkdir -p $(CHECK_DIR)
	@fail=0; total=0; \
	for h in $(filter-out $(INLINE_EXCLUDE),$(HEADERS)); do \
	  printf '#include <%s>\n' "$${h#lib/}" > $(CHECK_DIR)/inline.c; \
	  $(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -fsyntax-only -aux-info $(CHECK_DIR)/inline.X $(CHECK_DIR)/inline.c 2>/dev/null || true; \
	  names=$$(grep -E "^/\* $$h:[0-9]+:N[FC] \*/ static " $(CHECK_DIR)/inline.X | sed -E 's/ \(.*//; s/.*[ *]//' | sort -u); \
	  [ -n "$$names" ] || continue; \
	  total=$$((total + $$(echo "$$names" | wc -l))); \
	  { printf '#include <%s>\nvoid const * const megadev_inline_check[] = {\n' "$${h#lib/}"; \
	    for f in $$names; do printf '  (void const *) %s,\n' "$$f"; done; printf '};\n'; } > $(CHECK_DIR)/inline.c; \
	  if ! $(CC) $(CHECK_FLAGS) $(INLINE_FLAGS) -DTARGET=MEGACD -c $(CHECK_DIR)/inline.c -o $(CHECK_DIR)/inline.o 2> $(CHECK_DIR)/inline.err; then \
	    printf 'FAIL inline %s\n' "$$h"; cat $(CHECK_DIR)/inline.err; fail=1; \
	  fi; \
	done; \
	x=$$(echo $(INLINE_EXCLUDE) | wc -w); \
	[ $$fail = 0 ] && printf '== inline: %s functions instantiate, %s header(s) excluded\n' "$$total" "$$x" || exit 1

# ---------------------------------------------------------------------------
# Loop 8: the register definitions match the hardware manuals. check/asserts
# holds _Static_assert units for C and .if/.error units for assembly; a wrong
# address or bit position fails here instead of on hardware.
# ---------------------------------------------------------------------------
check-asserts:
	@mkdir -p $(CHECK_DIR)
	@fail=0; \
	for c in $(ASSERT_C); do \
	  $(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -fsyntax-only "$$c" 2> $(CHECK_DIR)/assert.err \
	    || { printf 'FAIL assert %s\n' "$$c"; cat $(CHECK_DIR)/assert.err; fail=1; }; \
	done; \
	for s in $(ASSERT_S); do \
	  $(CC) $(CHECK_FLAGS) -DTARGET=MEGACD -Wa,--bitwise-or -x assembler-with-cpp -c "$$s" \
	    -o $(CHECK_DIR)/assert.o 2> $(CHECK_DIR)/assert.err \
	    || { printf 'FAIL assert %s\n' "$$s"; cat $(CHECK_DIR)/assert.err; fail=1; }; \
	done; \
	n=$$(cat $(ASSERT_C) | grep -c '_Static_assert'); m=$$(cat $(ASSERT_S) | grep -c '^ASSERT_EQ'); \
	[ $$fail = 0 ] && printf '== asserts: %s C and %s assembly assertions hold\n' "$$n" "$$m" || exit 1

# ---------------------------------------------------------------------------
# Loop 9: the freestanding C library subset computes the right answers. The
# runtime sources compile for the host, where check/host/libc_test.c runs
# them against known results. The only loop in which code executes.
# ---------------------------------------------------------------------------
check-host:
	@mkdir -p $(CHECK_DIR)
	@$(HOST_CC) -std=c11 -Wall -Wextra -Werror -nostdinc -Ilib \
	  -I$$($(HOST_CC) -print-file-name=include) \
	  check/host/libc_test.c -o $(CHECK_DIR)/libc_test
	@$(CHECK_DIR)/libc_test
	@printf '== host: libc subset passes %s checks\n' "$$(grep -c 'CHECK(' check/host/libc_test.c)"

docs:
	@rm -rf docs/html; mkdir -p $(CHECK_DIR)
	@doxygen Doxyfile > /dev/null 2> $(CHECK_DIR)/doxygen.log || { cat $(CHECK_DIR)/doxygen.log; exit 1; }
	@printf '== docs: docs/html generated, %s warnings in %s\n' "$$(wc -l < $(CHECK_DIR)/doxygen.log)" '$(CHECK_DIR)/doxygen.log'

clean:
	@for p in $(PROJECTS); do $(MAKE) -s -C "$$p" clean >/dev/null; done
	@rm -rf $(CHECK_DIR) docs/html
