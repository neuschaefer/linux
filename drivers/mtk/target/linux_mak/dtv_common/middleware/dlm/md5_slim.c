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

#include "md5_slim.h"

int MD5_Update (MD5_CTX *c, const void *data_, int len)
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

        if ((c->num+len) >= 64)
        {
            l=p[sw]; { switch (sc)
                {
                case 0: l =((unsigned long)(*((data)++)));
                case 1: l|=((unsigned long)(*((data)++)))<< 8;
                case 2: l|=((unsigned long)(*((data)++)))<<16;
                case 3: l|=((unsigned long)(*((data)++)))<<24;
                }}; p[sw++]=l;
            for (; sw<(64/4); sw++)
            {
                ((l)=*((const unsigned int *)(data)), (data)+=4, l); p[sw]=l;
            }
            md5_block_host_order (c,p,1);
            len-=(64 -c->num);
            c->num=0;

        }
        else
        {
            c->num+=(unsigned int)len;
            if ((sc+len) < 4)
            {
                l=p[sw]; { switch (sc)
                    {
                    case 0: l =((unsigned long)(*((data)++))); if (--len == 0) break;
                    case 1: l|=((unsigned long)(*((data)++)))<< 8; if (--len == 0) break;
                    case 2: l|=((unsigned long)(*((data)++)))<<16;
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
                    case 0: l =((unsigned long)(*((data)++)));
                    case 1: l|=((unsigned long)(*((data)++)))<< 8;
                    case 2: l|=((unsigned long)(*((data)++)))<<16;
                    case 3: l|=((unsigned long)(*((data)++)))<<24;
                    }};
                p[sw++]=l;
                for (; sw < ew; sw++)
                {
                    ((l)=*((const unsigned int *)(data)), (data)+=4, l); p[sw]=l;
                }
                if (ec)
                {
                    { l=0; (data)+=ec; switch (ec)
                        {
                        case 3: l =((unsigned long)(*(--(data))))<<16;
                        case 2: l|=((unsigned long)(*(--(data))))<< 8;
                        case 1: l|=((unsigned long)(*(--(data))));
                        }}; p[sw]=l;
                }
            }
            return 1;
        }
    }

    sw=len/64;
    if (sw > 0)
    {
        if ((((int)data)%4) == 0)
        {

            md5_block_host_order (c,(const unsigned int *)data,sw);
            sw*=64;
            data+=sw;
            len-=sw;
        }
        else
        {
            md5_block_host_order(c,data,sw);
            sw*=64;
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
            ((l)=*((const unsigned int *)(data)), (data)+=4, l); *p=l;
        }
        { l=0; (data)+=ec; switch (ec)
            {
            case 3: l =((unsigned long)(*(--(data))))<<16;
            case 2: l|=((unsigned long)(*(--(data))))<< 8;
            case 1: l|=((unsigned long)(*(--(data))));
            }};
        *p=l;
    }
    return 1;
}


void MD5_Transform (MD5_CTX *c, const unsigned char *data)
{

    if ((((int)data)%4) == 0)

        md5_block_host_order (c,(const unsigned int *)data,1);
    else
        md5_block_host_order (c,data,1);
}


int MD5_Final (unsigned char *md, MD5_CTX *c)
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
        case 0: l =((unsigned long)(*((cp)++)));
        case 1: l|=((unsigned long)(*((cp)++)))<< 8;
        case 2: l|=((unsigned long)(*((cp)++)))<<16;
        case 3: l|=((unsigned long)(*((cp)++)))<<24;
        }}; p[i++]=l;

    if (i>((64/4)-2))
    {
        if (i<(64/4)) p[i]=0;
        md5_block_host_order (c,p,1);
        i=0;
    }
    for (; i<((64/4)-2); i++)
        p[i]=0;

    p[(64/4)-2]=c->Nl;
    p[(64/4)-1]=c->Nh;

    md5_block_host_order (c,p,1);

    do
    {
        unsigned long ll; ll=(c)->A; (*((unsigned int *)((md)))=(ll), ((md))+=4, ll); ll=(c)->B; (*((unsigned int *)((md)))=(ll), ((md))+=4, ll); ll=(c)->C; (*((unsigned int *)((md)))=(ll), ((md))+=4, ll); ll=(c)->D; (*((unsigned int *)((md)))=(ll), ((md))+=4, ll);
    } while (0);
    c->num=0;
    return 1;
}

