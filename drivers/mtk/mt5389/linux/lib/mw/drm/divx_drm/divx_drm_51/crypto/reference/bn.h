/* crypto/bn/bn.h */
/* Copyright (C) 1995-1997 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#ifndef HEADER_XBN_H
#define HEADER_XBN_H

#ifndef __KERNEL__

//#include <openssl/e_os2.h>
//#ifndef OPENSSL_NO_FP_API
#ifdef _MTK_BUILD_
#include "libc/stdio.h"
#else
#include <stdio.h> /* FILE */
#endif
//#endif

#define assert(x) /* assert */

#else

#include "../../../divx_drm_adp_common.h" /* NULL */

#define assert(x) /* assert */

#include "x_os.h" /* malloc/free */
#define malloc      x_mem_alloc
#define free        x_mem_free

#ifndef FILE
#define FILE void /* FILE */
#endif

#endif

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef OPENSSL_SYS_VMS
#undef XBN_LLONG /* experimental, so far... */
#endif

#define XBN_MUL_COMBA
#define XBN_SQR_COMBA
//#define XBN_RECURSION

/* This next option uses the C libraries (2 word)/(1 word) function.
 * If it is not defined, I use my C version (which is slower).
 * The reason for this flag is that when the particular C compiler
 * library routine is used, and the library is linked with a different
 * compiler, the library is missing.  This mostly happens when the
 * library is built with gcc and then linked using normal cc.  This would
 * be a common occurrence because gcc normally produces code that is
 * 2 times faster than system compilers for the big number stuff.
 * For machines with only one compiler (or shared libraries), this should
 * be on.  Again this in only really a problem on machines
 * using "long long's", are 32bit, and are not using my assembler code. */
#if defined(OPENSSL_SYS_MSDOS) || defined(OPENSSL_SYS_WINDOWS) || \
    defined(OPENSSL_SYS_WIN32) || defined(linux)
# ifndef XBN_DIV2W
#  define XBN_DIV2W
# endif
#endif

/* assuming long is 64bit - this is the DEC Alpha
 * unsigned long long is only 64 bits :-(, don't define
 * XBN_LLONG for the DEC Alpha */
#ifdef SIXTY_FOUR_BIT_LONG
#define XBN_ULLONG	unsigned long long
#define XBN_ULONG	unsigned long
#define XBN_LONG		long
#define XBN_BITS		128
#define XBN_BYTES	8
#define XBN_BITS2	64
#define XBN_BITS4	32
#define XBN_MASK		(0xffffffffffffffffffffffffffffffffLL)
#define XBN_MASK2	(0xffffffffffffffffL)
#define XBN_MASK2l	(0xffffffffL)
#define XBN_MASK2h	(0xffffffff00000000L)
#define XBN_MASK2h1	(0xffffffff80000000L)
#define XBN_TBIT		(0x8000000000000000L)
#define XBN_DEC_CONV	(10000000000000000000UL)
#define XBN_DEC_FMT1	"%lu"
#define XBN_DEC_FMT2	"%019lu"
#define XBN_DEC_NUM	19
#endif

/* This is where the long long data type is 64 bits, but long is 32.
 * For machines where there are 64bit registers, this is the mode to use.
 * IRIX, on R4000 and above should use this mode, along with the relevant
 * assembler code :-).  Do NOT define XBN_LLONG.
 */
#ifdef SIXTY_FOUR_BIT
#undef XBN_LLONG
#undef XBN_ULLONG
#define XBN_ULONG	unsigned long long
#define XBN_LONG		long long
#define XBN_BITS		128
#define XBN_BYTES	8
#define XBN_BITS2	64
#define XBN_BITS4	32
#define XBN_MASK2	(0xffffffffffffffffLL)
#define XBN_MASK2l	(0xffffffffL)
#define XBN_MASK2h	(0xffffffff00000000LL)
#define XBN_MASK2h1	(0xffffffff80000000LL)
#define XBN_TBIT		(0x8000000000000000LL)
#define XBN_DEC_CONV	(10000000000000000000ULL)
#define XBN_DEC_FMT1	"%llu"
#define XBN_DEC_FMT2	"%019llu"
#define XBN_DEC_NUM	19
#endif

