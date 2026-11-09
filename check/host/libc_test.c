/*
 * Host-side test of the freestanding C library subset in lib/runtime.
 *
 * The runtime sources are plain C, so they compile and run on the build host.
 * They are included here with their symbols prefixed, so they coexist with
 * the host's own libc, and every function is checked against known answers.
 * Built and run by `make check`.
 */

#define memcpy md_memcpy
#define memmove md_memmove
#define memset md_memset
#define memcmp md_memcmp
#define strlen md_strlen
#define strcmp md_strcmp
#define strncmp md_strncmp
#define strcpy md_strcpy
#define strncpy md_strncpy
#define strcat md_strcat
#define strchr md_strchr
#define atoi md_atoi
#define strtol md_strtol
#define rand md_rand
#define srand md_srand
#define bsearch md_bsearch
#define qsort md_qsort
#define vsnprintf md_vsnprintf
#define snprintf md_snprintf

#include "../../lib/runtime/snprintf.c"
#include "../../lib/runtime/rand.c"
#include "../../lib/runtime/sort.c"
#include "../../lib/runtime/strtol.c"
#include "../../lib/runtime/string.c"
#include "../../lib/runtime/text_expand.c"
#include "../../lib/runtime/sysfont.c"

/* The target's memcpy, memmove, memset and memcmp are 68000 assembly and cannot
 * run here; these are reference implementations with the same contract, so the
 * functions above that call them can be tested. */
void * memcpy(void * dest, void const * src, size_t n)
{
  char * d = dest;
  char const * s = src;
  while (n--)
    *d++ = *s++;
  return dest;
}

void * memmove(void * dest, void const * src, size_t n)
{
  char * d = dest;
  char const * s = src;
  if (d <= s)
    while (n--)
      *d++ = *s++;
  else
    while (n--)
      d[n] = s[n];
  return dest;
}

void * memset(void * dest, int value, size_t n)
{
  char * d = dest;
  while (n--)
    *d++ = (char) value;
  return dest;
}

int memcmp(void const * a, void const * b, size_t n)
{
  unsigned char const * x = a;
  unsigned char const * y = b;
  for (; n; --n, ++x, ++y)
    if (*x != *y)
      return *x - *y;
  return 0;
}

int printf(char const * fmt, ...);

static int failures;

#define CHECK(cond)                                           \
  do                                                          \
  {                                                           \
    if (!(cond))                                              \
    {                                                         \
      printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      ++failures;                                             \
    }                                                         \
  } while (0)

static int compare_int(void const * a, void const * b)
{
  int const x = *(int const *) a, y = *(int const *) b;
  return x < y ? -1 : x > y;
}

static void test_string(void)
{
  char buf[32];

  CHECK(strlen("") == 0);
  CHECK(strlen("megadev") == 7);

  CHECK(strcmp("abc", "abc") == 0);
  CHECK(strcmp("abc", "abd") < 0);
  CHECK(strcmp("abd", "abc") > 0);
  CHECK(strcmp("ab", "abc") < 0);
  CHECK(strcmp("", "") == 0);
  CHECK(strcmp("\x80", "\x01") > 0);  // compares as unsigned

  CHECK(strncmp("abcdef", "abcxyz", 3) == 0);
  CHECK(strncmp("abcdef", "abcxyz", 4) < 0);
  CHECK(strncmp("abc", "abc", 10) == 0);
  CHECK(strncmp("a", "b", 0) == 0);

  CHECK(strcpy(buf, "hello") == buf);
  CHECK(strcmp(buf, "hello") == 0);

  memset(buf, 'x', sizeof buf);
  CHECK(strncpy(buf, "hi", 5) == buf);
  CHECK(buf[0] == 'h' && buf[1] == 'i' && buf[2] == 0 && buf[3] == 0 && buf[4] == 0 && buf[5] == 'x');
  memset(buf, 'x', sizeof buf);
  strncpy(buf, "toolong", 4);
  CHECK(buf[3] == 'l' && buf[4] == 'x');  // no terminator when n is exhausted

  strcpy(buf, "foo");
  CHECK(strcat(buf, "bar") == buf);
  CHECK(strcmp(buf, "foobar") == 0);

  CHECK(strchr("hello", 'l') == "hello" + 2 || *strchr("hello", 'l') == 'l');
  CHECK(strchr("hello", 'z') == NULL);
  CHECK(*strchr("hello", '\0') == '\0');  // the terminator is findable

  CHECK(memcmp("abc", "abd", 3) < 0);
  CHECK(memcmp("abc", "abc", 3) == 0);
  CHECK(memcmp("\xff", "\x01", 1) > 0);
  CHECK(memcmp("abc", "xyz", 0) == 0);

  char src[8] = "0123456";
  memmove(src + 1, src, 6);
  CHECK(strcmp(src, "0012345") == 0);  // overlapping, forward source
  memmove(src, src + 1, 6);
  CHECK(strcmp(src, "0123455") == 0);  // overlapping, backward source

  memset(buf, 0x5A, 4);
  CHECK(buf[0] == 0x5A && buf[3] == 0x5A);
  CHECK(memset(buf, 0, 0) == buf);  // zero length

  memcpy(buf, "copy", 5);
  CHECK(strcmp(buf, "copy") == 0);
}

