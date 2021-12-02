/* crypto/bn/xbn_recp.c */
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

#ifndef __KERNEL__

#ifdef _MTK_BUILD_
#include "libc/stdio.h"
#else
#include <stdio.h>
#endif
//#include "cryptlib.h"
#include "bn_lcl.h"

#ifdef _MTK_BUILD_
#include "libc/stdlib.h"
#else
#include <stdlib.h>
#endif

#else

#include "bn_lcl.h"

#endif

void XBN_RECP_CTX_init(XBN_RECP_CTX *recp)
	{
	XBN_init(&(recp->N));
	XBN_init(&(recp->Nr));
	recp->num_bits=0;
	recp->flags=0;
	}

XBN_RECP_CTX *XBN_RECP_CTX_new(void)
	{
	XBN_RECP_CTX *ret;

	//if ((ret=(XBN_RECP_CTX *)OPENSSL_malloc(sizeof(XBN_RECP_CTX))) == NULL)
	if ((ret=(XBN_RECP_CTX *)malloc(sizeof(XBN_RECP_CTX))) == NULL)
		return(NULL);

	XBN_RECP_CTX_init(ret);
	ret->flags=XBN_FLG_MALLOCED;
	return(ret);
	}

void XBN_RECP_CTX_free(XBN_RECP_CTX *recp)
	{
	if(recp == NULL)
	    return;

	XBN_free(&(recp->N));
	XBN_free(&(recp->Nr));
	if (recp->flags & XBN_FLG_MALLOCED)
	{
		//OPENSSL_free(recp);
		free(recp);
	}
		
	}

int XBN_RECP_CTX_set(XBN_RECP_CTX *recp, const XBIGNUM *d, XBN_CTX *ctx)
	{
	ctx = ctx; // to avoid warning ... it is not used...
	if (!XBN_copy(&(recp->N),d)) return 0;
	if (!XBN_zero(&(recp->Nr))) return 0;
	recp->num_bits=XBN_num_bits(d);
	recp->shift=0;
	return(1);
	}

int XBN_mod_mul_reciprocal(XBIGNUM *r, const XBIGNUM *x, const XBIGNUM *y,
	XBN_RECP_CTX *recp, XBN_CTX *ctx)
	{
	int ret=0;
	XBIGNUM *a;
	const XBIGNUM *ca;

	XBN_CTX_start(ctx);
	if ((a = XBN_CTX_get(ctx)) == NULL) goto err;
	if (y != NULL)
		{
		if (x == y)
			{ if (!XBN_sqr(a,x,ctx)) goto err; }
		else
			{ if (!XBN_mul(a,x,y,ctx)) goto err; }
		ca = a;
		}
	else
		ca=x; /* Just do the mod */

	ret = XBN_div_recp(NULL,r,ca,recp,ctx);
err:
	XBN_CTX_end(ctx);
	return(ret);
	}

int XBN_div_recp(XBIGNUM *dv, XBIGNUM *rem, const XBIGNUM *m,
	XBN_RECP_CTX *recp, XBN_CTX *ctx)
	{
	int i,j,ret=0;
	XBIGNUM *a,*b,*d,*r;

	XBN_CTX_start(ctx);
	a=XBN_CTX_get(ctx);
	b=XBN_CTX_get(ctx);
	if (dv != NULL)
		d=dv;
	else
		d=XBN_CTX_get(ctx);
	if (rem != NULL)
		r=rem;
	else
		r=XBN_CTX_get(ctx);
	if (a == NULL || b == NULL || d == NULL || r == NULL) goto err;

	if (XBN_ucmp(m,&(recp->N)) < 0)
		{
		if (!XBN_zero(d)) return 0;
		if (!XBN_copy(r,m)) return 0;
		XBN_CTX_end(ctx);
		return(1);
		}

	/* We want the remainder
	 * Given input of ABCDEF / ab
	 * we need multiply ABCDEF by 3 digests of the reciprocal of ab
	 *
	 */

	/* i := max(XBN_num_bits(m), 2*XBN_num_bits(N)) */
	i=XBN_num_bits(m);
	j=recp->num_bits<<1;
	if (j>i) i=j;

	/* Nr := round(2^i / N) */
	if (i != recp->shift)
		recp->shift=XBN_reciprocal(&(recp->Nr),&(recp->N),
			i,ctx); /* XBN_reciprocal returns i, or -1 for an error */
	if (recp->shift == -1) goto err;

	/* d := |round(round(m / 2^XBN_num_bits(N)) * recp->Nr / 2^(i - XBN_num_bits(N)))|
	 *    = |round(round(m / 2^XBN_num_bits(N)) * round(2^i / N) / 2^(i - XBN_num_bits(N)))|
	 *   <= |(m / 2^XBN_num_bits(N)) * (2^i / N) * (2^XBN_num_bits(N) / 2^i)|
	 *    = |m/N|
	 */
	if (!XBN_rshift(a,m,recp->num_bits)) goto err;
	if (!XBN_mul(b,a,&(recp->Nr),ctx)) goto err;
	if (!XBN_rshift(d,b,i-recp->num_bits)) goto err;
	d->neg=0;

	if (!XBN_mul(b,&(recp->N),d,ctx)) goto err;
	if (!XBN_usub(r,m,b)) goto err;
	r->neg=0;

#if 1
	j=0;
	while (XBN_ucmp(r,&(recp->N)) >= 0)
		{
		if (j++ > 2)
			{
			//BNerr(XBN_F_XBN_MOD_MUL_RECIPROCAL,XBN_R_BAD_RECIPROCAL);
			goto err;
			}
		if (!XBN_usub(r,r,&(recp->N))) goto err;
		if (!XBN_add_word(d,1)) goto err;
		}
#endif

	r->neg=XBN_is_zero(r)?0:m->neg;
	d->neg=m->neg^recp->N.neg;
	ret=1;
err:
	XBN_CTX_end(ctx);
	return(ret);
	} 

/* len is the expected size of the result
 * We actually calculate with an extra word of precision, so
 * we can do faster division if the remainder is not required.
 */
/* r := 2^len / m */
int XBN_reciprocal(XBIGNUM *r, const XBIGNUM *m, int len, XBN_CTX *ctx)
	{
	int ret= -1;
	XBIGNUM t;

	XBN_init(&t);

	if (!XBN_zero(&t)) goto err;
	if (!XBN_set_bit(&t,len)) goto err;

	if (!XBN_div(r,NULL,&t,m,ctx)) goto err;

	ret=len;
err:
	XBN_free(&t);
	return(ret);
	}