#define THIRTY_TWO_BIT // hmaxf ... use this one
#ifdef THIRTY_TWO_BIT
#if defined(OPENSSL_SYS_WIN32) && !defined(__GNUC__)
#define XBN_ULLONG	unsigned _int64
#else
#define XBN_ULLONG	unsigned long long
#endif
#define XBN_ULONG	unsigned long
#define XBN_LONG		long
#define XBN_BITS		64
#define XBN_BYTES	4
#define XBN_BITS2	32
#define XBN_BITS4	16
#ifdef OPENSSL_SYS_WIN32
/* VC++ doesn't like the LL suffix */
#define XBN_MASK		(0xffffffffffffffffL)
#else
#define XBN_MASK		(0xffffffffffffffffLL)
#endif
#define XBN_MASK2	(0xffffffffL)
#define XBN_MASK2l	(0xffff)
#define XBN_MASK2h1	(0xffff8000L)
#define XBN_MASK2h	(0xffff0000L)
#define XBN_TBIT		(0x80000000L)
#define XBN_DEC_CONV	(1000000000L)
#define XBN_DEC_FMT1	"%lu"
#define XBN_DEC_FMT2	"%09lu"
#define XBN_DEC_NUM	9
#endif

#ifdef SIXTEEN_BIT
#ifndef XBN_DIV2W
#define XBN_DIV2W
#endif
#define XBN_ULLONG	unsigned long
#define XBN_ULONG	unsigned short
#define XBN_LONG		short
#define XBN_BITS		32
#define XBN_BYTES	2
#define XBN_BITS2	16
#define XBN_BITS4	8
#define XBN_MASK		(0xffffffff)
#define XBN_MASK2	(0xffff)
#define XBN_MASK2l	(0xff)
#define XBN_MASK2h1	(0xff80)
#define XBN_MASK2h	(0xff00)
#define XBN_TBIT		(0x8000)
#define XBN_DEC_CONV	(100000)
#define XBN_DEC_FMT1	"%u"
#define XBN_DEC_FMT2	"%05u"
#define XBN_DEC_NUM	5
#endif

#ifdef EIGHT_BIT
#ifndef XBN_DIV2W
#define XBN_DIV2W
#endif
#define XBN_ULLONG	unsigned short
#define XBN_ULONG	unsigned char
#define XBN_LONG		char
#define XBN_BITS		16
#define XBN_BYTES	1
#define XBN_BITS2	8
#define XBN_BITS4	4
#define XBN_MASK		(0xffff)
#define XBN_MASK2	(0xff)
#define XBN_MASK2l	(0xf)
#define XBN_MASK2h1	(0xf8)
#define XBN_MASK2h	(0xf0)
#define XBN_TBIT		(0x80)
#define XBN_DEC_CONV	(100)
#define XBN_DEC_FMT1	"%u"
#define XBN_DEC_FMT2	"%02u"
#define XBN_DEC_NUM	2
#endif

#define XBN_DEFAULT_BITS	1280

#ifdef XBIGNUM
#undef XBIGNUM
#endif

#define XBN_FLG_MALLOCED		0x01
#define XBN_FLG_STATIC_DATA	0x02
#define XBN_FLG_FREE		0x8000	/* used for debuging */
#define XBN_set_flags(b,n)	((b)->flags|=(n))
#define XBN_get_flags(b,n)	((b)->flags&(n))

typedef struct xbignum_st
	{
	XBN_ULONG *d;	/* Pointer to an array of 'XBN_BITS2' bit chunks. */
	int top;	/* Index of last used d +1. */
	/* The next are internal book keeping for xbn_expand. */
	int dmax;	/* Size of the d array. */
	int neg;	/* one if the number is negative */
	int flags;
	} XBIGNUM;