static void test_stdlib(void)
{
  char * end;

  CHECK(atoi("0") == 0);
  CHECK(atoi("42") == 42);
  CHECK(atoi("  -17xyz") == -17);
  CHECK(atoi("+8") == 8);
  CHECK(atoi("abc") == 0);

  CHECK(strtol("ff", NULL, 16) == 255);
  CHECK(strtol("0xFF", NULL, 16) == 255);
  CHECK(strtol("0xff", NULL, 0) == 255);
  CHECK(strtol("077", NULL, 0) == 63);
  CHECK(strtol("77", NULL, 0) == 77);
  CHECK(strtol("1010", NULL, 2) == 10);
  CHECK(strtol("zz", NULL, 36) == 35 * 36 + 35);
  CHECK(strtol("12ab", &end, 10) == 12 && *end == 'a');
  CHECK(strtol("nope", &end, 10) == 0 && end[0] == 'n');
  CHECK(strtol("-0x10", NULL, 0) == -16);
  CHECK(strtol("99999999999999999999", NULL, 10) == LONG_MAX);
  CHECK(strtol("-99999999999999999999", NULL, 10) == LONG_MIN);

  CHECK(abs(-5) == 5 && abs(5) == 5 && labs(-7L) == 7);
  div_t const d = div(17, 5);
  CHECK(d.quot == 3 && d.rem == 2);
  div_t const dn = div(-17, 5);
  CHECK(dn.quot == -3 && dn.rem == -2);
  ldiv_t const ld = ldiv(100000L, 7L);
  CHECK(ld.quot == 14285 && ld.rem == 5);

  srand(1);
  int const r1 = rand(), r2 = rand();
  srand(1);
  CHECK(rand() == r1 && rand() == r2);  // deterministic
  CHECK(r1 != r2);
  CHECK(r1 >= 0 && r2 >= 0);
  srand(0);
  int const r0 = rand();
  srand(0);
  CHECK(rand() == r0);  // seed 0 is replaced by a fixed seed, deterministically
  int nonzero = 0;
  for (int i = 0; i < 100; ++i)
    nonzero |= rand() != 0;
  CHECK(nonzero);

  int sorted[] = {1, 3, 5, 7, 9, 11};
  int key = 7;
  int * found = bsearch(&key, sorted, 6, sizeof(int), compare_int);
  CHECK(found && *found == 7 && found == &sorted[3]);
  key = 4;
  CHECK(bsearch(&key, sorted, 6, sizeof(int), compare_int) == NULL);
  key = 1;
  CHECK(bsearch(&key, sorted, 6, sizeof(int), compare_int) == &sorted[0]);
  key = 11;
  CHECK(bsearch(&key, sorted, 6, sizeof(int), compare_int) == &sorted[5]);
  CHECK(bsearch(&key, sorted, 0, sizeof(int), compare_int) == NULL);

  int unsorted[] = {5, -2, 9, 0, 3, 3, -8, 1};
  qsort(unsorted, 8, sizeof(int), compare_int);
  int const expected[] = {-8, -2, 0, 1, 3, 3, 5, 9};
  CHECK(memcmp(unsorted, expected, sizeof expected) == 0);
  qsort(unsorted, 0, sizeof(int), compare_int);
  qsort(unsorted, 1, sizeof(int), compare_int);
  CHECK(unsorted[0] == -8);
}

