/*
 * Alignment gate (make check, check-align): locals the compiler initializes.
 *
 * Every m68k-linux GCC initializes these with word or long stores at odd
 * stack offsets (KB-68), an address error on a 68000. The Megadev toolchain
 * must use byte stores, or word and long stores at even offsets only.
 */

void use(void *);

void align_char3_zero(void) { char a[3] = {0}; use(a); }
void align_char3_string(void) { char a[3] = "ab"; use(a); }
void align_char7_list(void) { char a[7] = {1, 2, 3, 4, 5, 6, 7}; use(a); }
void align_struct3_zero(void) { struct { char x, y, z; } s = {0}; use(&s); }
void align_char_then_char5(void) { char c = 1; char a[5] = {0}; use(&c); use(a); }

void align_two_char3(void)
{
	char buf[9];
	unsigned char a[3] = {0}, b[3] = {0};
	use(a);
	use(b);
	use(buf);
}