/* Used for temp variables (declaration hidden in bn_lcl.h) */
typedef struct xbignum_ctx XBN_CTX;

typedef struct xbn_blinding_st
	{
	int init;
	XBIGNUM *A;
	XBIGNUM *Ai;
	XBIGNUM *mod; /* just a reference */
	} XBN_BLINDING;

/* Used for montgomery multiplication */
typedef struct xbn_mont_ctx_st
	{
	int ri;        /* number of bits in R */
	XBIGNUM RR;     /* used to convert to montgomery form */
	XBIGNUM N;      /* The modulus */
	XBIGNUM Ni;     /* R*(1/R mod N) - N*Ni = 1
	                * (Ni is only stored for bignum algorithm) */
	XBN_ULONG n0;   /* least significant word of Ni */
	int flags;
	} XBN_MONT_CTX;

/* Used for reciprocal division/mod functions
 * It cannot be shared between threads
 */
typedef struct xbn_recp_ctx_st
	{
	XBIGNUM N;	/* the divisor */
	XBIGNUM Nr;	/* the reciprocal */
	int num_bits;
	int shift;
	int flags;
	} XBN_RECP_CTX;

#define XBN_prime_checks 0 /* default: select number of iterations
			     based on the size of the number */

/* number of Miller-Rabin iterations for an error rate  of less than 2^-80
 * for random 'b'-bit input, b >= 100 (taken from table 4.4 in the Handbook
 * of Applied Cryptography [Menezes, van Oorschot, Vanstone; CRC Press 1996];
 * original paper: Damgaard, Landrock, Pomerance: Average case error estimates
 * for the strong probable prime test. -- Math. Comp. 61 (1993) 177-194) */
#define XBN_prime_checks_for_size(b) ((b) >= 1300 ?  2 : \
                                (b) >=  850 ?  3 : \
                                (b) >=  650 ?  4 : \
                                (b) >=  550 ?  5 : \
                                (b) >=  450 ?  6 : \
                                (b) >=  400 ?  7 : \
                                (b) >=  350 ?  8 : \
                                (b) >=  300 ?  9 : \
                                (b) >=  250 ? 12 : \
                                (b) >=  200 ? 15 : \
                                (b) >=  150 ? 18 : \
                                /* b >= 100 */ 27)

#define XBN_num_bytes(a)	((XBN_num_bits(a)+7)/8)

/* Note that XBN_abs_is_word does not work reliably for w == 0 */
#define XBN_abs_is_word(a,w) (((a)->top == 1) && ((a)->d[0] == (XBN_ULONG)(w)))
#define XBN_is_zero(a)       (((a)->top == 0) || XBN_abs_is_word(a,0))
#define XBN_is_one(a)        (XBN_abs_is_word((a),1) && !(a)->neg)
#define XBN_is_word(a,w)     ((w) ? XBN_abs_is_word((a),(w)) && !(a)->neg : \
                                   XBN_is_zero((a)))
#define XBN_is_odd(a)	    (((a)->top > 0) && ((a)->d[0] & 1))

#define XBN_one(a)	(XBN_set_word((a),1))
#define XBN_zero(a)	(XBN_set_word((a),0))

/*#define XBN_ascii2bn(a)	XBN_hex2bn(a) */
/*#define XBN_bn2ascii(a)	XBN_bn2hex(a) */
struct _xbn_limits
{
   int xbn_limit_bits;
   int xbn_limit_num;        /* (1<<xbn_limit_bits) */
   int xbn_limit_bits_low;
   int xbn_limit_num_low;    /* (1<<xbn_limit_bits_low) */
   int xbn_limit_bits_high;
   int xbn_limit_num_high;   /* (1<<xbn_limit_bits_high) */
   int xbn_limit_bits_mont;
   int xbn_limit_num_mont;   /* (1<<xbn_limit_bits_mont) */
};
typedef struct _xbn_limits xbn_limits_t;

