/* crypto/bn/xbn_mod.c */
/* Includes code written by Lenka Fibikova <fibikova@exp-math.uni-essen.de>
 * for the OpenSSL project. */
/* ====================================================================
 * Copyright (c) 1998-2000 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
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

//#include "cryptlib.h"
#include "bn_lcl.h"


#if 0 /* now just a #define */
int XBN_mod(XBIGNUM *rem, const XBIGNUM *m, const XBIGNUM *d, XBN_CTX *ctx)
	{
	return(XBN_div(NULL,rem,m,d,ctx));
	/* note that  rem->neg == m->neg  (unless the remainder is zero) */
	}
#endif


int XBN_nnmod(XBIGNUM *r, const XBIGNUM *m, const XBIGNUM *d, XBN_CTX *ctx)
	{
	/* like XBN_mod, but returns non-negative remainder
	 * (i.e.,  0 <= r < |d|  always holds) */

	if (!(XBN_mod(r,m,d,ctx)))
		return 0;
	if (!r->neg)
		return 1;
	/* now   -|d| < r < 0,  so we have to set  r := r + |d| */
	return (d->neg ? XBN_sub : XBN_add)(r, r, d);
}


int XBN_mod_add(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m, XBN_CTX *ctx)
	{
	if (!XBN_add(r, a, b)) return 0;
	return XBN_nnmod(r, r, m, ctx);
	}


/* XBN_mod_add variant that may be used if both  a  and  b  are non-negative
 * and less than  m */
int XBN_mod_add_quick(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m)
	{
	if (!XBN_add(r, a, b)) return 0;
	if (XBN_ucmp(r, m) >= 0)
		return XBN_usub(r, r, m);
	return 1;
	}


int XBN_mod_sub(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m, XBN_CTX *ctx)
	{
	if (!XBN_sub(r, a, b)) return 0;
	return XBN_nnmod(r, r, m, ctx);
	}


/* XBN_mod_sub variant that may be used if both  a  and  b  are non-negative
 * and less than  m */
int XBN_mod_sub_quick(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m)
	{
	if (!XBN_sub(r, a, b)) return 0;
	if (r->neg)
		return XBN_add(r, r, m);
	return 1;
	}


/* slow but works */
int XBN_mod_mul(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b, const XBIGNUM *m,
	XBN_CTX *ctx)
	{
	XBIGNUM *t;
	int ret=0;

	xbn_check_top(a);
	xbn_check_top(b);
	xbn_check_top(m);

	XBN_CTX_start(ctx);
	if ((t = XBN_CTX_get(ctx)) == NULL) goto err;
	if (a == b)
		{ if (!XBN_sqr(t,a,ctx)) goto err; }
	else
		{ if (!XBN_mul(t,a,b,ctx)) goto err; }
	if (!XBN_nnmod(r,t,m,ctx)) goto err;
	ret=1;
err:
	XBN_CTX_end(ctx);
	return(ret);
	}


int XBN_mod_sqr(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *m, XBN_CTX *ctx)
	{
	if (!XBN_sqr(r, a, ctx)) return 0;
	/* r->neg == 0,  thus we don't need XBN_nnmod */
	return XBN_mod(r, r, m, ctx);
	}


int XBN_mod_lshift1(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *m, XBN_CTX *ctx)
	{
	if (!XBN_lshift1(r, a)) return 0;
	return XBN_nnmod(r, r, m, ctx);
	}


/* XBN_mod_lshift1 variant that may be used if  a  is non-negative
 * and less than  m */
int XBN_mod_lshift1_quick(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *m)
	{
	if (!XBN_lshift1(r, a)) return 0;
	if (XBN_cmp(r, m) >= 0)
		return XBN_sub(r, r, m);
	return 1;
	}


int XBN_mod_lshift(XBIGNUM *r, const XBIGNUM *a, int n, const XBIGNUM *m, XBN_CTX *ctx)
	{
	XBIGNUM *abs_m = NULL;
	int ret;

	if (!XBN_nnmod(r, a, m, ctx)) return 0;

	if (m->neg)
		{
		abs_m = XBN_dup(m);
		if (abs_m == NULL) return 0;
		abs_m->neg = 0;
		}
	
	ret = XBN_mod_lshift_quick(r, r, n, (abs_m ? abs_m : m));

	if (abs_m)
		XBN_free(abs_m);
	return ret;
	}


/* XBN_mod_lshift variant that may be used if  a  is non-negative
 * and less than  m */
int XBN_mod_lshift_quick(XBIGNUM *r, const XBIGNUM *a, int n, const XBIGNUM *m)
	{
	if (r != a)
		{
		if (XBN_copy(r, a) == NULL) return 0;
		}

	while (n > 0)
		{
		int max_shift;
		
		/* 0 < r < m */
		max_shift = XBN_num_bits(m) - XBN_num_bits(r);
		/* max_shift >= 0 */

		if (max_shift < 0)
			{
			//BNerr(XBN_F_XBN_MOD_LSHIFT_QUICK, XBN_R_INPUT_NOT_REDUCED);
			return 0;
			}

		if (max_shift > n)
			max_shift = n;

		if (max_shift)
			{
			if (!XBN_lshift(r, r, max_shift)) return 0;
			n -= max_shift;
			}
		else
			{
			if (!XBN_lshift1(r, r)) return 0;
			--n;
			}

		/* XBN_num_bits(r) <= XBN_num_bits(m) */

		if (XBN_cmp(r, m) >= 0) 
			{
			if (!XBN_sub(r, r, m)) return 0;
			}
		}
	
	return 1;
	}
