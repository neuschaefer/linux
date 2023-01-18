// SPDX-License-Identifier: GPL-2.0
#define _LINUX_STRING_H_

#include <linux/compiler.h>	/* for inline */
#include <linux/types.h>	/* for size_t */
#include <linux/stddef.h>	/* for NULL */
#include <linux/linkage.h>
#include <asm/string.h>
#include "misc.h"

#define STATIC static
#define STATIC_RW_DATA	/* non-static please */

/* Diagnostic functions */
#ifdef DEBUG
#  define Assert(cond,msg) {if(!(cond)) error(msg);}
#  define Trace(x) fprintf x
#  define Tracev(x) {if (verbose) fprintf x ;}
#  define Tracevv(x) {if (verbose>1) fprintf x ;}
#  define Tracec(c,x) {if (verbose && (c)) fprintf x ;}
#  define Tracecv(c,x) {if (verbose>1 && (c)) fprintf x ;}
#else
#  define Assert(cond,msg)
#  define Trace(x)
#  define Tracev(x)
#  define Tracevv(x)
#  define Tracec(c,x)
#  define Tracecv(c,x)
#endif

/* Not needed, but used in some headers pulled in by decompressors */
extern char * strstr(const char * s1, const char *s2);
extern size_t strlen(const char *s);
extern int memcmp(const void *cs, const void *ct, size_t count);
extern char * strchrnul(const char *, int);

#if CONFIG_DEBUG_UNCOMPRESS
extern void putc(char c);

static void putc2(char c)
{
	if (c == '\n')
		putc('\r');
	putc(c);
}

void puts(const char *str)
{
	while (*str)
		putc2(*str++);
}

void puthex4(uint8_t x)
{
	putc("0123456789abcdef"[x & 0xf]);
}

void puthex8(uint8_t x)
{
	puthex4(x >> 4);
	puthex4(x);
}

void puthex16(uint16_t x)
{
	puthex8(x >> 8);
	puthex8(x);
}

void puthex32(uint32_t x)
{
	puthex16(x >> 16);
	puthex16(x);
}

void warn_slowpath_fmt(const char *file, const int line, unsigned taint, const char *fmt, ...)
{
	putc('>');
	puts(file); puts(":"); puthex32(line);
	if (fmt) {
		puts(": ");
		puts(fmt);
	}
	puts("\n");
}

#define assert(condition) do { \
	if (!(condition)) { \
		puts("assertion failure!\n"); \
		puts(__stringify(condition)); \
		puts("\n"); \
		while (1) /* halt */; \
	} \
} while(0)

#define ZSTD_DEPS_IO
#define ZSTD_DEBUG_PRINT(fmt, ...) puts(fmt)

#endif

#ifdef CONFIG_KERNEL_GZIP
#include "../../../../lib/decompress_inflate.c"
#endif

#ifdef CONFIG_KERNEL_LZO
#include "../../../../lib/decompress_unlzo.c"
#endif

#ifdef CONFIG_KERNEL_LZMA
#include "../../../../lib/decompress_unlzma.c"
#endif

#ifdef CONFIG_KERNEL_XZ
/* Prevent KASAN override of string helpers in decompressor */
#undef memmove
#define memmove memmove
#undef memcpy
#define memcpy memcpy
#include "../../../../lib/decompress_unxz.c"
#endif

#ifdef CONFIG_KERNEL_LZ4
#include "../../../../lib/decompress_unlz4.c"
#endif

#ifdef CONFIG_KERNEL_ZSTD
#include "../../../../lib/decompress_unzstd.c"
#endif

int do_decompress(u8 *input, int len, u8 *output, void (*error)(char *x))
{
	return __decompress(input, len, NULL, NULL, output, 0, NULL, error);
}