struct XBN_Context_t
    {
    int init;
    char data[16];
    xbn_limits_t xbn_limits;
    };
typedef struct XBN_Context_t XBN_Context;

const XBIGNUM *XBN_value_one(void);
void XBN_init_options(XBN_Context* context);
char *XBN_options(XBN_Context* context);
XBN_CTX *XBN_CTX_new(void);
void	XBN_CTX_init(XBN_CTX *c);
void	XBN_CTX_free(XBN_CTX *c);
void	XBN_CTX_start(XBN_CTX *ctx);
XBIGNUM *XBN_CTX_get(XBN_CTX *ctx);
void	XBN_CTX_end(XBN_CTX *ctx);

int XBN_generate_random_bit(XBIGNUM *target, int bits, int top, int bottom);
/* hmaxf
int     XBN_rand(XBIGNUM *rnd, int bits, int top,int bottom);
int     XBN_pseudo_rand(XBIGNUM *rnd, int bits, int top,int bottom);
int	XBN_rand_range(XBIGNUM *rnd, XBIGNUM *range);
int	XBN_pseudo_rand_range(XBIGNUM *rnd, XBIGNUM *range);
*/

int	XBN_num_bits(const XBIGNUM *a);
int	XBN_num_bits_word(XBN_ULONG);
XBIGNUM *XBN_new(void);
void	XBN_init(XBIGNUM *);
void	XBN_clear_free(XBIGNUM *a);
XBIGNUM *XBN_copy(XBIGNUM *a, const XBIGNUM *b);
void	XBN_swap(XBIGNUM *a, XBIGNUM *b);
XBIGNUM *XBN_bin2bn(const unsigned char *s,int len,XBIGNUM *ret);
int	XBN_bn2bin(const XBIGNUM *a, unsigned char *to);
XBIGNUM *XBN_mpi2bn(const unsigned char *s,int len,XBIGNUM *ret);
int	XBN_bn2mpi(const XBIGNUM *a, unsigned char *to);
int	XBN_sub(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b);
int	XBN_usub(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b);
int	XBN_uadd(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b);
int	XBN_add(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b);
int	XBN_mul(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, XBN_CTX *ctx);
int	XBN_sqr(XBIGNUM *r, const XBIGNUM *a,XBN_CTX *ctx);

int	XBN_div(XBIGNUM *dv, XBIGNUM *rem, const XBIGNUM *m, const XBIGNUM *d,
	XBN_CTX *ctx);
#define XBN_mod(rem,m,d,ctx) XBN_div(NULL,(rem),(m),(d),(ctx))
int	XBN_nnmod(XBIGNUM *r, const XBIGNUM *m, const XBIGNUM *d, XBN_CTX *ctx);
int	XBN_mod_add(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m, XBN_CTX *ctx);
int	XBN_mod_add_quick(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m);
int	XBN_mod_sub(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m, XBN_CTX *ctx);
int	XBN_mod_sub_quick(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m);
int	XBN_mod_mul(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b,
	const XBIGNUM *m, XBN_CTX *ctx);
int	XBN_mod_sqr(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *m, XBN_CTX *ctx);
int	XBN_mod_lshift1(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *m, XBN_CTX *ctx);
int	XBN_mod_lshift1_quick(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *m);
int	XBN_mod_lshift(XBIGNUM *r, const XBIGNUM *a, int n, const XBIGNUM *m, XBN_CTX *ctx);
int	XBN_mod_lshift_quick(XBIGNUM *r, const XBIGNUM *a, int n, const XBIGNUM *m);

XBN_ULONG XBN_mod_word(const XBIGNUM *a, XBN_ULONG w);
XBN_ULONG XBN_div_word(XBIGNUM *a, XBN_ULONG w);
int	XBN_mul_word(XBIGNUM *a, XBN_ULONG w);
int	XBN_add_word(XBIGNUM *a, XBN_ULONG w);
int	XBN_sub_word(XBIGNUM *a, XBN_ULONG w);
int	XBN_set_word(XBIGNUM *a, XBN_ULONG w);
XBN_ULONG XBN_get_word(const XBIGNUM *a);

