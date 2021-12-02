/* crypto/bn/xbn_mont.c */
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

/*
 * Details about Montgomery multiplication algorithms can be found at
 * http://security.ece.orst.edu/publications.html, e.g.
 * http://security.ece.orst.edu/koc/papers/j37acmon.pdf and
 * sections 3.8 and 4.2 in http://security.ece.orst.edu/koc/papers/r01rsasw.pdf
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

#define MONT_WORD /* use the faster word-based algorithm */

int XBN_mod_mul_montgomery(XBIGNUM *r, const XBIGNUM *a, const XBIGNUM *b,
			  XBN_MONT_CTX *mont, XBN_CTX *ctx)
	{
	XBIGNUM *tmp;
	int ret=0;

	XBN_CTX_start(ctx);
	tmp = XBN_CTX_get(ctx);
	if (tmp == NULL) goto err;

	xbn_check_top(tmp);
	if (a == b)
		{
		if (!XBN_sqr(tmp,a,ctx)) goto err;
		}
	else
		{
		if (!XBN_mul(tmp,a,b,ctx)) goto err;
		}
	/* reduce from aRR to aR */
	if (!XBN_from_montgomery(r,tmp,mont,ctx)) goto err;
	ret=1;
err:
	XBN_CTX_end(ctx);
	return(ret);
	}

int XBN_from_montgomery(XBIGNUM *ret, const XBIGNUM *a, XBN_MONT_CTX *mont,
	     XBN_CTX *ctx)
	{
	int retn=0;

#ifdef MONT_WORD
	XBIGNUM *n,*r;
	XBN_ULONG *ap,*np,*rp,n0,v,*nrp;
	int al,nl,max,i,x,ri;

	XBN_CTX_start(ctx);
	if ((r = XBN_CTX_get(ctx)) == NULL) goto err;

	if (!XBN_copy(r,a)) goto err;
	n= &(mont->N);

	ap=a->d;
	/* mont->ri is the size of mont->N in bits (rounded up
	   to the word size) */
	al=ri=mont->ri/XBN_BITS2;
	
	nl=n->top;
	if ((al == 0) || (nl == 0)) { r->top=0; return(1); }

	max=(nl+al+1); /* allow for overflow (no?) XXX */
	if (xbn_wexpand(r,max) == NULL) goto err;
	if (xbn_wexpand(ret,max) == NULL) goto err;

	r->neg=a->neg^n->neg;
	np=n->d;
	rp=r->d;
	nrp= &(r->d[nl]);

	/* clear the top words of T */
#if 1
	for (i=r->top; i<max; i++) /* memset? XXX */
		r->d[i]=0;
#else
	memset(&(r->d[r->top]),0,(max-r->top)*sizeof(XBN_ULONG)); 
#endif

	r->top=max;
	n0=mont->n0;

#ifdef XBN_COUNT
	fprintf(stderr,"word XBN_from_montgomery %d * %d\n",nl,nl);
#endif
	for (i=0; i<nl; i++)
		{
#ifdef __TANDEM
                {
                   long long t1;
                   long long t2;
                   long long t3;
                   t1 = rp[0] * (n0 & 0177777);
                   t2 = 037777600000l;
                   t2 = n0 & t2;
                   t3 = rp[0] & 0177777;
                   t2 = (t3 * t2) & XBN_MASK2;
                   t1 = t1 + t2;
                   v=xbn_mul_add_words(rp,np,nl,(XBN_ULONG) t1);
                }
#else
		v=xbn_mul_add_words(rp,np,nl,(rp[0]*n0)&XBN_MASK2);
#endif
		nrp++;
		rp++;
		if (((nrp[-1]+=v)&XBN_MASK2) >= v)
			continue;
		else
			{
			if (((++nrp[0])&XBN_MASK2) != 0) continue;
			if (((++nrp[1])&XBN_MASK2) != 0) continue;
			for (x=2; (((++nrp[x])&XBN_MASK2) == 0); x++) ;
			}
		}
	xbn_fix_top(r);
	
	/* mont->ri will be a multiple of the word size */
#if 0
	XBN_rshift(ret,r,mont->ri);
#else
	ret->neg = r->neg;
	x=ri;
	rp=ret->d;
	ap= &(r->d[x]);
	if (r->top < x)
		al=0;
	else
		al=r->top-x;
	ret->top=al;
	al-=4;
	for (i=0; i<al; i+=4)
		{
		XBN_ULONG t1,t2,t3,t4;
		
		t1=ap[i+0];
		t2=ap[i+1];
		t3=ap[i+2];
		t4=ap[i+3];
		rp[i+0]=t1;
		rp[i+1]=t2;
		rp[i+2]=t3;
		rp[i+3]=t4;
		}
	al+=4;
	for (; i<al; i++)
		rp[i]=ap[i];
#endif
#else /* !MONT_WORD */ 
	XBIGNUM *t1,*t2;

	XBN_CTX_start(ctx);
	t1 = XBN_CTX_get(ctx);
	t2 = XBN_CTX_get(ctx);
	if (t1 == NULL || t2 == NULL) goto err;
	
	if (!XBN_copy(t1,a)) goto err;
	XBN_mask_bits(t1,mont->ri);

	if (!XBN_mul(t2,t1,&mont->Ni,ctx)) goto err;
	XBN_mask_bits(t2,mont->ri);

	if (!XBN_mul(t1,t2,&mont->N,ctx)) goto err;
	if (!XBN_add(t2,a,t1)) goto err;
	if (!XBN_rshift(ret,t2,mont->ri)) goto err;
#endif /* MONT_WORD */

	if (XBN_ucmp(ret, &(mont->N)) >= 0)
		{
		if (!XBN_usub(ret,ret,&(mont->N))) goto err;
		}
	retn=1;
 err:
	XBN_CTX_end(ctx);
	return(retn);
	}