static void test_snprintf(void)
{
  char buf[64];

  CHECK(snprintf(buf, sizeof buf, "plain") == 5 && strcmp(buf, "plain") == 0);
  CHECK(snprintf(buf, sizeof buf, "%d", 0) == 1 && strcmp(buf, "0") == 0);
  CHECK(snprintf(buf, sizeof buf, "%d|%i", 123, -456) == 8 && strcmp(buf, "123|-456") == 0);
  CHECK(snprintf(buf, sizeof buf, "%u", 4000000000U) == 10 && strcmp(buf, "4000000000") == 0);
  CHECK(snprintf(buf, sizeof buf, "%x %X", 0xBEEF, 0xBEEF) == 9 && strcmp(buf, "beef BEEF") == 0);
  CHECK(snprintf(buf, sizeof buf, "%ld", -2147483647L - 1) == 11 && strcmp(buf, "-2147483648") == 0);
  CHECK(snprintf(buf, sizeof buf, "%lu", 4294967295UL) == 10 && strcmp(buf, "4294967295") == 0);
  CHECK(snprintf(buf, sizeof buf, "%lx", 0xFFFFFFFFUL) == 8 && strcmp(buf, "ffffffff") == 0);
  CHECK(snprintf(buf, sizeof buf, "%c%c", 'o', 'k') == 2 && strcmp(buf, "ok") == 0);
  CHECK(snprintf(buf, sizeof buf, "[%s]", "str") == 5 && strcmp(buf, "[str]") == 0);
  CHECK(snprintf(buf, sizeof buf, "100%%") == 4 && strcmp(buf, "100%") == 0);

  CHECK(snprintf(buf, sizeof buf, "%5d|", 42) == 6 && strcmp(buf, "   42|") == 0);
  CHECK(snprintf(buf, sizeof buf, "%-5d|", 42) == 6 && strcmp(buf, "42   |") == 0);
  CHECK(snprintf(buf, sizeof buf, "%05d|", 42) == 6 && strcmp(buf, "00042|") == 0);
  CHECK(snprintf(buf, sizeof buf, "%05d|", -42) == 6 && strcmp(buf, "-0042|") == 0);
  CHECK(snprintf(buf, sizeof buf, "%04x", 0xAB) == 4 && strcmp(buf, "00ab") == 0);
  CHECK(snprintf(buf, sizeof buf, "%8s|", "right") == 9 && strcmp(buf, "   right|") == 0);
  CHECK(snprintf(buf, sizeof buf, "%-8s|", "left") == 9 && strcmp(buf, "left    |") == 0);
  CHECK(snprintf(buf, sizeof buf, "%2d", 12345) == 5 && strcmp(buf, "12345") == 0);  // width is a minimum

  // Truncation: return value is the full length, output is cut and terminated
  CHECK(snprintf(buf, 4, "abcdef") == 6 && strcmp(buf, "abc") == 0);
  CHECK(snprintf(buf, 1, "abc") == 3 && buf[0] == '\0');
  CHECK(snprintf(buf, 0, "abc") == 3);
  CHECK(snprintf(buf, 6, "%d", 1234567) == 7 && strcmp(buf, "12345") == 0);

  CHECK(snprintf(buf, sizeof buf, "%q") == 2 && strcmp(buf, "%q") == 0);  // unknown conversion is literal
  CHECK(snprintf(buf, sizeof buf, "x%") == 1 && strcmp(buf, "x") == 0);   // trailing % is dropped
}

static void test_text(void)
{
  // 1bpp to 4bpp: leftmost pixel in bit 7 lands in the top nibble
  CHECK(text_expand_row(0x00, 1, 0) == 0x00000000);
  CHECK(text_expand_row(0xFF, 1, 0) == 0x11111111);
  CHECK(text_expand_row(0x80, 0xF, 0) == 0xF0000000);
  CHECK(text_expand_row(0x01, 0xF, 0) == 0x0000000F);
  CHECK(text_expand_row(0xA5, 3, 2) == 0x32322323);
  CHECK(text_expand_row(0x0F, 0x1E, 0x12) == 0x2222EEEE);  // colors are masked to 4 bits

  // the system font: 'A' starts with two set pixels at columns 2-3, space is empty
  CHECK(sysfont['A' - 0x20][0] == 0x30);
  CHECK(sysfont[0][0] == 0 && sysfont[0][7] == 0);
  CHECK(sysfont['!' - 0x20][0] == 0x18);  // symmetric: centered in either bit order
}

int main(void)
{
  test_string();
  test_stdlib();
  test_snprintf();
  test_text();
  if (failures)
  {
    printf("libc test: %d failure(s)\n", failures);
    return 1;
  }
  return 0;
}