int	XBN_cmp(const XBIGNUM *a, const XBIGNUM *b);
void	XBN_free(XBIGNUM *a);
int	XBN_is_bit_set(const XBIGNUM *a, int n);
int	XBN_lshift(XBIGNUM *r, const XBIGNUM *a, int n);
int	XBN_lshift1(XBIGNUM *r, const XBIGNUM *a);
int	XBN_exp(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *p,XBN_CTX *ctx);

int	XBN_mod_exp(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *p,
	const XBIGNUM *m,XBN_CTX *ctx);
int	XBN_mod_exp_mont(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *p,
	const XBIGNUM *m, XBN_CTX *ctx, XBN_MONT_CTX *m_ctx);
int	XBN_mod_exp_mont_word(XBIGNUM *r, XBN_ULONG a, const XBIGNUM *p,
	const XBIGNUM *m, XBN_CTX *ctx, XBN_MONT_CTX *m_ctx);
int	XBN_mod_exp2_mont(XBIGNUM *r, const XBIGNUM *a1, const XBIGNUM *p1,
	const XBIGNUM *a2, const XBIGNUM *p2,const XBIGNUM *m,
	XBN_CTX *ctx,XBN_MONT_CTX *m_ctx);
int	XBN_mod_exp_simple(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *p,
	const XBIGNUM *m,XBN_CTX *ctx);

int	XBN_mask_bits(XBIGNUM *a,int n);
#ifndef OPENSSL_NO_FP_API
int	XBN_print_fp(FILE *fp, const XBIGNUM *a);
#endif
#ifdef HEADER_BIO_H
int	XBN_print(BIO *fp, const XBIGNUM *a);
#else
int	XBN_print(void *fp, const XBIGNUM *a);
#endif
int	XBN_reciprocal(XBIGNUM *r, const XBIGNUM *m, int len, XBN_CTX *ctx);
int	XBN_rshift(XBIGNUM *r, const XBIGNUM *a, int n);
int	XBN_rshift1(XBIGNUM *r, const XBIGNUM *a);
void	XBN_clear(XBIGNUM *a);
XBIGNUM *XBN_dup(const XBIGNUM *a);
int	XBN_ucmp(const XBIGNUM *a, const XBIGNUM *b);
int	XBN_set_bit(XBIGNUM *a, int n);
int	XBN_clear_bit(XBIGNUM *a, int n);
char *	XBN_bn2hex(const XBIGNUM *a);
char *	XBN_bn2dec(const XBIGNUM *a);
int 	XBN_hex2bn(XBIGNUM **a, const char *str);
int 	XBN_dec2bn(XBIGNUM **a, const char *str);
int	XBN_gcd(XBIGNUM *r,const XBIGNUM *a,const XBIGNUM *b,XBN_CTX *ctx);
int	XBN_kronecker(const XBIGNUM *a,const XBIGNUM *b,XBN_CTX *ctx); /* returns -2 for error */
XBIGNUM *XBN_mod_inverse(XBIGNUM *ret,
	const XBIGNUM *a, const XBIGNUM *n,XBN_CTX *ctx);
XBIGNUM *XBN_mod_sqrt(XBIGNUM *ret,
	const XBIGNUM *a, const XBIGNUM *n,XBN_CTX *ctx);
XBIGNUM *XBN_generate_prime(XBIGNUM *ret,int bits,int safe,
	const XBIGNUM *add, const XBIGNUM *rem,
	void (*callback)(int,int,void *),void *cb_arg);
int	XBN_is_prime(const XBIGNUM *p,int nchecks,
	void (*callback)(int,int,void *),
	XBN_CTX *ctx,void *cb_arg);
