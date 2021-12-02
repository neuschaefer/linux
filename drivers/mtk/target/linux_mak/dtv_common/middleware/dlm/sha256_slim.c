/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * $RCSfile:  $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *---------------------------------------------------------------------------*/
/* crypto/sha/sha1.c */
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

#include "sha256_slim.h"

int SHA256_Init (SHA256_CTX *c)
{
    c->h[0]=0x6a09e667UL; c->h[1]=0xbb67ae85UL;
    c->h[2]=0x3c6ef372UL; c->h[3]=0xa54ff53aUL;
    c->h[4]=0x510e527fUL; c->h[5]=0x9b05688cUL;
    c->h[6]=0x1f83d9abUL; c->h[7]=0x5be0cd19UL;
    c->Nl=0; c->Nh=0;
    c->num=0; c->md_len=32;
    return 1;
}

unsigned char *SHA256(const unsigned char *d, int n, unsigned char *md)
{
    SHA256_CTX c;
    static unsigned char m[32];

    if (md == ((void *)0)) md=m;
    SHA256_Init(&c);
    SHA256_Update(&c,d,n);
    SHA256_Final(md,&c);
    return(md);
}

void sha256_block_host_order (SHA256_CTX *ctx, const void *in, int num);
void sha256_block_data_order (SHA256_CTX *ctx, const void *in, int num);

int SHA256_Update (SHA256_CTX *c, const void *data_, int len)
{
    const unsigned char *data=data_;
    register unsigned int * p;
    register unsigned int l;
    int sw,sc,ew,ec;

    if (len==0) return 1;

    l=(c->Nl+(((unsigned int)len)<<3))&0xffffffffUL;


    if (l < c->Nl)
        c->Nh++;
    c->Nh+=(len>>29);
    c->Nl=l;

    if (c->num != 0)
    {
        p=c->data;
        sw=c->num>>2;
        sc=c->num&0x03;

        if ((c->num+len) >= (16*4))
        {
            l=p[sw]; { switch (sc)
                {
                case 0: l =((unsigned long)(*((data)++)))<<24;
                case 1: l|=((unsigned long)(*((data)++)))<<16;
                case 2: l|=((unsigned long)(*((data)++)))<< 8;
                case 3: l|=((unsigned long)(*((data)++)));
                }}; p[sw++]=l;
            for (; sw<16; sw++)
            {
                (l =(((unsigned long)(*((data)++)))<<24), 
                 l|=(((unsigned long)(*((data)++)))<<16), 
                 l|=(((unsigned long)(*((data)++)))<< 8), 
                 l|=(((unsigned long)(*((data)++))) ), l); p[sw]=l;
            }
            sha256_block_host_order (c,p,1);
            len-=((16*4)-c->num);
            c->num=0;

        }
        else
        {
            c->num+=(unsigned int)len;
            if ((sc+len) < 4)
            {
                l=p[sw]; { switch (sc)
                    {
                    case 0: l =((unsigned long)(*((data)++)))<<24; if (--len == 0) break;
                    case 1: l|=((unsigned long)(*((data)++)))<<16; if (--len == 0) break;
                    case 2: l|=((unsigned long)(*((data)++)))<< 8;
                    }}; p[sw]=l;
            }
            else
            {
                ew=(c->num>>2);
                ec=(c->num&0x03);
                if (sc)
                    l=p[sw];
                { switch (sc)
                    {
                    case 0: l =((unsigned long)(*((data)++)))<<24;
                    case 1: l|=((unsigned long)(*((data)++)))<<16;
                    case 2: l|=((unsigned long)(*((data)++)))<< 8;
                    case 3: l|=((unsigned long)(*((data)++)));
                    }};
                p[sw++]=l;
                for (; sw < ew; sw++)
                {
                    (l =(((unsigned long)(*((data)++)))<<24), 
                     l|=(((unsigned long)(*((data)++)))<<16), 
                     l|=(((unsigned long)(*((data)++)))<< 8), 
                     l|=(((unsigned long)(*((data)++))) ), l); p[sw]=l;
                }
                if (ec)
                {
                    { l=0; (data)+=ec; switch (ec)
                        {
                        case 3: l =((unsigned long)(*(--(data))))<< 8;
                        case 2: l|=((unsigned long)(*(--(data))))<<16;
                        case 1: l|=((unsigned long)(*(--(data))))<<24;
                        }}; p[sw]=l;
                }
            }
            return 1;
        }
    }

    sw=len/(16*4);
    if (sw > 0)
    {
        {
            sha256_block_data_order(c,data,sw);
            sw*=(16*4);
            data+=sw;
            len-=sw;
        }

    }

    if (len!=0)
    {
        p = c->data;
        c->num = len;
        ew=len>>2;
        ec=len&0x03;
        for (; ew; ew--,p++)
        {
            (l =(((unsigned long)(*((data)++)))<<24), 
             l|=(((unsigned long)(*((data)++)))<<16), 
             l|=(((unsigned long)(*((data)++)))<< 8), 
             l|=(((unsigned long)(*((data)++))) ), l); *p=l;
        }
        { l=0; (data)+=ec; switch (ec)
            {
            case 3: l =((unsigned long)(*(--(data))))<< 8;
            case 2: l|=((unsigned long)(*(--(data))))<<16;
            case 1: l|=((unsigned long)(*(--(data))))<<24;
            }};
        *p=l;
    }
    return 1;
}


void SHA256_Transform (SHA256_CTX *c, const unsigned char *data)
{
    sha256_block_data_order (c,data,1);

}


