/*
 * Alignment gate (make check, check-align): accesses through pointers to
 * byte-aligned data.
 *
 * The compiler cannot know these pointers are even, so it must not read or
 * write them with word or long accesses. Every m68k-linux GCC does (KB-68),
 * an address error on a 68000 when the pointer is odd.
 */

struct rgb { unsigned char r, g, b; };
struct rgb2 { struct rgb a, b; };
struct pk16 { char c; short s; } __attribute__((packed));
struct pk32 { char c; long l; } __attribute__((packed));

extern struct rgb2 align_global;

void align_copy2(char *d, char const *s) { __builtin_memcpy(d, s, 2); }
void align_copy4(char *d, char const *s) { __builtin_memcpy(d, s, 4); }
void align_copy8(char *d, char const *s) { __builtin_memcpy(d, s, 8); }
void align_struct3_copy(struct rgb *d, struct rgb const *s) { *d = *s; }
void align_struct6_copy(struct rgb2 *d) { *d = align_global; }
short align_packed16_read(struct pk16 const *p) { return p->s; }
void align_packed16_write(struct pk16 *p, short v) { p->s = v; }
long align_packed32_read(struct pk32 const *p) { return p->l; }
void align_zero4(char *d) { __builtin_memset(d, 0, 4); }
void align_zero6(char *d) { __builtin_memset(d, 0, 6); }

unsigned short align_load16(unsigned char const *p)
{
	unsigned short v;
	__builtin_memcpy(&v, p, 2);
	return v;
}

void align_store32(unsigned char *p, unsigned long v) { __builtin_memcpy(p, &v, 4); }