int	XBN_is_prime_fasttest(const XBIGNUM *p,int nchecks,
	void (*callback)(int,int,void *),XBN_CTX *ctx,void *cb_arg,
	int do_trial_division);

XBN_MONT_CTX *XBN_MONT_CTX_new(void );
void XBN_MONT_CTX_init(XBN_MONT_CTX *ctx);
int XBN_mod_mul_montgomery(XBIGNUM *r,const XBIGNUM *a,const XBIGNUM *b,
	XBN_MONT_CTX *mont, XBN_CTX *ctx);
#define XBN_to_montgomery(r,a,mont,ctx)	XBN_mod_mul_montgomery(\
	(r),(a),&((mont)->RR),(mont),(ctx))
int XBN_from_montgomery(XBIGNUM *r,const XBIGNUM *a,
	XBN_MONT_CTX *mont, XBN_CTX *ctx);
void XBN_MONT_CTX_free(XBN_MONT_CTX *mont);
int XBN_MONT_CTX_set(XBN_MONT_CTX *mont,const XBIGNUM *modulus,XBN_CTX *ctx);
XBN_MONT_CTX *XBN_MONT_CTX_copy(XBN_MONT_CTX *to,XBN_MONT_CTX *from);

XBN_BLINDING *XBN_BLINDING_new(XBIGNUM *A,XBIGNUM *Ai,XBIGNUM *mod);
void XBN_BLINDING_free(XBN_BLINDING *b);
int XBN_BLINDING_update(XBN_BLINDING *b,XBN_CTX *ctx);
int XBN_BLINDING_convert(XBIGNUM *n, XBN_BLINDING *r, XBN_CTX *ctx);
int XBN_BLINDING_invert(XBIGNUM *n, XBN_BLINDING *b, XBN_CTX *ctx);

void XBN_set_params(XBN_Context* context, int mul,int high,int low,int mont);
int XBN_get_params(XBN_Context* context, int which); /* 0, mul, 1 high, 2 low, 3 mont */

void	XBN_RECP_CTX_init(XBN_RECP_CTX *recp);
XBN_RECP_CTX *XBN_RECP_CTX_new(void);
void	XBN_RECP_CTX_free(XBN_RECP_CTX *recp);
int	XBN_RECP_CTX_set(XBN_RECP_CTX *recp,const XBIGNUM *rdiv,XBN_CTX *ctx);
int	XBN_mod_mul_reciprocal(XBIGNUM *r, const XBIGNUM *x, const XBIGNUM *y,
	XBN_RECP_CTX *recp,XBN_CTX *ctx);
int	XBN_mod_exp_recp(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *p,
	const XBIGNUM *m, XBN_CTX *ctx);
int	XBN_div_recp(XBIGNUM *dv, XBIGNUM *rem, const XBIGNUM *m,
	XBN_RECP_CTX *recp, XBN_CTX *ctx);

/* library internal functions */

#define xbn_expand(a,bits) ((((((bits+XBN_BITS2-1))/XBN_BITS2)) <= (a)->dmax)?\
	(a):xbn_expand2((a),(bits)/XBN_BITS2+1))
#define xbn_wexpand(a,words) (((words) <= (a)->dmax)?(a):xbn_expand2((a),(words)))
XBIGNUM *xbn_expand2(XBIGNUM *a, int words);
XBIGNUM *xbn_dup_expand(const XBIGNUM *a, int words);

#define xbn_fix_top(a) \
        { \
        XBN_ULONG *ftl; \
	if ((a)->top > 0) \
		{ \
		for (ftl= &((a)->d[(a)->top-1]); (a)->top > 0; (a)->top--) \
		if (*(ftl--)) break; \
		} \
	}