int SHA256_Final (unsigned char *md, SHA256_CTX *c)
{
    register unsigned int *p;
    register unsigned long l;
    register int i,j;
    static const unsigned char end[4]={0x80,0x00,0x00,0x00};
    const unsigned char *cp=end;


    p=c->data;
    i=c->num>>2;
    j=c->num&0x03;
    l = (j==0) ? 0 : p[i];

    { switch (j)
        {
        case 0: l =((unsigned long)(*((cp)++)))<<24;
        case 1: l|=((unsigned long)(*((cp)++)))<<16;
        case 2: l|=((unsigned long)(*((cp)++)))<< 8;
        case 3: l|=((unsigned long)(*((cp)++)));
        }}; p[i++]=l;

    if (i>(16 -2))
    {
        if (i<16) p[i]=0;
        sha256_block_host_order (c,p,1);
        i=0;
    }
    for (; i<(16 -2); i++)
        p[i]=0;


    p[16 -2]=c->Nh;
    p[16 -1]=c->Nl;

    sha256_block_host_order (c,p,1);

    do
    {
        unsigned long ll; unsigned int n; switch ((c)->md_len)
        {
        case 28: for (n=0;n<28/4;n++)
            {
                ll=(c)->h[n]; (*(((md))++)=(unsigned char)(((ll)>>24)&0xff), 
                               *(((md))++)=(unsigned char)(((ll)>>16)&0xff), 
                               *(((md))++)=(unsigned char)(((ll)>> 8)&0xff), 
                               *(((md))++)=(unsigned char)(((ll) )&0xff), ll);
            } break;
        case 32: for (n=0;n<32/4;n++)
            {
                ll=(c)->h[n]; (*(((md))++)=(unsigned char)(((ll)>>24)&0xff), 
                               *(((md))++)=(unsigned char)(((ll)>>16)&0xff), 
                               *(((md))++)=(unsigned char)(((ll)>> 8)&0xff), 
                               *(((md))++)=(unsigned char)(((ll) )&0xff), ll);
            } break;
        default: if ((c)->md_len > 32) return 0;for (n=0;n<(c)->md_len/4;n++)
            {
                ll=(c)->h[n]; (*(((md))++)=(unsigned char)(((ll)>>24)&0xff), 
                               *(((md))++)=(unsigned char)(((ll)>>16)&0xff), 
                               *(((md))++)=(unsigned char)(((ll)>> 8)&0xff), 
                               *(((md))++)=(unsigned char)(((ll) )&0xff), ll);
            } break;
        }
    } while (0);
    c->num=0;
    return 1;
}



static const unsigned int K256[64] = {
    0x428a2f98UL,0x71374491UL,0xb5c0fbcfUL,0xe9b5dba5UL,
    0x3956c25bUL,0x59f111f1UL,0x923f82a4UL,0xab1c5ed5UL,
    0xd807aa98UL,0x12835b01UL,0x243185beUL,0x550c7dc3UL,
    0x72be5d74UL,0x80deb1feUL,0x9bdc06a7UL,0xc19bf174UL,
    0xe49b69c1UL,0xefbe4786UL,0x0fc19dc6UL,0x240ca1ccUL,
    0x2de92c6fUL,0x4a7484aaUL,0x5cb0a9dcUL,0x76f988daUL,
    0x983e5152UL,0xa831c66dUL,0xb00327c8UL,0xbf597fc7UL,
    0xc6e00bf3UL,0xd5a79147UL,0x06ca6351UL,0x14292967UL,
    0x27b70a85UL,0x2e1b2138UL,0x4d2c6dfcUL,0x53380d13UL,
    0x650a7354UL,0x766a0abbUL,0x81c2c92eUL,0x92722c85UL,
    0xa2bfe8a1UL,0xa81a664bUL,0xc24b8b70UL,0xc76c51a3UL,
    0xd192e819UL,0xd6990624UL,0xf40e3585UL,0x106aa070UL,
    0x19a4c116UL,0x1e376c08UL,0x2748774cUL,0x34b0bcb5UL,
    0x391c0cb3UL,0x4ed8aa4aUL,0x5b9cca4fUL,0x682e6ff3UL,
    0x748f82eeUL,0x78a5636fUL,0x84c87814UL,0x8cc70208UL,
    0x90befffaUL,0xa4506cebUL,0xbef9a3f7UL,0xc67178f2UL};