XBN_MONT_CTX *XBN_MONT_CTX_new(void)
	{
	XBN_MONT_CTX *ret;

	//if ((ret=(XBN_MONT_CTX *)OPENSSL_malloc(sizeof(XBN_MONT_CTX))) == NULL)
	if ((ret=(XBN_MONT_CTX *)malloc(sizeof(XBN_MONT_CTX))) == NULL)
		return(NULL);

	XBN_MONT_CTX_init(ret);
	ret->flags=XBN_FLG_MALLOCED;
	return(ret);
	}

void XBN_MONT_CTX_init(XBN_MONT_CTX *ctx)
	{
	ctx->ri=0;
	XBN_init(&(ctx->RR));
	XBN_init(&(ctx->N));
	XBN_init(&(ctx->Ni));
	ctx->flags=0;
	}

void XBN_MONT_CTX_free(XBN_MONT_CTX *mont)
	{
	if(mont == NULL)
	    return;

	XBN_free(&(mont->RR));
	XBN_free(&(mont->N));
	XBN_free(&(mont->Ni));
	if (mont->flags & XBN_FLG_MALLOCED)
	{
		//OPENSSL_free(mont);
		free(mont);
	}
		
	}

int XBN_MONT_CTX_set(XBN_MONT_CTX *mont, const XBIGNUM *mod, XBN_CTX *ctx)
	{
	XBIGNUM Ri,*R;

	XBN_init(&Ri);
	R= &(mont->RR);					/* grab RR as a temp */
	XBN_copy(&(mont->N),mod);			/* Set N */
	mont->N.neg = 0;

#ifdef MONT_WORD
		{
		XBIGNUM tmod;
		XBN_ULONG buf[2];

		mont->ri=(XBN_num_bits(mod)+(XBN_BITS2-1))/XBN_BITS2*XBN_BITS2;
		if (!(XBN_zero(R))) goto err;
		if (!(XBN_set_bit(R,XBN_BITS2))) goto err;	/* R */

		buf[0]=mod->d[0]; /* tmod = N mod word size */
		buf[1]=0;
		tmod.d=buf;
		tmod.top=1;
		tmod.dmax=2;
		tmod.neg=0;
							/* Ri = R^-1 mod N*/
		if ((XBN_mod_inverse(&Ri,R,&tmod,ctx)) == NULL)
			goto err;
		if (!XBN_lshift(&Ri,&Ri,XBN_BITS2)) goto err; /* R*Ri */
		if (!XBN_is_zero(&Ri))
			{
			if (!XBN_sub_word(&Ri,1)) goto err;
			}
		else /* if N mod word size == 1 */
			{
			if (!XBN_set_word(&Ri,XBN_MASK2)) goto err;  /* Ri-- (mod word size) */
			}
		if (!XBN_div(&Ri,NULL,&Ri,&tmod,ctx)) goto err;
		/* Ni = (R*Ri-1)/N,
		 * keep only least significant word: */
		mont->n0 = (Ri.top > 0) ? Ri.d[0] : 0;
		XBN_free(&Ri);
		}
#else /* !MONT_WORD */
		{ /* bignum version */
		mont->ri=XBN_num_bits(&mont->N);
		if (!XBN_zero(R)) goto err;
		if (!XBN_set_bit(R,mont->ri)) goto err;  /* R = 2^ri */
		                                        /* Ri = R^-1 mod N*/
		if ((XBN_mod_inverse(&Ri,R,&mont->N,ctx)) == NULL)
			goto err;
		if (!XBN_lshift(&Ri,&Ri,mont->ri)) goto err; /* R*Ri */
		if (!XBN_sub_word(&Ri,1)) goto err;
							/* Ni = (R*Ri-1) / N */
		if (!XBN_div(&(mont->Ni),NULL,&Ri,&mont->N,ctx)) goto err;
		XBN_free(&Ri);
		}
#endif

	/* setup RR for conversions */
	if (!XBN_zero(&(mont->RR))) goto err;
	if (!XBN_set_bit(&(mont->RR),mont->ri*2)) goto err;
	if (!XBN_mod(&(mont->RR),&(mont->RR),&(mont->N),ctx)) goto err;

	return(1);
err:
	return(0);
	}

XBN_MONT_CTX *XBN_MONT_CTX_copy(XBN_MONT_CTX *to, XBN_MONT_CTX *from)
	{
	if (to == from) return(to);

	if (!XBN_copy(&(to->RR),&(from->RR))) return NULL;
	if (!XBN_copy(&(to->N),&(from->N))) return NULL;
	if (!XBN_copy(&(to->Ni),&(from->Ni))) return NULL;
	to->ri=from->ri;
	to->n0=from->n0;
	return(to);
	}