XBN_ULONG xbn_mul_add_words(XBN_ULONG *rp, const XBN_ULONG *ap, int num, XBN_ULONG w);
XBN_ULONG xbn_mul_words(XBN_ULONG *rp, const XBN_ULONG *ap, int num, XBN_ULONG w);
void     xbn_sqr_words(XBN_ULONG *rp, const XBN_ULONG *ap, int num);
XBN_ULONG xbn_div_words(XBN_ULONG h, XBN_ULONG l, XBN_ULONG d);
XBN_ULONG xbn_add_words(XBN_ULONG *rp, const XBN_ULONG *ap, const XBN_ULONG *bp,int num);
XBN_ULONG xbn_sub_words(XBN_ULONG *rp, const XBN_ULONG *ap, const XBN_ULONG *bp,int num);

#ifdef XBN_DEBUG
void xbn_dump1(FILE *o, const char *a, const XBN_ULONG *b,int n);
# define xbn_print(a) {fprintf(stderr, #a "="); XBN_print_fp(stderr,a); \
   fprintf(stderr,"\n");}
# define xbn_dump(a,n) xbn_dump1(stderr,#a,a,n);
#else
# define xbn_print(a)
# define xbn_dump(a,b)
#endif

int XBN_bntest_rand(XBIGNUM *rnd, int bits, int top,int bottom);

/* BEGIN ERROR CODES */
/* The following lines are auto generated by the script mkerr.pl. Any changes
 * made after this point may be overwritten when the script is next run.
 */
void ERR_load_XBN_strings(void);

/* Error codes for the BN functions. */

/* Function codes. */
#define XBN_F_XBN_BLINDING_CONVERT			 100
#define XBN_F_XBN_BLINDING_INVERT				 101
#define XBN_F_XBN_BLINDING_NEW				 102
#define XBN_F_XBN_BLINDING_UPDATE				 103
#define XBN_F_XBN_BN2DEC					 104
#define XBN_F_XBN_BN2HEX					 105
#define XBN_F_XBN_CTX_GET					 116
#define XBN_F_XBN_CTX_NEW					 106
#define XBN_F_XBN_DIV					 107
#define XBN_F_XBN_EXPAND2					 108
#define XBN_F_XBN_EXPAND_INTERNAL				 120
#define XBN_F_XBN_MOD_EXP2_MONT				 118
#define XBN_F_XBN_MOD_EXP_MONT				 109
#define XBN_F_XBN_MOD_EXP_MONT_WORD			 117
#define XBN_F_XBN_MOD_INVERSE				 110
#define XBN_F_XBN_MOD_LSHIFT_QUICK			 119
#define XBN_F_XBN_MOD_MUL_RECIPROCAL			 111
#define XBN_F_XBN_MOD_SQRT				 121
#define XBN_F_XBN_MPI2BN					 112
#define XBN_F_XBN_NEW					 113
#define XBN_F_XBN_RAND					 114
#define XBN_F_XBN_RAND_RANGE				 122
#define XBN_F_XBN_USUB					 115

/* Reason codes. */
#define XBN_R_ARG2_LT_ARG3				 100
#define XBN_R_BAD_RECIPROCAL				 101
#define XBN_R_XBIGNUM_TOO_LONG				 114
#define XBN_R_CALLED_WITH_EVEN_MODULUS			 102
#define XBN_R_DIV_BY_ZERO				 103
#define XBN_R_ENCODING_ERROR				 104
#define XBN_R_EXPAND_ON_STATIC_XBIGNUM_DATA		 105
#define XBN_R_INPUT_NOT_REDUCED				 110
#define XBN_R_INVALID_LENGTH				 106
#define XBN_R_INVALID_RANGE				 115
#define XBN_R_NOT_A_SQUARE				 111
#define XBN_R_NOT_INITIALIZED				 107
#define XBN_R_NO_INVERSE					 108
#define XBN_R_P_IS_NOT_PRIME				 112
#define XBN_R_TOO_MANY_ITERATIONS			 113
#define XBN_R_TOO_MANY_TEMPORARY_VARIABLES		 109

#ifdef  __cplusplus
}
#endif
#endif