const char MD5_version[]="MD5" " part of " "OpenSSL 0.9.8e 23 Feb 2007";

int MD5_Init(MD5_CTX *c)
{
    c->A=(unsigned long)0x67452301L;
    c->B=(unsigned long)0xefcdab89L;
    c->C=(unsigned long)0x98badcfeL;
    c->D=(unsigned long)0x10325476L;
    c->Nl=0;
    c->Nh=0;
    c->num=0;
    return 1;
}


void md5_block_host_order (MD5_CTX *c, const void *data, int num)
{
    const unsigned int *X=data;
    register unsigned long A,B,C,D;

    A=c->A;
    B=c->B;
    C=c->C;
    D=c->D;

    for (;num--;X+=(64/4))
    {
        
        { A+=((X[ 0])+(0xd76aa478L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B;};;
        { D+=((X[ 1])+(0xe8c7b756L)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A;};;
        { C+=((X[ 2])+(0x242070dbL)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D;};;
        { B+=((X[ 3])+(0xc1bdceeeL)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C;};;
        { A+=((X[ 4])+(0xf57c0fafL)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B;};;
        { D+=((X[ 5])+(0x4787c62aL)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A;};;
        { C+=((X[ 6])+(0xa8304613L)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D;};;
        { B+=((X[ 7])+(0xfd469501L)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C;};;
        { A+=((X[ 8])+(0x698098d8L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B;};;
        { D+=((X[ 9])+(0x8b44f7afL)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A;};;
        { C+=((X[10])+(0xffff5bb1L)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D;};;
        { B+=((X[11])+(0x895cd7beL)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C;};;
        { A+=((X[12])+(0x6b901122L)+(((((C)) ^ ((D))) & ((B))) ^ ((D)))); A=(((A)<<(7))|(((A)&0xffffffff)>>(32-(7)))); A+=B;};;
        { D+=((X[13])+(0xfd987193L)+(((((B)) ^ ((C))) & ((A))) ^ ((C)))); D=(((D)<<(12))|(((D)&0xffffffff)>>(32-(12)))); D+=A;};;
        { C+=((X[14])+(0xa679438eL)+(((((A)) ^ ((B))) & ((D))) ^ ((B)))); C=(((C)<<(17))|(((C)&0xffffffff)>>(32-(17)))); C+=D;};;
        { B+=((X[15])+(0x49b40821L)+(((((D)) ^ ((A))) & ((C))) ^ ((A)))); B=(((B)<<(22))|(((B)&0xffffffff)>>(32-(22)))); B+=C;};;

        { A+=((X[ 1])+(0xf61e2562L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B;};;
        { D+=((X[ 6])+(0xc040b340L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A;};;
        { C+=((X[11])+(0x265e5a51L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D;};;
        { B+=((X[ 0])+(0xe9b6c7aaL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C;};;
        { A+=((X[ 5])+(0xd62f105dL)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B;};;
        { D+=((X[10])+(0x02441453L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A;};;
        { C+=((X[15])+(0xd8a1e681L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D;};;
        { B+=((X[ 4])+(0xe7d3fbc8L)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C;};;
        { A+=((X[ 9])+(0x21e1cde6L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B;};;
        { D+=((X[14])+(0xc33707d6L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A;};;
        { C+=((X[ 3])+(0xf4d50d87L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D;};;
        { B+=((X[ 8])+(0x455a14edL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C;};;
        { A+=((X[13])+(0xa9e3e905L)+(((((B)) ^ ((C))) & ((D))) ^ ((C)))); A=(((A)<<(5))|(((A)&0xffffffff)>>(32-(5)))); A+=B;};;
        { D+=((X[ 2])+(0xfcefa3f8L)+(((((A)) ^ ((B))) & ((C))) ^ ((B)))); D=(((D)<<(9))|(((D)&0xffffffff)>>(32-(9)))); D+=A;};;
        { C+=((X[ 7])+(0x676f02d9L)+(((((D)) ^ ((A))) & ((B))) ^ ((A)))); C=(((C)<<(14))|(((C)&0xffffffff)>>(32-(14)))); C+=D;};;
        { B+=((X[12])+(0x8d2a4c8aL)+(((((C)) ^ ((D))) & ((A))) ^ ((D)))); B=(((B)<<(20))|(((B)&0xffffffff)>>(32-(20)))); B+=C;};;

        { A+=((X[ 5])+(0xfffa3942L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B;};;
        { D+=((X[ 8])+(0x8771f681L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A;};;
        { C+=((X[11])+(0x6d9d6122L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D;};;
        { B+=((X[14])+(0xfde5380cL)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C;};;
        { A+=((X[ 1])+(0xa4beea44L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B;};;
        { D+=((X[ 4])+(0x4bdecfa9L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A;};;
        { C+=((X[ 7])+(0xf6bb4b60L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D;};;
        { B+=((X[10])+(0xbebfbc70L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C;};;
        { A+=((X[13])+(0x289b7ec6L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B;};;
        { D+=((X[ 0])+(0xeaa127faL)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A;};;
        { C+=((X[ 3])+(0xd4ef3085L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D;};;
        { B+=((X[ 6])+(0x04881d05L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C;};;
        { A+=((X[ 9])+(0xd9d4d039L)+(((B)) ^ ((C)) ^ ((D)))); A=(((A)<<(4))|(((A)&0xffffffff)>>(32-(4)))); A+=B;};;
        { D+=((X[12])+(0xe6db99e5L)+(((A)) ^ ((B)) ^ ((C)))); D=(((D)<<(11))|(((D)&0xffffffff)>>(32-(11)))); D+=A;};;
        { C+=((X[15])+(0x1fa27cf8L)+(((D)) ^ ((A)) ^ ((B)))); C=(((C)<<(16))|(((C)&0xffffffff)>>(32-(16)))); C+=D;};;
        { B+=((X[ 2])+(0xc4ac5665L)+(((C)) ^ ((D)) ^ ((A)))); B=(((B)<<(23))|(((B)&0xffffffff)>>(32-(23)))); B+=C;};;

        { A+=((X[ 0])+(0xf4292244L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B;};;
        { D+=((X[ 7])+(0x432aff97L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A;};;
        { C+=((X[14])+(0xab9423a7L)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D;};;
        { B+=((X[ 5])+(0xfc93a039L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C;};;
        { A+=((X[12])+(0x655b59c3L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B;};;
        { D+=((X[ 3])+(0x8f0ccc92L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A;};;
        { C+=((X[10])+(0xffeff47dL)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D;};;
        { B+=((X[ 1])+(0x85845dd1L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C;};;
        { A+=((X[ 8])+(0x6fa87e4fL)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B;};;
        { D+=((X[15])+(0xfe2ce6e0L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A;};;
        { C+=((X[ 6])+(0xa3014314L)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D;};;
        { B+=((X[13])+(0x4e0811a1L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C;};;
        { A+=((X[ 4])+(0xf7537e82L)+(((~((D))) | ((B))) ^ ((C)))); A=(((A)<<(6))|(((A)&0xffffffff)>>(32-(6)))); A+=B;};;
        { D+=((X[11])+(0xbd3af235L)+(((~((C))) | ((A))) ^ ((B)))); D=(((D)<<(10))|(((D)&0xffffffff)>>(32-(10)))); D+=A;};;
        { C+=((X[ 2])+(0x2ad7d2bbL)+(((~((B))) | ((D))) ^ ((A)))); C=(((C)<<(15))|(((C)&0xffffffff)>>(32-(15)))); C+=D;};;
        { B+=((X[ 9])+(0xeb86d391L)+(((~((A))) | ((C))) ^ ((D)))); B=(((B)<<(21))|(((B)&0xffffffff)>>(32-(21)))); B+=C;};;

        A = c->A += A;
        B = c->B += B;
        C = c->C += C;
        D = c->D += D;
    }
}