static void sha256_block (SHA256_CTX *ctx, const void *in, int num, int host)
{
    unsigned long a,b,c,d,e,f,g,h,s0,s1,T1;
    unsigned int X[16];
    int i;
    const unsigned char *data=in;

    while (num--)
    {

        a = ctx->h[0]; b = ctx->h[1]; c = ctx->h[2]; d = ctx->h[3];
        e = ctx->h[4]; f = ctx->h[5]; g = ctx->h[6]; h = ctx->h[7];

        if (host)
        {
            const unsigned int *W=(const unsigned int *)data;

            T1 = X[0] = W[0]; do { T1 += h + (((((e))<<(26))|((((e))&0xffffffff)>>(32-(26)))) ^ ((((e))<<(21))|((((e))&0xffffffff)>>(32-(21)))) ^ ((((e))<<(7))|((((e))&0xffffffff)>>(32-(7))))) + (((e) & (f)) ^ ((~(e)) & (g))) + K256[0]; h = (((((a))<<(30))|((((a))&0xffffffff)>>(32-(30)))) ^ ((((a))<<(19))|((((a))&0xffffffff)>>(32-(19)))) ^ ((((a))<<(10))|((((a))&0xffffffff)>>(32-(10))))) + (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c))); d += T1; h += T1; } while (0);
            T1 = X[1] = W[1]; do { T1 += g + (((((d))<<(26))|((((d))&0xffffffff)>>(32-(26)))) ^ ((((d))<<(21))|((((d))&0xffffffff)>>(32-(21)))) ^ ((((d))<<(7))|((((d))&0xffffffff)>>(32-(7))))) + (((d) & (e)) ^ ((~(d)) & (f))) + K256[1]; g = (((((h))<<(30))|((((h))&0xffffffff)>>(32-(30)))) ^ ((((h))<<(19))|((((h))&0xffffffff)>>(32-(19)))) ^ ((((h))<<(10))|((((h))&0xffffffff)>>(32-(10))))) + (((h) & (a)) ^ ((h) & (b)) ^ ((a) & (b))); c += T1; g += T1; } while (0);
            T1 = X[2] = W[2]; do { T1 += f + (((((c))<<(26))|((((c))&0xffffffff)>>(32-(26)))) ^ ((((c))<<(21))|((((c))&0xffffffff)>>(32-(21)))) ^ ((((c))<<(7))|((((c))&0xffffffff)>>(32-(7))))) + (((c) & (d)) ^ ((~(c)) & (e))) + K256[2]; f = (((((g))<<(30))|((((g))&0xffffffff)>>(32-(30)))) ^ ((((g))<<(19))|((((g))&0xffffffff)>>(32-(19)))) ^ ((((g))<<(10))|((((g))&0xffffffff)>>(32-(10))))) + (((g) & (h)) ^ ((g) & (a)) ^ ((h) & (a))); b += T1; f += T1; } while (0);
            T1 = X[3] = W[3]; do { T1 += e + (((((b))<<(26))|((((b))&0xffffffff)>>(32-(26)))) ^ ((((b))<<(21))|((((b))&0xffffffff)>>(32-(21)))) ^ ((((b))<<(7))|((((b))&0xffffffff)>>(32-(7))))) + (((b) & (c)) ^ ((~(b)) & (d))) + K256[3]; e = (((((f))<<(30))|((((f))&0xffffffff)>>(32-(30)))) ^ ((((f))<<(19))|((((f))&0xffffffff)>>(32-(19)))) ^ ((((f))<<(10))|((((f))&0xffffffff)>>(32-(10))))) + (((f) & (g)) ^ ((f) & (h)) ^ ((g) & (h))); a += T1; e += T1; } while (0);
            T1 = X[4] = W[4]; do { T1 += d + (((((a))<<(26))|((((a))&0xffffffff)>>(32-(26)))) ^ ((((a))<<(21))|((((a))&0xffffffff)>>(32-(21)))) ^ ((((a))<<(7))|((((a))&0xffffffff)>>(32-(7))))) + (((a) & (b)) ^ ((~(a)) & (c))) + K256[4]; d = (((((e))<<(30))|((((e))&0xffffffff)>>(32-(30)))) ^ ((((e))<<(19))|((((e))&0xffffffff)>>(32-(19)))) ^ ((((e))<<(10))|((((e))&0xffffffff)>>(32-(10))))) + (((e) & (f)) ^ ((e) & (g)) ^ ((f) & (g))); h += T1; d += T1; } while (0);
            T1 = X[5] = W[5]; do { T1 += c + (((((h))<<(26))|((((h))&0xffffffff)>>(32-(26)))) ^ ((((h))<<(21))|((((h))&0xffffffff)>>(32-(21)))) ^ ((((h))<<(7))|((((h))&0xffffffff)>>(32-(7))))) + (((h) & (a)) ^ ((~(h)) & (b))) + K256[5]; c = (((((d))<<(30))|((((d))&0xffffffff)>>(32-(30)))) ^ ((((d))<<(19))|((((d))&0xffffffff)>>(32-(19)))) ^ ((((d))<<(10))|((((d))&0xffffffff)>>(32-(10))))) + (((d) & (e)) ^ ((d) & (f)) ^ ((e) & (f))); g += T1; c += T1; } while (0);
            T1 = X[6] = W[6]; do { T1 += b + (((((g))<<(26))|((((g))&0xffffffff)>>(32-(26)))) ^ ((((g))<<(21))|((((g))&0xffffffff)>>(32-(21)))) ^ ((((g))<<(7))|((((g))&0xffffffff)>>(32-(7))))) + (((g) & (h)) ^ ((~(g)) & (a))) + K256[6]; b = (((((c))<<(30))|((((c))&0xffffffff)>>(32-(30)))) ^ ((((c))<<(19))|((((c))&0xffffffff)>>(32-(19)))) ^ ((((c))<<(10))|((((c))&0xffffffff)>>(32-(10))))) + (((c) & (d)) ^ ((c) & (e)) ^ ((d) & (e))); f += T1; b += T1; } while (0);
            T1 = X[7] = W[7]; do { T1 += a + (((((f))<<(26))|((((f))&0xffffffff)>>(32-(26)))) ^ ((((f))<<(21))|((((f))&0xffffffff)>>(32-(21)))) ^ ((((f))<<(7))|((((f))&0xffffffff)>>(32-(7))))) + (((f) & (g)) ^ ((~(f)) & (h))) + K256[7]; a = (((((b))<<(30))|((((b))&0xffffffff)>>(32-(30)))) ^ ((((b))<<(19))|((((b))&0xffffffff)>>(32-(19)))) ^ ((((b))<<(10))|((((b))&0xffffffff)>>(32-(10))))) + (((b) & (c)) ^ ((b) & (d)) ^ ((c) & (d))); e += T1; a += T1; } while (0);
            T1 = X[8] = W[8]; do { T1 += h + (((((e))<<(26))|((((e))&0xffffffff)>>(32-(26)))) ^ ((((e))<<(21))|((((e))&0xffffffff)>>(32-(21)))) ^ ((((e))<<(7))|((((e))&0xffffffff)>>(32-(7))))) + (((e) & (f)) ^ ((~(e)) & (g))) + K256[8]; h = (((((a))<<(30))|((((a))&0xffffffff)>>(32-(30)))) ^ ((((a))<<(19))|((((a))&0xffffffff)>>(32-(19)))) ^ ((((a))<<(10))|((((a))&0xffffffff)>>(32-(10))))) + (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c))); d += T1; h += T1; } while (0);
            T1 = X[9] = W[9]; do { T1 += g + (((((d))<<(26))|((((d))&0xffffffff)>>(32-(26)))) ^ ((((d))<<(21))|((((d))&0xffffffff)>>(32-(21)))) ^ ((((d))<<(7))|((((d))&0xffffffff)>>(32-(7))))) + (((d) & (e)) ^ ((~(d)) & (f))) + K256[9]; g = (((((h))<<(30))|((((h))&0xffffffff)>>(32-(30)))) ^ ((((h))<<(19))|((((h))&0xffffffff)>>(32-(19)))) ^ ((((h))<<(10))|((((h))&0xffffffff)>>(32-(10))))) + (((h) & (a)) ^ ((h) & (b)) ^ ((a) & (b))); c += T1; g += T1; } while (0);
            T1 = X[10] = W[10]; do { T1 += f + (((((c))<<(26))|((((c))&0xffffffff)>>(32-(26)))) ^ ((((c))<<(21))|((((c))&0xffffffff)>>(32-(21)))) ^ ((((c))<<(7))|((((c))&0xffffffff)>>(32-(7))))) + (((c) & (d)) ^ ((~(c)) & (e))) + K256[10]; f = (((((g))<<(30))|((((g))&0xffffffff)>>(32-(30)))) ^ ((((g))<<(19))|((((g))&0xffffffff)>>(32-(19)))) ^ ((((g))<<(10))|((((g))&0xffffffff)>>(32-(10))))) + (((g) & (h)) ^ ((g) & (a)) ^ ((h) & (a))); b += T1; f += T1; } while (0);
            T1 = X[11] = W[11]; do { T1 += e + (((((b))<<(26))|((((b))&0xffffffff)>>(32-(26)))) ^ ((((b))<<(21))|((((b))&0xffffffff)>>(32-(21)))) ^ ((((b))<<(7))|((((b))&0xffffffff)>>(32-(7))))) + (((b) & (c)) ^ ((~(b)) & (d))) + K256[11]; e = (((((f))<<(30))|((((f))&0xffffffff)>>(32-(30)))) ^ ((((f))<<(19))|((((f))&0xffffffff)>>(32-(19)))) ^ ((((f))<<(10))|((((f))&0xffffffff)>>(32-(10))))) + (((f) & (g)) ^ ((f) & (h)) ^ ((g) & (h))); a += T1; e += T1; } while (0);
            T1 = X[12] = W[12]; do { T1 += d + (((((a))<<(26))|((((a))&0xffffffff)>>(32-(26)))) ^ ((((a))<<(21))|((((a))&0xffffffff)>>(32-(21)))) ^ ((((a))<<(7))|((((a))&0xffffffff)>>(32-(7))))) + (((a) & (b)) ^ ((~(a)) & (c))) + K256[12]; d = (((((e))<<(30))|((((e))&0xffffffff)>>(32-(30)))) ^ ((((e))<<(19))|((((e))&0xffffffff)>>(32-(19)))) ^ ((((e))<<(10))|((((e))&0xffffffff)>>(32-(10))))) + (((e) & (f)) ^ ((e) & (g)) ^ ((f) & (g))); h += T1; d += T1; } while (0);
            T1 = X[13] = W[13]; do { T1 += c + (((((h))<<(26))|((((h))&0xffffffff)>>(32-(26)))) ^ ((((h))<<(21))|((((h))&0xffffffff)>>(32-(21)))) ^ ((((h))<<(7))|((((h))&0xffffffff)>>(32-(7))))) + (((h) & (a)) ^ ((~(h)) & (b))) + K256[13]; c = (((((d))<<(30))|((((d))&0xffffffff)>>(32-(30)))) ^ ((((d))<<(19))|((((d))&0xffffffff)>>(32-(19)))) ^ ((((d))<<(10))|((((d))&0xffffffff)>>(32-(10))))) + (((d) & (e)) ^ ((d) & (f)) ^ ((e) & (f))); g += T1; c += T1; } while (0);
            T1 = X[14] = W[14]; do { T1 += b + (((((g))<<(26))|((((g))&0xffffffff)>>(32-(26)))) ^ ((((g))<<(21))|((((g))&0xffffffff)>>(32-(21)))) ^ ((((g))<<(7))|((((g))&0xffffffff)>>(32-(7))))) + (((g) & (h)) ^ ((~(g)) & (a))) + K256[14]; b = (((((c))<<(30))|((((c))&0xffffffff)>>(32-(30)))) ^ ((((c))<<(19))|((((c))&0xffffffff)>>(32-(19)))) ^ ((((c))<<(10))|((((c))&0xffffffff)>>(32-(10))))) + (((c) & (d)) ^ ((c) & (e)) ^ ((d) & (e))); f += T1; b += T1; } while (0);
            T1 = X[15] = W[15]; do { T1 += a + (((((f))<<(26))|((((f))&0xffffffff)>>(32-(26)))) ^ ((((f))<<(21))|((((f))&0xffffffff)>>(32-(21)))) ^ ((((f))<<(7))|((((f))&0xffffffff)>>(32-(7))))) + (((f) & (g)) ^ ((~(f)) & (h))) + K256[15]; a = (((((b))<<(30))|((((b))&0xffffffff)>>(32-(30)))) ^ ((((b))<<(19))|((((b))&0xffffffff)>>(32-(19)))) ^ ((((b))<<(10))|((((b))&0xffffffff)>>(32-(10))))) + (((b) & (c)) ^ ((b) & (d)) ^ ((c) & (d))); e += T1; a += T1; } while (0);

            data += (16*4);
         }
        else
        {
            unsigned int l;

            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[0] = l; do { T1 += h + (((((e))<<(26))|((((e))&0xffffffff)>>(32-(26)))) ^ ((((e))<<(21))|((((e))&0xffffffff)>>(32-(21)))) ^ ((((e))<<(7))|((((e))&0xffffffff)>>(32-(7))))) + (((e) & (f)) ^ ((~(e)) & (g))) + K256[0]; h = (((((a))<<(30))|((((a))&0xffffffff)>>(32-(30)))) ^ ((((a))<<(19))|((((a))&0xffffffff)>>(32-(19)))) ^ ((((a))<<(10))|((((a))&0xffffffff)>>(32-(10))))) + (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c))); d += T1; h += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[1] = l; do { T1 += g + (((((d))<<(26))|((((d))&0xffffffff)>>(32-(26)))) ^ ((((d))<<(21))|((((d))&0xffffffff)>>(32-(21)))) ^ ((((d))<<(7))|((((d))&0xffffffff)>>(32-(7))))) + (((d) & (e)) ^ ((~(d)) & (f))) + K256[1]; g = (((((h))<<(30))|((((h))&0xffffffff)>>(32-(30)))) ^ ((((h))<<(19))|((((h))&0xffffffff)>>(32-(19)))) ^ ((((h))<<(10))|((((h))&0xffffffff)>>(32-(10))))) + (((h) & (a)) ^ ((h) & (b)) ^ ((a) & (b))); c += T1; g += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[2] = l; do { T1 += f + (((((c))<<(26))|((((c))&0xffffffff)>>(32-(26)))) ^ ((((c))<<(21))|((((c))&0xffffffff)>>(32-(21)))) ^ ((((c))<<(7))|((((c))&0xffffffff)>>(32-(7))))) + (((c) & (d)) ^ ((~(c)) & (e))) + K256[2]; f = (((((g))<<(30))|((((g))&0xffffffff)>>(32-(30)))) ^ ((((g))<<(19))|((((g))&0xffffffff)>>(32-(19)))) ^ ((((g))<<(10))|((((g))&0xffffffff)>>(32-(10))))) + (((g) & (h)) ^ ((g) & (a)) ^ ((h) & (a))); b += T1; f += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[3] = l; do { T1 += e + (((((b))<<(26))|((((b))&0xffffffff)>>(32-(26)))) ^ ((((b))<<(21))|((((b))&0xffffffff)>>(32-(21)))) ^ ((((b))<<(7))|((((b))&0xffffffff)>>(32-(7))))) + (((b) & (c)) ^ ((~(b)) & (d))) + K256[3]; e = (((((f))<<(30))|((((f))&0xffffffff)>>(32-(30)))) ^ ((((f))<<(19))|((((f))&0xffffffff)>>(32-(19)))) ^ ((((f))<<(10))|((((f))&0xffffffff)>>(32-(10))))) + (((f) & (g)) ^ ((f) & (h)) ^ ((g) & (h))); a += T1; e += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[4] = l; do { T1 += d + (((((a))<<(26))|((((a))&0xffffffff)>>(32-(26)))) ^ ((((a))<<(21))|((((a))&0xffffffff)>>(32-(21)))) ^ ((((a))<<(7))|((((a))&0xffffffff)>>(32-(7))))) + (((a) & (b)) ^ ((~(a)) & (c))) + K256[4]; d = (((((e))<<(30))|((((e))&0xffffffff)>>(32-(30)))) ^ ((((e))<<(19))|((((e))&0xffffffff)>>(32-(19)))) ^ ((((e))<<(10))|((((e))&0xffffffff)>>(32-(10))))) + (((e) & (f)) ^ ((e) & (g)) ^ ((f) & (g))); h += T1; d += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[5] = l; do { T1 += c + (((((h))<<(26))|((((h))&0xffffffff)>>(32-(26)))) ^ ((((h))<<(21))|((((h))&0xffffffff)>>(32-(21)))) ^ ((((h))<<(7))|((((h))&0xffffffff)>>(32-(7))))) + (((h) & (a)) ^ ((~(h)) & (b))) + K256[5]; c = (((((d))<<(30))|((((d))&0xffffffff)>>(32-(30)))) ^ ((((d))<<(19))|((((d))&0xffffffff)>>(32-(19)))) ^ ((((d))<<(10))|((((d))&0xffffffff)>>(32-(10))))) + (((d) & (e)) ^ ((d) & (f)) ^ ((e) & (f))); g += T1; c += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[6] = l; do { T1 += b + (((((g))<<(26))|((((g))&0xffffffff)>>(32-(26)))) ^ ((((g))<<(21))|((((g))&0xffffffff)>>(32-(21)))) ^ ((((g))<<(7))|((((g))&0xffffffff)>>(32-(7))))) + (((g) & (h)) ^ ((~(g)) & (a))) + K256[6]; b = (((((c))<<(30))|((((c))&0xffffffff)>>(32-(30)))) ^ ((((c))<<(19))|((((c))&0xffffffff)>>(32-(19)))) ^ ((((c))<<(10))|((((c))&0xffffffff)>>(32-(10))))) + (((c) & (d)) ^ ((c) & (e)) ^ ((d) & (e))); f += T1; b += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[7] = l; do { T1 += a + (((((f))<<(26))|((((f))&0xffffffff)>>(32-(26)))) ^ ((((f))<<(21))|((((f))&0xffffffff)>>(32-(21)))) ^ ((((f))<<(7))|((((f))&0xffffffff)>>(32-(7))))) + (((f) & (g)) ^ ((~(f)) & (h))) + K256[7]; a = (((((b))<<(30))|((((b))&0xffffffff)>>(32-(30)))) ^ ((((b))<<(19))|((((b))&0xffffffff)>>(32-(19)))) ^ ((((b))<<(10))|((((b))&0xffffffff)>>(32-(10))))) + (((b) & (c)) ^ ((b) & (d)) ^ ((c) & (d))); e += T1; a += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[8] = l; do { T1 += h + (((((e))<<(26))|((((e))&0xffffffff)>>(32-(26)))) ^ ((((e))<<(21))|((((e))&0xffffffff)>>(32-(21)))) ^ ((((e))<<(7))|((((e))&0xffffffff)>>(32-(7))))) + (((e) & (f)) ^ ((~(e)) & (g))) + K256[8]; h = (((((a))<<(30))|((((a))&0xffffffff)>>(32-(30)))) ^ ((((a))<<(19))|((((a))&0xffffffff)>>(32-(19)))) ^ ((((a))<<(10))|((((a))&0xffffffff)>>(32-(10))))) + (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c))); d += T1; h += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[9] = l; do { T1 += g + (((((d))<<(26))|((((d))&0xffffffff)>>(32-(26)))) ^ ((((d))<<(21))|((((d))&0xffffffff)>>(32-(21)))) ^ ((((d))<<(7))|((((d))&0xffffffff)>>(32-(7))))) + (((d) & (e)) ^ ((~(d)) & (f))) + K256[9]; g = (((((h))<<(30))|((((h))&0xffffffff)>>(32-(30)))) ^ ((((h))<<(19))|((((h))&0xffffffff)>>(32-(19)))) ^ ((((h))<<(10))|((((h))&0xffffffff)>>(32-(10))))) + (((h) & (a)) ^ ((h) & (b)) ^ ((a) & (b))); c += T1; g += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[10] = l; do { T1 += f + (((((c))<<(26))|((((c))&0xffffffff)>>(32-(26)))) ^ ((((c))<<(21))|((((c))&0xffffffff)>>(32-(21)))) ^ ((((c))<<(7))|((((c))&0xffffffff)>>(32-(7))))) + (((c) & (d)) ^ ((~(c)) & (e))) + K256[10]; f = (((((g))<<(30))|((((g))&0xffffffff)>>(32-(30)))) ^ ((((g))<<(19))|((((g))&0xffffffff)>>(32-(19)))) ^ ((((g))<<(10))|((((g))&0xffffffff)>>(32-(10))))) + (((g) & (h)) ^ ((g) & (a)) ^ ((h) & (a))); b += T1; f += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[11] = l; do { T1 += e + (((((b))<<(26))|((((b))&0xffffffff)>>(32-(26)))) ^ ((((b))<<(21))|((((b))&0xffffffff)>>(32-(21)))) ^ ((((b))<<(7))|((((b))&0xffffffff)>>(32-(7))))) + (((b) & (c)) ^ ((~(b)) & (d))) + K256[11]; e = (((((f))<<(30))|((((f))&0xffffffff)>>(32-(30)))) ^ ((((f))<<(19))|((((f))&0xffffffff)>>(32-(19)))) ^ ((((f))<<(10))|((((f))&0xffffffff)>>(32-(10))))) + (((f) & (g)) ^ ((f) & (h)) ^ ((g) & (h))); a += T1; e += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[12] = l; do { T1 += d + (((((a))<<(26))|((((a))&0xffffffff)>>(32-(26)))) ^ ((((a))<<(21))|((((a))&0xffffffff)>>(32-(21)))) ^ ((((a))<<(7))|((((a))&0xffffffff)>>(32-(7))))) + (((a) & (b)) ^ ((~(a)) & (c))) + K256[12]; d = (((((e))<<(30))|((((e))&0xffffffff)>>(32-(30)))) ^ ((((e))<<(19))|((((e))&0xffffffff)>>(32-(19)))) ^ ((((e))<<(10))|((((e))&0xffffffff)>>(32-(10))))) + (((e) & (f)) ^ ((e) & (g)) ^ ((f) & (g))); h += T1; d += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[13] = l; do { T1 += c + (((((h))<<(26))|((((h))&0xffffffff)>>(32-(26)))) ^ ((((h))<<(21))|((((h))&0xffffffff)>>(32-(21)))) ^ ((((h))<<(7))|((((h))&0xffffffff)>>(32-(7))))) + (((h) & (a)) ^ ((~(h)) & (b))) + K256[13]; c = (((((d))<<(30))|((((d))&0xffffffff)>>(32-(30)))) ^ ((((d))<<(19))|((((d))&0xffffffff)>>(32-(19)))) ^ ((((d))<<(10))|((((d))&0xffffffff)>>(32-(10))))) + (((d) & (e)) ^ ((d) & (f)) ^ ((e) & (f))); g += T1; c += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[14] = l; do { T1 += b + (((((g))<<(26))|((((g))&0xffffffff)>>(32-(26)))) ^ ((((g))<<(21))|((((g))&0xffffffff)>>(32-(21)))) ^ ((((g))<<(7))|((((g))&0xffffffff)>>(32-(7))))) + (((g) & (h)) ^ ((~(g)) & (a))) + K256[14]; b = (((((c))<<(30))|((((c))&0xffffffff)>>(32-(30)))) ^ ((((c))<<(19))|((((c))&0xffffffff)>>(32-(19)))) ^ ((((c))<<(10))|((((c))&0xffffffff)>>(32-(10))))) + (((c) & (d)) ^ ((c) & (e)) ^ ((d) & (e))); f += T1; b += T1; } while (0);
            (l =(((unsigned long)(*((data)++)))<<24), l|=(((unsigned long)(*((data)++)))<<16), l|=(((unsigned long)(*((data)++)))<< 8), l|=(((unsigned long)(*((data)++))) ), l); T1 = X[15] = l; do { T1 += a + (((((f))<<(26))|((((f))&0xffffffff)>>(32-(26)))) ^ ((((f))<<(21))|((((f))&0xffffffff)>>(32-(21)))) ^ ((((f))<<(7))|((((f))&0xffffffff)>>(32-(7))))) + (((f) & (g)) ^ ((~(f)) & (h))) + K256[15]; a = (((((b))<<(30))|((((b))&0xffffffff)>>(32-(30)))) ^ ((((b))<<(19))|((((b))&0xffffffff)>>(32-(19)))) ^ ((((b))<<(10))|((((b))&0xffffffff)>>(32-(10))))) + (((b) & (c)) ^ ((b) & (d)) ^ ((c) & (d))); e += T1; a += T1; } while (0);
        }

        for (i=16;i<64;i+=8)
        {
            do { s0 = X[(i+0 +1)&0x0f]; s0 = (((((s0))<<(25))|((((s0))&0xffffffff)>>(32-(25)))) ^ ((((s0))<<(14))|((((s0))&0xffffffff)>>(32-(14)))) ^ ((s0)>>3)); s1 = X[(i+0 +14)&0x0f]; s1 = (((((s1))<<(15))|((((s1))&0xffffffff)>>(32-(15)))) ^ ((((s1))<<(13))|((((s1))&0xffffffff)>>(32-(13)))) ^ ((s1)>>10)); T1 = X[(i+0)&0x0f] += s0 + s1 + X[(i+0 +9)&0x0f]; do { T1 += h + (((((e))<<(26))|((((e))&0xffffffff)>>(32-(26)))) ^ ((((e))<<(21))|((((e))&0xffffffff)>>(32-(21)))) ^ ((((e))<<(7))|((((e))&0xffffffff)>>(32-(7))))) + (((e) & (f)) ^ ((~(e)) & (g))) + K256[i+0]; h = (((((a))<<(30))|((((a))&0xffffffff)>>(32-(30)))) ^ ((((a))<<(19))|((((a))&0xffffffff)>>(32-(19)))) ^ ((((a))<<(10))|((((a))&0xffffffff)>>(32-(10))))) + (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c))); d += T1; h += T1; } while (0); } while (0);
            do { s0 = X[(i+1 +1)&0x0f]; s0 = (((((s0))<<(25))|((((s0))&0xffffffff)>>(32-(25)))) ^ ((((s0))<<(14))|((((s0))&0xffffffff)>>(32-(14)))) ^ ((s0)>>3)); s1 = X[(i+1 +14)&0x0f]; s1 = (((((s1))<<(15))|((((s1))&0xffffffff)>>(32-(15)))) ^ ((((s1))<<(13))|((((s1))&0xffffffff)>>(32-(13)))) ^ ((s1)>>10)); T1 = X[(i+1)&0x0f] += s0 + s1 + X[(i+1 +9)&0x0f]; do { T1 += g + (((((d))<<(26))|((((d))&0xffffffff)>>(32-(26)))) ^ ((((d))<<(21))|((((d))&0xffffffff)>>(32-(21)))) ^ ((((d))<<(7))|((((d))&0xffffffff)>>(32-(7))))) + (((d) & (e)) ^ ((~(d)) & (f))) + K256[i+1]; g = (((((h))<<(30))|((((h))&0xffffffff)>>(32-(30)))) ^ ((((h))<<(19))|((((h))&0xffffffff)>>(32-(19)))) ^ ((((h))<<(10))|((((h))&0xffffffff)>>(32-(10))))) + (((h) & (a)) ^ ((h) & (b)) ^ ((a) & (b))); c += T1; g += T1; } while (0); } while (0);
            do { s0 = X[(i+2 +1)&0x0f]; s0 = (((((s0))<<(25))|((((s0))&0xffffffff)>>(32-(25)))) ^ ((((s0))<<(14))|((((s0))&0xffffffff)>>(32-(14)))) ^ ((s0)>>3)); s1 = X[(i+2 +14)&0x0f]; s1 = (((((s1))<<(15))|((((s1))&0xffffffff)>>(32-(15)))) ^ ((((s1))<<(13))|((((s1))&0xffffffff)>>(32-(13)))) ^ ((s1)>>10)); T1 = X[(i+2)&0x0f] += s0 + s1 + X[(i+2 +9)&0x0f]; do { T1 += f + (((((c))<<(26))|((((c))&0xffffffff)>>(32-(26)))) ^ ((((c))<<(21))|((((c))&0xffffffff)>>(32-(21)))) ^ ((((c))<<(7))|((((c))&0xffffffff)>>(32-(7))))) + (((c) & (d)) ^ ((~(c)) & (e))) + K256[i+2]; f = (((((g))<<(30))|((((g))&0xffffffff)>>(32-(30)))) ^ ((((g))<<(19))|((((g))&0xffffffff)>>(32-(19)))) ^ ((((g))<<(10))|((((g))&0xffffffff)>>(32-(10))))) + (((g) & (h)) ^ ((g) & (a)) ^ ((h) & (a))); b += T1; f += T1; } while (0); } while (0);
            do { s0 = X[(i+3 +1)&0x0f]; s0 = (((((s0))<<(25))|((((s0))&0xffffffff)>>(32-(25)))) ^ ((((s0))<<(14))|((((s0))&0xffffffff)>>(32-(14)))) ^ ((s0)>>3)); s1 = X[(i+3 +14)&0x0f]; s1 = (((((s1))<<(15))|((((s1))&0xffffffff)>>(32-(15)))) ^ ((((s1))<<(13))|((((s1))&0xffffffff)>>(32-(13)))) ^ ((s1)>>10)); T1 = X[(i+3)&0x0f] += s0 + s1 + X[(i+3 +9)&0x0f]; do { T1 += e + (((((b))<<(26))|((((b))&0xffffffff)>>(32-(26)))) ^ ((((b))<<(21))|((((b))&0xffffffff)>>(32-(21)))) ^ ((((b))<<(7))|((((b))&0xffffffff)>>(32-(7))))) + (((b) & (c)) ^ ((~(b)) & (d))) + K256[i+3]; e = (((((f))<<(30))|((((f))&0xffffffff)>>(32-(30)))) ^ ((((f))<<(19))|((((f))&0xffffffff)>>(32-(19)))) ^ ((((f))<<(10))|((((f))&0xffffffff)>>(32-(10))))) + (((f) & (g)) ^ ((f) & (h)) ^ ((g) & (h))); a += T1; e += T1; } while (0); } while (0);
            do { s0 = X[(i+4 +1)&0x0f]; s0 = (((((s0))<<(25))|((((s0))&0xffffffff)>>(32-(25)))) ^ ((((s0))<<(14))|((((s0))&0xffffffff)>>(32-(14)))) ^ ((s0)>>3)); s1 = X[(i+4 +14)&0x0f]; s1 = (((((s1))<<(15))|((((s1))&0xffffffff)>>(32-(15)))) ^ ((((s1))<<(13))|((((s1))&0xffffffff)>>(32-(13)))) ^ ((s1)>>10)); T1 = X[(i+4)&0x0f] += s0 + s1 + X[(i+4 +9)&0x0f]; do { T1 += d + (((((a))<<(26))|((((a))&0xffffffff)>>(32-(26)))) ^ ((((a))<<(21))|((((a))&0xffffffff)>>(32-(21)))) ^ ((((a))<<(7))|((((a))&0xffffffff)>>(32-(7))))) + (((a) & (b)) ^ ((~(a)) & (c))) + K256[i+4]; d = (((((e))<<(30))|((((e))&0xffffffff)>>(32-(30)))) ^ ((((e))<<(19))|((((e))&0xffffffff)>>(32-(19)))) ^ ((((e))<<(10))|((((e))&0xffffffff)>>(32-(10))))) + (((e) & (f)) ^ ((e) & (g)) ^ ((f) & (g))); h += T1; d += T1; } while (0); } while (0);
            do { s0 = X[(i+5 +1)&0x0f]; s0 = (((((s0))<<(25))|((((s0))&0xffffffff)>>(32-(25)))) ^ ((((s0))<<(14))|((((s0))&0xffffffff)>>(32-(14)))) ^ ((s0)>>3)); s1 = X[(i+5 +14)&0x0f]; s1 = (((((s1))<<(15))|((((s1))&0xffffffff)>>(32-(15)))) ^ ((((s1))<<(13))|((((s1))&0xffffffff)>>(32-(13)))) ^ ((s1)>>10)); T1 = X[(i+5)&0x0f] += s0 + s1 + X[(i+5 +9)&0x0f]; do { T1 += c + (((((h))<<(26))|((((h))&0xffffffff)>>(32-(26)))) ^ ((((h))<<(21))|((((h))&0xffffffff)>>(32-(21)))) ^ ((((h))<<(7))|((((h))&0xffffffff)>>(32-(7))))) + (((h) & (a)) ^ ((~(h)) & (b))) + K256[i+5]; c = (((((d))<<(30))|((((d))&0xffffffff)>>(32-(30)))) ^ ((((d))<<(19))|((((d))&0xffffffff)>>(32-(19)))) ^ ((((d))<<(10))|((((d))&0xffffffff)>>(32-(10))))) + (((d) & (e)) ^ ((d) & (f)) ^ ((e) & (f))); g += T1; c += T1; } while (0); } while (0);
            do { s0 = X[(i+6 +1)&0x0f]; s0 = (((((s0))<<(25))|((((s0))&0xffffffff)>>(32-(25)))) ^ ((((s0))<<(14))|((((s0))&0xffffffff)>>(32-(14)))) ^ ((s0)>>3)); s1 = X[(i+6 +14)&0x0f]; s1 = (((((s1))<<(15))|((((s1))&0xffffffff)>>(32-(15)))) ^ ((((s1))<<(13))|((((s1))&0xffffffff)>>(32-(13)))) ^ ((s1)>>10)); T1 = X[(i+6)&0x0f] += s0 + s1 + X[(i+6 +9)&0x0f]; do { T1 += b + (((((g))<<(26))|((((g))&0xffffffff)>>(32-(26)))) ^ ((((g))<<(21))|((((g))&0xffffffff)>>(32-(21)))) ^ ((((g))<<(7))|((((g))&0xffffffff)>>(32-(7))))) + (((g) & (h)) ^ ((~(g)) & (a))) + K256[i+6]; b = (((((c))<<(30))|((((c))&0xffffffff)>>(32-(30)))) ^ ((((c))<<(19))|((((c))&0xffffffff)>>(32-(19)))) ^ ((((c))<<(10))|((((c))&0xffffffff)>>(32-(10))))) + (((c) & (d)) ^ ((c) & (e)) ^ ((d) & (e))); f += T1; b += T1; } while (0); } while (0);
            do { s0 = X[(i+7 +1)&0x0f]; s0 = (((((s0))<<(25))|((((s0))&0xffffffff)>>(32-(25)))) ^ ((((s0))<<(14))|((((s0))&0xffffffff)>>(32-(14)))) ^ ((s0)>>3)); s1 = X[(i+7 +14)&0x0f]; s1 = (((((s1))<<(15))|((((s1))&0xffffffff)>>(32-(15)))) ^ ((((s1))<<(13))|((((s1))&0xffffffff)>>(32-(13)))) ^ ((s1)>>10)); T1 = X[(i+7)&0x0f] += s0 + s1 + X[(i+7 +9)&0x0f]; do { T1 += a + (((((f))<<(26))|((((f))&0xffffffff)>>(32-(26)))) ^ ((((f))<<(21))|((((f))&0xffffffff)>>(32-(21)))) ^ ((((f))<<(7))|((((f))&0xffffffff)>>(32-(7))))) + (((f) & (g)) ^ ((~(f)) & (h))) + K256[i+7]; a = (((((b))<<(30))|((((b))&0xffffffff)>>(32-(30)))) ^ ((((b))<<(19))|((((b))&0xffffffff)>>(32-(19)))) ^ ((((b))<<(10))|((((b))&0xffffffff)>>(32-(10))))) + (((b) & (c)) ^ ((b) & (d)) ^ ((c) & (d))); e += T1; a += T1; } while (0); } while (0);
        }

        ctx->h[0] += a; ctx->h[1] += b; ctx->h[2] += c; ctx->h[3] += d;
        ctx->h[4] += e; ctx->h[5] += f; ctx->h[6] += g; ctx->h[7] += h;

    }
}

void sha256_block_host_order (SHA256_CTX *ctx, const void *in, int num)
{ sha256_block (ctx,in,num,1);}

void sha256_block_data_order (SHA256_CTX *ctx, const void *in, int num)
{ sha256_block (ctx,in,num,0);}

#define TEST_MAIN 0

#if TEST_MAIN
int main()
{
    char msg[34567];
    char *p = msg;
    int i;
    char digest[32];
    SHA256_CTX ctx;

    /* init data */
    for (i=0; i< 34567; i++)
    {
        msg[i] = (char) (i % 0x7f);
    }

    /* case 1; do all data */
    SHA256(msg, 34567, digest);

    printf("result -1");
    for (i=0; i<32; i++)
    {
        if (i % 16 ==0) printf ("\n");
        printf("%02x ", (unsigned char)digest[i]);
    }
    printf("\n\n");

    /* case 2, block by block */
    memset(digest, 0, 32);
    SHA256_Init(&ctx);
    i = 34567;
    while (i > 1000)
    { 
        SHA256_Update(&ctx,p,1000);
        p += 1000;
        i -= 1000;
    }
    if (i != 0)
    {
        SHA256_Update(&ctx,p,i);
    }
    SHA256_Final(digest,&ctx);

    printf("result -2");
    for (i=0; i<32; i++)
    {
        if (i % 16 ==0) printf ("\n");
        printf("%02x ", (unsigned char)digest[i]);
    }
    printf("\n\n");

    return 0;
}
#endif 

