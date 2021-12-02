/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   syn_filt.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR codec implementation
 *
 * Author:
 * -------
 *   Stan Huang
 *
 *------------------------------------------------------------------------------
 * $Revision: #1 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : syn_filt.c
*      Purpose          : Perform synthesis filtering through 1/A(z).
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "syn_filt.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "copy.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
/*
*--------------------------------------*
* Constants (defined in cnst.h         *
*--------------------------------------*
*  M         : LPC order               *
*--------------------------------------*
*/

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/



static void Syn_filt_sub1(
  Word16 *a,
  Word32 *px,
  Word16 *yy,
  Word32 i
)
{
  do{
    Word32 *pyy = (Word32 *)&(yy[-10]);
    Word32 *pa = (Word32 *)&(a[10]);
    Word32 a_t = *pa--;
    Word32 yy_t = *pyy++;
    Word32 x_t = *px++;
    Word32 j, s, t;
    Word32 L_32;
    Word32 L_tmp;

    s = smulbb(a_t, yy_t);
    t = smulbt(a_t, yy_t);
    a_t = *pa--;
    s = smlatt(a_t, yy_t, s);

    j = M/2-1;
    do{
      yy_t = *pyy++;
      t = smlatb( a_t, yy_t, t);
      s = smlabb(a_t, yy_t, s);
      t = smlabt(a_t, yy_t, t);
      a_t = *pa--;
      s = smlatt(a_t, yy_t, s);
    } while( --j != 0 );
    L_32 = smulbb(a_t, x_t);
    L_tmp = smulbt(a_t, x_t);
    L_32 = qadd(L_32, L_32);
    L_tmp = qadd(L_tmp, L_tmp);

    s = qdsub(L_32, s);
    s = qadd(s, s);
    s = qadd(s, s);
    s = extract_h(qdadd(0x8000, s));
    *yy++ = s;

    t = smlatb(a_t, s, t);
    t = qdsub(L_tmp, t);
    t = qadd(t, t);
    t = qadd(t, t);
    t = qdadd(0x8000, t);
    *yy++ = extract_h(t);

/*
    s = smulbb(a_t, yy_t);
    s = qadd(s, s);
    t = smulbt(a_t, yy_t);
    t = qadd(t, t);
    a_t = *pa--;
    s = qdadd(s, smultt(a_t, yy_t));

    j = M/2-1;
    do{
      yy_t = *pyy++;
      t = qdadd(t, smultb(a_t, yy_t));
      s = qdadd(s, smulbb(a_t, yy_t));
      t = qdadd(t, smulbt(a_t, yy_t));
      a_t = *pa--;
      s = qdadd(s, smultt(a_t, yy_t));
    } while( --j != 0 );
    L_32 = smulbb(a_t, x_t);
    L_32 = qadd(L_32, L_32);
    s = qsub(L_32, s);
    s = qadd(s, s);
    s = qadd(s, s);
    s = round(qadd(s, s));
    *yy++ = s;

    t = qdadd(t, smultb(a_t, s));
    L_32 = smulbt(a_t, x_t);
    L_32 = qadd(L_32, L_32);
    t = qsub(L_32, t);
    t = qadd(t, t);
    t = qadd(t, t);
    t = round(qadd(t, t));
    *yy++ = t;
*/

  }while( --i != 0 );
}


void Syn_filt_Post (
	Word32 *tmp_wt,
    Word16 a[],     /* (i)     : a[M+1] prediction coefficients   (M=10)  */
    Word16 x[],     /* (i)     : input signal                             */
    Word16 y[],     /* (o)     : output signal                            */
    Word16 lg,      /* (i)     : size of filtering                        */
    Word16 mem[],   /* (i/o)   : memory associated with this filtering.   */
    Word16 update   /* (i)     : 0=no update, 1=update of memory.         */
)
{
   Word32 i;
   Word16 *yy;
   Word16 *ptr16_mem,*ptr16_y;
//   Word32 tmp_w[M];
   Word32 *tmp_w = tmp_wt;
   Word16 *tmp = (Word16 *)tmp_w;     /* This is usually done by memory allocation (lg+M) */
   Word32 *px = (Word32 *)x;

   /* Copy mem[] to yy[] */
   yy = tmp;
   ptr16_mem = mem;
   i = M/2;
   do
   {
      *yy++ = *ptr16_mem++;
      *yy++ = *ptr16_mem++;
   }while(--i != 0);

	/* Do the filtering. */
   Syn_filt_sub1(a, px, yy, M/2);
   px += M/2;

   AMR_Copy32((Word32 *)&tmp[M], (Word32 *)y, M/2);
   yy = y + M;

   Syn_filt_sub1(a, px, yy, (lg-M)/2);

	/* Update of memory if update==1 */
	if (update != 0)
	{
		ptr16_mem = mem;
		ptr16_y   = &(y[lg-M]);
		i = M/2;
		do
		{
			*ptr16_mem++ = *ptr16_y++;
			*ptr16_mem++ = *ptr16_y++;
		}while(--i != 0);
	}

	return;

}


void Syn_filt (
	Word32 *tmp_wt,
    Word16 a[],     /* (i)     : a[M+1] prediction coefficients   (M=10)  */
    Word16 x[],     /* (i)     : input signal                             */
    Word16 y[],     /* (o)     : output signal                            */
    Word16 lg,      /* (i)     : size of filtering                        */
    Word16 mem[],   /* (i/o)   : memory associated with this filtering.   */
    Word16 update   /* (i)     : 0=no update, 1=update of memory.         */
)
{
   Word32 *ptr321, *ptr322 ;
   Word16 *yy;
//   Word32 tmp_w[M];
   Word32 *tmp_w = tmp_wt;

   Word16 *tmp = (Word16 *)tmp_w;     /* This is usually done by memory allocation (lg+M) */
   Word32 *px = (Word32 *)x;

	/* Copy mem[] to yy[] */
   ptr321  = (Word32 *) tmp;
	ptr322 = (Word32 *) mem;
   *ptr321++ = *ptr322++;
   *ptr321++ = *ptr322++;
   *ptr321++ = *ptr322++;
   *ptr321++ = *ptr322++;
   *ptr321++ = *ptr322++;
	yy = (Word16 *)ptr321;

	/* Do the filtering. */
  Syn_filt_sub1(a, px, yy, M/2);
  px += M/2;

  AMR_Copy32((Word32 *)&tmp[M], (Word32 *)y, M/2);
  yy = y + M;

  Syn_filt_sub1(a, px, yy, (lg-M)/2);

	/* Update of memory if update==1 */
	if (update != 0)
	{
		ptr321 = (Word32 *)mem;
		ptr322 = (Word32 *)&(y[lg-M]);
		*ptr321++ = *ptr322++;
		*ptr321++ = *ptr322++;
		*ptr321++ = *ptr322++;
		*ptr321++ = *ptr322++;
		*ptr321++ = *ptr322++;
	}

	return;
}

#endif

