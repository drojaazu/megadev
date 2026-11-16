# Alignment gate scanner (make check, check-align). Reads `objdump -d` output
# (MIT syntax) of the check/align objects and prints each word or long access
# that may be misaligned on a 68000, as "function address: instruction".
#
# A word or long access is accepted only when its address is known to be
# even: a stack or frame pointer (sp, fp/a6) base with an even displacement,
# including push and pop. Any other memory operand (an address register, the
# PC, an absolute address) is reported, since everything the check/align units
# reach that way is byte aligned. Registers, immediates and register lists
# are not memory.

/^[0-9a-f]+ <.*>:$/ { fn = $2; gsub(/[<>:]/, "", fn); next }

# Instruction lines: address, encoding, then "mnemonic operands".
/^ *[0-9a-f]+:\t/ {
	n = split($0, f, "\t")
	if (n < 3) next
	ins = f[3]
	op = ins; sub(/ .*/, "", op)
	args = ins; if (!sub(/^[^ ]+ +/, "", args)) args = ""

	# Byte, quick and unsized operations, and those that do not access data
	# memory at their operands.
	if (op !~ /[wl]$/) next
	if (op ~ /^(lea|pea|link|unlk|movep|jsr|jmp|j[a-z]+)/) next
	if (op ~ /^(b(ra|sr|hi|ls|cc|cs|ne|eq|vc|vs|pl|mi|ge|lt|gt|le)|db[a-z]+)[swl]$/) next

	# Split the operands at commas outside parentheses.
	k = 0; depth = 0; cur = ""
	for (i = 1; i <= length(args); i++) {
		c = substr(args, i, 1)
		if (c == "(") depth++
		else if (c == ")") depth--
		if (c == "," && depth == 0) { opnd[++k] = cur; cur = ""; continue }
		cur = cur c
	}
	if (cur != "") opnd[++k] = cur

	bad = 0
	for (j = 1; j <= k; j++) {
		o = opnd[j]
		if (o ~ /^#/) continue
		if (o ~ /^%/ && o !~ /@/) continue
		if (o ~ /^%(sp|fp|a6|a7)@([-+]|$)/) continue
		if (o ~ /^%(sp|fp|a6|a7)@\(/) {
			d = o; sub(/^%[a-z0-9]+@\(/, "", d); sub(/[,)].*/, "", d)
			if (d ~ /^-?[0-9]+$/ && d % 2 == 0) continue
		}
		bad = 1
	}
	if (bad) { a = f[1]; gsub(/[ :]/, "", a); printf "%s %s: %s\n", fn, a, ins }
}
