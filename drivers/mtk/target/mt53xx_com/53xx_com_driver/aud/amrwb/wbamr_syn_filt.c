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
 *   WBAMR_SYN_FILT.C
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_math_op.h"
#include "wbamr_cnst.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"



WBAMR_CODE_SECTION void WBAMR_Syn_filt(
   fract16 a[],                    /* (i) Q12 : a[m+1] prediction coefficients           */
   fract16 m,                      /* (i)     : order of LP filter                       */
   fract16 x[],                    /* (i)     : input signal                             */
   fract16 y[],                    /* (o)     : output signal                            */
   fract16 lg,                     /* (i)     : size of filtering                        */
   fract16 mem[],                  /* (i/o)   : memory associated with this filtering.   */
   fract16 update,                 /* (i)     : 0=no update, 1=update of memory.         */
   Common_Scratch * com_scr_ptr
)
{
#if (AWB_OPT_VER == 2)
  fract32 i, j;
  fract32 L_tmp1, L_tmp2, L_tmp3, L_tmp4;
  fract32 wA, wB, wC, wD;
  fract16 *yy16;
  fract32 a0, filt_len, filt_len_4;
  fract32 *yyptr, *aptr, *xptr, *y32;


  yy16 = com_scr_ptr->u.y_buf;
  filt_len = (fract32)m;

  j = filt_len >> 2;
  do
  {
    *yy16++ = *mem++;
    *yy16++ = *mem++;
    *yy16++ = *mem++;
    *yy16++ = *mem++;
  } while (--j != 0);
  mem -= filt_len;

  if(_awb_check_word_align(a) == 0)
  {
     //4p
     yyptr = (fract32 *)com_scr_ptr->u.y_buf;
     aptr = (fract32 *)(a + filt_len);
     xptr = (fract32 *)x;
     y32 = (fract32 *)y;
     filt_len_4 = (filt_len >> 2) - 2;
     i = (fract32)lg >> 2;
     do
     {
        wC = *yyptr++;
        wA = *aptr--;
        L_tmp1 = AWB_smlabb(wA, wC, 0x800);
        L_tmp2 = AWB_smlabt(wA, wC, 0x800);
        wB = *aptr--;
        L_tmp1 = AWB_smlatt(wB, wC, L_tmp1);
        wD = *yyptr++;
        L_tmp3 = AWB_smlabb(wA, wD, 0x800);
        L_tmp4 = AWB_smlabt(wA, wD, 0x800);
        L_tmp2 = AWB_smlatb(wB, wD, L_tmp2);
        L_tmp3 = AWB_smlatt(wB, wD, L_tmp3);
        L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
        L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
        wA = *aptr--;
        L_tmp1 = AWB_smlatt(wA, wD, L_tmp1);

        j = filt_len_4; // 3, 4
        do
        {
           wC = *yyptr++;
           L_tmp4 = AWB_smlatb(wB, wC, L_tmp4);
           L_tmp3 = AWB_smlabb(wB, wC, L_tmp3);
           L_tmp4 = AWB_smlabt(wB, wC, L_tmp4);
           L_tmp2 = AWB_smlatb(wA, wC, L_tmp2);
           L_tmp3 = AWB_smlatt(wA, wC, L_tmp3);
           L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
           L_tmp2 = AWB_smlabt(wA, wC, L_tmp2);
           wB = *aptr--;
           L_tmp1 = AWB_smlatt(wB, wC, L_tmp1);
           wD = *yyptr++;
           L_tmp4 = AWB_smlatb(wA, wD, L_tmp4);
           L_tmp3 = AWB_smlabb(wA, wD, L_tmp3);
           L_tmp4 = AWB_smlabt(wA, wD, L_tmp4);
           L_tmp2 = AWB_smlatb(wB, wD, L_tmp2);
           L_tmp3 = AWB_smlatt(wB, wD, L_tmp3);
           L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
           L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
           wA = *aptr--;
           L_tmp1 = AWB_smlatt(wA, wD, L_tmp1);
        } while( --j != 0);
        wC = *yyptr++;
        L_tmp4 = AWB_smlatb(wB, wC, L_tmp4);
        L_tmp3 = AWB_smlabb(wB, wC, L_tmp3);
        L_tmp4 = AWB_smlabt(wB, wC, L_tmp4);
        L_tmp2 = AWB_smlatb(wA, wC, L_tmp2);
        L_tmp3 = AWB_smlatt(wA, wC, L_tmp3);
        L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
        L_tmp2 = AWB_smlabt(wA, wC, L_tmp2);
        wB = *aptr--;
        L_tmp1 = AWB_smlatt(wB, wC, L_tmp1);
        wD = *yyptr++;
        L_tmp4 = AWB_smlatb(wA, wD, L_tmp4);
        L_tmp3 = AWB_smlabb(wA, wD, L_tmp3);
        L_tmp4 = AWB_smlabt(wA, wD, L_tmp4);
        L_tmp2 = AWB_smlatb(wB, wD, L_tmp2);
        L_tmp3 = AWB_smlatt(wB, wD, L_tmp3);
        L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
        L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
        wA = *aptr--;
        L_tmp1 = AWB_qadd(L_tmp1, AWB_smultt(wA, wD));

        wD = *xptr++;
        L_tmp1 = AWB_qdsub(AWB_smulbb(wD, wA), L_tmp1); // x[i] * a[0]
        if(AWB_clz(L_tmp1 ^ (L_tmp1 >> 31)) > 3) {
           L_tmp1 >>= 13;
        }
        else if(L_tmp1 < 0) {
           L_tmp1 = 0x8000;
        }
        else {
           L_tmp1 = 0x7fff;
        }

        L_tmp2 = AWB_qadd(L_tmp2, AWB_smultb(wA, L_tmp1)); //L_tmp1
        L_tmp2 = AWB_qdsub(AWB_smultb(wD, wA), L_tmp2); // x[i] * a[0]
        if(AWB_clz(L_tmp2 ^ (L_tmp2 >> 31)) > 3) {
           L_tmp2 = (L_tmp2 >> 13) << 16;
        }
        else if(L_tmp1 < 0) {
           L_tmp2 = 0x80000000L;
        }
        else {
           L_tmp2 = 0x7fff0000L;
        }

        *y32++ = *yyptr++ = L_tmp1 + L_tmp2;

        L_tmp4 = AWB_smlatb(wB, L_tmp1, L_tmp4); //L_tmp1
        L_tmp3 = AWB_smlabb(wB, L_tmp1, L_tmp3); //L_tmp1
        L_tmp4 = AWB_smlabt(wB, L_tmp2, L_tmp4); //L_tmp2
        L_tmp3 = AWB_qadd(L_tmp3, AWB_smultt(wA, L_tmp2)); //L_tmp2

        wD = *xptr++;
        L_tmp3 = AWB_qdsub(AWB_smulbb(wD, wA), L_tmp3); // x[i] * a[0]
        if(AWB_clz(L_tmp3 ^ (L_tmp3 >> 31)) > 3) {
           L_tmp3 >>= 13;
        }
        else if(L_tmp1 < 0) {
           L_tmp3 = 0x8000;
        }
        else {
           L_tmp3 = 0x7fff;
        }

        L_tmp4 = AWB_qadd(L_tmp4, AWB_smultb(wA, L_tmp3)); //L_tmp3
        L_tmp4 = AWB_qdsub(AWB_smultb(wD, wA), L_tmp4); // x[i] * a[0]
        if(AWB_clz(L_tmp4 ^ (L_tmp4 >> 31)) > 3) {
           L_tmp4 = (L_tmp4 >> 13) << 16;
        }
        else if(L_tmp1 < 0) {
           L_tmp4 = 0x80000000L;
        }
        else {
           L_tmp4 = 0x7fff0000L;
        }
        *y32++ = *yyptr++ = L_tmp3 + L_tmp4;


        yyptr -= (filt_len >> 1); // 8; 10  (16 / 20)
        aptr += (filt_len >> 1) + 1; // 9; 11 (16 / 20)
     } while(--i != 0);

  }
  else //if(_awb_check_word_align(a) == 0)
  {
     //4p
     yyptr = (fract32 *)com_scr_ptr->u.y_buf;
     aptr = (fract32 *)(a + filt_len - 1);
     xptr = (fract32 *)x;
     y32 = (fract32 *)y;
     a0 = (fract32)a[0];
     filt_len_4 = (filt_len >> 2) - 2;
     i = (fract32)lg >> 2;
     do
     {
        wC = *yyptr++;
        wA = *aptr--;
        L_tmp1 = AWB_smlatb(wA, wC, 0x800);
        L_tmp2 = AWB_smlatt(wA, wC, 0x800);
        L_tmp1 = AWB_smlabt(wA, wC, L_tmp1);
        wD = *yyptr++;
        L_tmp3 = AWB_smlatb(wA, wD, 0x800);
        L_tmp2 = AWB_smlabb(wA, wD, L_tmp2);
        L_tmp4 = AWB_smlatt(wA, wD, 0x800);
        L_tmp3 = AWB_smlabt(wA, wD, L_tmp3);
        wB = *aptr--;
        L_tmp1 = AWB_smlatb(wB, wD, L_tmp1);
        L_tmp2 = AWB_smlatt(wB, wD, L_tmp2);
        L_tmp1 = AWB_smlabt(wB, wD, L_tmp1);

        j = filt_len_4;
        do
        {
	        wC = *yyptr++;
	        L_tmp3 = AWB_smlatb(wB, wC, L_tmp3);
	        L_tmp4 = AWB_smlatt(wB, wC, L_tmp4);
	        L_tmp2 = AWB_smlabb(wB, wC, L_tmp2);
	        L_tmp3 = AWB_smlabt(wB, wC, L_tmp3);
	        L_tmp4 = AWB_smlabb(wA, wC, L_tmp4);
	        wA = *aptr--;
	        L_tmp1 = AWB_smlatb(wA, wC, L_tmp1);
	        L_tmp2 = AWB_smlatt(wA, wC, L_tmp2);
	        L_tmp1 = AWB_smlabt(wA, wC, L_tmp1);
	        wD = *yyptr++;
	        L_tmp3 = AWB_smlatb(wA, wD, L_tmp3);
	        L_tmp4 = AWB_smlatt(wA, wD, L_tmp4);
	        L_tmp2 = AWB_smlabb(wA, wD, L_tmp2);
	        L_tmp3 = AWB_smlabt(wA, wD, L_tmp3);
	        L_tmp4 = AWB_smlabb(wB, wD, L_tmp4);
	        wB = *aptr--;
	        L_tmp1 = AWB_smlatb(wB, wD, L_tmp1);
	        L_tmp2 = AWB_smlatt(wB, wD, L_tmp2);
	        L_tmp1 = AWB_smlabt(wB, wD, L_tmp1);
        } while( --j != 0);
	     wC = *yyptr++;
	     L_tmp3 = AWB_smlatb(wB, wC, L_tmp3);
	     L_tmp4 = AWB_smlatt(wB, wC, L_tmp4);
	     L_tmp2 = AWB_smlabb(wB, wC, L_tmp2);
	     L_tmp3 = AWB_smlabt(wB, wC, L_tmp3);
	     L_tmp4 = AWB_smlabb(wA, wC, L_tmp4);
	     wA = *aptr--;
	     L_tmp1 = AWB_smlatb(wA, wC, L_tmp1);
	     L_tmp2 = AWB_smlatt(wA, wC, L_tmp2);
	     L_tmp1 = AWB_smlabt(wA, wC, L_tmp1);
	     wD = *yyptr++;
	     L_tmp3 = AWB_smlatb(wA, wD, L_tmp3);
	     L_tmp4 = AWB_smlatt(wA, wD, L_tmp4);
	     L_tmp2 = AWB_smlabb(wA, wD, L_tmp2);
	     L_tmp3 = AWB_smlabt(wA, wD, L_tmp3);
	     L_tmp4 = AWB_smlabb(wB, wD, L_tmp4);
	     wB = *aptr--;
	     L_tmp1 = AWB_smlatb(wB, wD, L_tmp1);
	     L_tmp2 = AWB_smlatt(wB, wD, L_tmp2);
	     L_tmp1 = AWB_qadd(L_tmp1, AWB_smulbt(wB, wD));


        wD = *xptr++;
        L_tmp1 = AWB_qdsub(AWB_smulbb(wD, a0), L_tmp1); // x[i] * a[0]
        if(AWB_clz(L_tmp1 ^ (L_tmp1 >> 31)) > 3) {
           L_tmp1 >>= 13;
        }
        else if(L_tmp1 < 0) {
           L_tmp1 = 0x8000;
        }
        else {
           L_tmp1 = 0x7fff;
        }

        L_tmp2 = AWB_qadd(L_tmp2, AWB_smulbb(wB, L_tmp1)); //L_tmp1
        L_tmp2 = AWB_qdsub(AWB_smultb(wD, a0), L_tmp2); // x[i] * a[0]
        if(AWB_clz(L_tmp2 ^ (L_tmp2 >> 31)) > 3) {
           L_tmp2 = (L_tmp2 >> 13) << 16;
        }
        else if(L_tmp1 < 0) {
           L_tmp1 = 0x80000000L;
        }
        else {
           L_tmp1 = 0x7fff0000L;
        }

        *y32++ = *yyptr++ = L_tmp1 + L_tmp2;

        L_tmp4 = AWB_smlabb(wA, L_tmp1, L_tmp4); //L_tmp1
        L_tmp3 = AWB_smlatb(wB, L_tmp1, L_tmp3); //L_tmp1
        L_tmp4 = AWB_smlatt(wB, L_tmp2, L_tmp4); //L_tmp2
        L_tmp3 = AWB_qadd(L_tmp3, AWB_smulbt(wB, L_tmp2)); //L_tmp2

        wD = *xptr++;
        L_tmp3 = AWB_qdsub(AWB_smulbb(wD, a0), L_tmp3); // x[i] * a[0]
        if(AWB_clz(L_tmp3 ^ (L_tmp3 >> 31)) > 3) {
           L_tmp3 >>= 13;
        }
        else if(L_tmp1 < 0) {
           L_tmp3 = 0x8000;
        }
        else {
           L_tmp3 = 0x7fff;
        }
        L_tmp4 = AWB_qadd(L_tmp4, AWB_smulbb(wB, L_tmp3)); //L_tmp3
        L_tmp4 = AWB_qdsub(AWB_smultb(wD, a0), L_tmp4); // x[i] * a[0]
        if(AWB_clz(L_tmp4 ^ (L_tmp4 >> 31)) > 3) {
           L_tmp4 = (L_tmp4 >> 13) << 16;
        }
        else if(L_tmp1 < 0) {
           L_tmp4 = 0x80000000L;
        }
        else {
           L_tmp4 = 0x7fff0000L;
        }
        *y32++ = *yyptr++ = L_tmp3 + L_tmp4;


        yyptr -= (filt_len >> 1); // 8; 10  (16 / 20)
        aptr += (filt_len >> 1); // 8; 10 (16 / 20)
     } while(--i != 0);

  }
  if (update) {
    yy16 = (fract16 *)yyptr;
    j = filt_len >> 2;
    do
    {
      *mem++ = *yy16++;
      *mem++ = *yy16++;
      *mem++ = *yy16++;
      *mem++ = *yy16++;
    } while(--j != 0);
  }

#elif (AWB_OPT_VER == 1)
   fract32 j;
   fract32 L_tmp;
   fract32 wA, wY;
   fract32 *yy, *pA;
   fract32 a0;
   fract16 *yy16;

   yy16 = com_scr_ptr->u.y_buf;

   j = m;
   do
   {
      *yy16++ = *mem++;
   } while (--j != 0);
   mem -= m;

   m >>= 1;
   a0 = a[0] >> 1;
   yy = (fract32 *)com_scr_ptr->u.y_buf + m - 1;

   if(_awb_check_word_align(a) == 0)
   {
      pA = (fract32 *)a;
      lg >>= 1;
      do
      {
         wA = *pA++;
         L_tmp = Mult_fr1x32(*x++, a0);

         j = m;
         do {
            wY = *yy--;
            L_tmp = AWB_qdsub(L_tmp, AWB_smultt(wA, wY));
            wA = *pA++;
            L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(wA, wY));
         } while(--j != 0);

         L_tmp = Shl_fr1x32(L_tmp, 3);

         yy += (m + 1);
         pA -= (m + 1);

         *y = Round_fr1x32(L_tmp);
         *yy = (*yy & 0xffff0000) + *y++;

         wA = *pA++;
         wY = *yy--;
         L_tmp = Mult_fr1x32(*x++, a0);
         j = m;
         do {
            L_tmp = AWB_qdsub(L_tmp, AWB_smultb(wA, wY));
            wY = *yy--;
            wA = *pA++;
            L_tmp = AWB_qdsub(L_tmp, AWB_smulbt(wA, wY));
         } while(--j != 0);
         L_tmp = Shl_fr1x32(L_tmp, 3);

         yy += (m + 1);
         pA -= (m + 1);

         *y = Round_fr1x32(L_tmp);
         *yy = (*yy & 0x0000ffff) + (*y++ << 16);
      } while (--lg != 0);
   }
   else
   {
      // not word aligned
      pA = (fract32 *)(&a[1]);

      lg >>= 1;
      do
      {
         L_tmp = Mult_fr1x32(*x++, a0);
         j = m;
         do {
            wA = *pA++;
            wY = *yy--;
            L_tmp = AWB_qdsub(L_tmp, AWB_smulbt(wA, wY));
            L_tmp = AWB_qdsub(L_tmp, AWB_smultb(wA, wY));
         } while(--j != 0);

         L_tmp = Shl_fr1x32(L_tmp, 3);
         yy += (m + 1);
         pA -= m;

         *y = Round_fr1x32(L_tmp);
         *yy = (*yy & 0xffff0000) + *y++;

         L_tmp = Mult_fr1x32(*x++, a0);
         wY = *yy--;
         j = m;
         do {
            wA = *pA++;
            L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(wA, wY));
            wY = *yy--;
            L_tmp = AWB_qdsub(L_tmp, AWB_smultt(wA, wY));
         } while(--j != 0);
         L_tmp = Shl_fr1x32(L_tmp, 3);

         yy += (m + 1);
         pA -= m;

         *y = Round_fr1x32(L_tmp);
         *yy = (*yy & 0x0000ffff) + (*y++ << 16);
     } while (--lg != 0);
   }
   if (update) {
      yy16 = (fract16 *)(yy - m + 1);
      j = m << 1;
      do
      {
         *mem++ = *yy16++;
      } while(--j != 0);
   }
#else
  int_native i, j;
  fract32 L_tmp;
  fract16 a0;
  fract16 *yy;

  yy = com_scr_ptr->u.y_buf;

  /* copy initial filter states into synthesis buffer */
  for (i = 0; i < m; i++)
  {
    *yy++ = mem[i];
  }

  a0 = Shr_fr1x16(a[0], 1);        /* input / 2 */

  /* Do the filtering. */
  for (i = 0; i < lg; i++)
  {
    L_tmp = Mult_fr1x32(x[i], a0);

    for (j = 1; j <= m; j++)
      L_tmp = WBAMR_L_msu(L_tmp, a[j], yy[i - j]);

    L_tmp = Shl_fr1x32(L_tmp, 3);
    y[i] = yy[i] = Round_fr1x32(L_tmp);
  }

  /* Update memory if required */
  if (update)
    for (i = 0; i < m; i++)
    {
      mem[i] = yy[lg - m + i];
    }

  return;
#endif

}

#if (AWB_OPT_VER == 2)
WBAMR_CODE_SECTION void WBAMR_Syn_filt_32 (
   fract16 a[],                     /* (i) Q12 : a[m+1] prediction coefficients */
   fract16 exc[],                   /* (i) Qnew: excitation (exc[i] >> Qnew)    */
   fract16 Qnew,                    /* (i)     : exc scaling = 0(min) to 8(max) */
   fract32 sig32[]                /* (o) /16 : synthesis high                 */
)
{
   fract32 i, j;
   fract32 a0;
   fract32 L_tmp1, L_tmp2, L_tmp3, L_tmp4, wA, wB, wC, wD, wExc;
   fract32 *psig, *aptr, *pexc;

   a0 = (fract32)a[0] >> ( 4 + Qnew);

   if(_awb_check_word_align(a) == 0)
   {
      //4p
      aptr = (fract32 *)(a + 16);
      psig = sig32 - 16;
      pexc = (fract32 *)exc;
      i = L_SUBFR >> 2;
      do
      {
         wC = *psig++;
         wA = *aptr--;
         L_tmp1 = AWB_smulwb(wC, wA);
         wC = *psig++;
         L_tmp2 = AWB_smulwb(wC, wA);
         wB = *aptr--;
         L_tmp1 = AWB_smlawt(wC, wB, L_tmp1);
         wC = *psig++;
         L_tmp3 = AWB_smulwb(wC, wA);
         L_tmp2 = AWB_smlawt(wC, wB, L_tmp2);
         L_tmp1 = AWB_smlawb(wC, wB, L_tmp1);
         wC = *psig++;
         L_tmp4 = AWB_smulwb(wC, wA);
         L_tmp3 = AWB_smlawt(wC, wB, L_tmp3);
         L_tmp2 = AWB_smlawb(wC, wB, L_tmp2);
         wA = *aptr--;
         L_tmp1 = AWB_smlawt(wC, wA, L_tmp1);
         if(L_tmp1 == 0xE0000000L) {
            L_tmp1 += 1;
         }

         j = 3;
         do
         {
            wC = *psig++;
            L_tmp4 = AWB_smlawt(wC, wB, L_tmp4);
            L_tmp3 = AWB_smlawb(wC, wB, L_tmp3);
            L_tmp2 = AWB_smlawt(wC, wA, L_tmp2);
            L_tmp1 = AWB_smlawb(wC, wA, L_tmp1);
            wC = *psig++;
            L_tmp4 = AWB_smlawb(wC, wB, L_tmp4);
            L_tmp3 = AWB_smlawt(wC, wA, L_tmp3);
            L_tmp2 = AWB_smlawb(wC, wA, L_tmp2);
            wB = *aptr--;
            L_tmp1 = AWB_smlawt(wC, wB, L_tmp1);
            wC = *psig++;
            L_tmp4 = AWB_smlawt(wC, wA, L_tmp4);
            L_tmp3 = AWB_smlawb(wC, wA, L_tmp3);
            L_tmp2 = AWB_smlawt(wC, wB, L_tmp2);
            L_tmp1 = AWB_smlawb(wC, wB, L_tmp1);
            wC = *psig++;
            L_tmp4 = AWB_smlawb(wC, wA, L_tmp4);
            L_tmp3 = AWB_smlawt(wC, wB, L_tmp3);
            L_tmp2 = AWB_smlawb(wC, wB, L_tmp2);
            wA = *aptr--;
            L_tmp1 = AWB_smlawt(wC, wA, L_tmp1);
         } while( --j != 0);
         wExc = *pexc++;
         L_tmp1 = AWB_qsub(AWB_smulbb(wExc, a0), L_tmp1);
         if(AWB_clz(L_tmp1 ^ (L_tmp1 >> 31)) > 4) {
            L_tmp1 <<= 4;
         }
         else if(L_tmp1 < 0) {
            L_tmp1 = 0x80000000L;
         }
         else {
            L_tmp1 = 0x7ffffff0L;
         }
         *psig++ = L_tmp1;

         L_tmp2 = AWB_qadd(L_tmp2, AWB_smulwt(L_tmp1, wA)); //L_tmp1
         L_tmp2 = AWB_qsub(AWB_smultb(wExc, a0), L_tmp2); // x[i] * a[0]
         if(AWB_clz(L_tmp2 ^ (L_tmp2 >> 31)) > 4) {
            L_tmp2 = L_tmp2 << 4;
         }
         else if(L_tmp1 < 0) {
            L_tmp2 = 0x80000000L;
         }
         else {
            L_tmp2 = 0x7ffffff0L;
         }
         *psig++ = L_tmp2;

         L_tmp4 = AWB_smlawt(L_tmp1, wB, L_tmp4); //L_tmp1
         L_tmp3 = AWB_smlawb(L_tmp1, wB, L_tmp3); //L_tmp1
         L_tmp4 = AWB_smlawb(L_tmp2, wB, L_tmp4); //L_tmp2
         L_tmp3 = AWB_qadd(L_tmp3, AWB_smulwt(L_tmp2, wA)); //L_tmp2

         wExc = *pexc++;
         L_tmp3 = AWB_qsub(AWB_smulbb(wExc, a0), L_tmp3); // x[i] * a[0]
         if(AWB_clz(L_tmp3 ^ (L_tmp3 >> 31)) > 4) {
            L_tmp3 <<= 4;
         }
         else if(L_tmp1 < 0) {
            L_tmp3 = 0x80000000L;
         }
         else {
            L_tmp3 = 0x7ffffff0L;
         }
         *psig++ = L_tmp3;

         L_tmp4 = AWB_qadd(L_tmp4, AWB_smulwt(L_tmp3, wA)); //L_tmp3
         L_tmp4 = AWB_qsub(AWB_smultb(wExc, a0), L_tmp4); // x[i] * a[0]
         if(AWB_clz(L_tmp4 ^ (L_tmp4 >> 31)) > 4) {
            L_tmp4 <<= 4;
         }
         else if(L_tmp1 < 0) {
            L_tmp4 = 0x80000000L;
         }
         else {
            L_tmp4 = 0x7ffffff0L;
         }
         *psig++ = L_tmp4;


         psig -= 16;
         aptr += 9;
      } while(--i != 0);

   }
   else //if(_awb_check_word_align(a) == 0)
   {
      aptr = (fract32 *)(a + 15);
      psig = sig32 - 16;
      pexc = (fract32 *)exc;
      i = L_SUBFR >> 2;
      do
      {
         wC = *psig++;
         wA = *aptr--;
         L_tmp1 = AWB_smulwt(wC, wA);
         wC = *psig++;
         L_tmp2 = AWB_smulwt(wC, wA);
         L_tmp1 = AWB_smlawb(wC, wA, L_tmp1);
         wC = *psig++;
         L_tmp3 = AWB_smulwt(wC, wA);
         L_tmp2 = AWB_smlawb(wC, wA, L_tmp2);
         wD = *psig++;
         L_tmp4 = AWB_smulwt(wD, wA);
         L_tmp3 = AWB_smlawb(wD, wA, L_tmp3);
         wB = *aptr--;
         L_tmp1 = AWB_smlawt(wC, wB, L_tmp1);
         L_tmp2 = AWB_smlawt(wD, wB, L_tmp2);
         L_tmp1 = AWB_smlawb(wD, wB, L_tmp1);
         if(L_tmp1 == 0xE0000000L) {
            L_tmp1 += 1;
         }

         j = 3;
         do
         {
            wC = *psig++;
            L_tmp4 = AWB_smlawb(wC, wA, L_tmp4);
            L_tmp3 = AWB_smlawt(wC, wB, L_tmp3);
            L_tmp2 = AWB_smlawb(wC, wB, L_tmp2);
            wD = *psig++;
            L_tmp4 = AWB_smlawt(wD, wB, L_tmp4);
            L_tmp3 = AWB_smlawb(wD, wB, L_tmp3);
            wA = *aptr--;
            L_tmp1 = AWB_smlawt(wC, wA, L_tmp1);
            L_tmp2 = AWB_smlawt(wD, wA, L_tmp2);
            L_tmp1 = AWB_smlawb(wD, wA, L_tmp1);
            wC = *psig++;
            L_tmp4 = AWB_smlawb(wC, wB, L_tmp4);
            L_tmp3 = AWB_smlawt(wC, wA, L_tmp3);
            L_tmp2 = AWB_smlawb(wC, wA, L_tmp2);
            wD = *psig++;
            L_tmp4 = AWB_smlawt(wD, wA, L_tmp4);
            L_tmp3 = AWB_smlawb(wD, wA, L_tmp3);
            wB = *aptr--;
            L_tmp1 = AWB_smlawt(wC, wB, L_tmp1);
            L_tmp2 = AWB_smlawt(wD, wB, L_tmp2);
            L_tmp1 = AWB_smlawb(wD, wB, L_tmp1);
         } while( --j != 0);


         wExc = *pexc++;
         L_tmp1 = AWB_qsub(AWB_smulbb(wExc, a0), L_tmp1); // x[i] * a[0]
         if(AWB_clz(L_tmp1 ^ (L_tmp1 >> 31)) > 4) {
            L_tmp1 <<= 4;
         }
         else if(L_tmp1 < 0) {
            L_tmp1 = 0x80000000L;
         }
         else {
            L_tmp1 = 0x7ffffff0L;
         }
         *psig++ = L_tmp1;

         L_tmp2 = AWB_qadd(L_tmp2, AWB_smulwb(L_tmp1, wB)); //L_tmp1
         L_tmp2 = AWB_qsub(AWB_smultb(wExc, a0), L_tmp2); // x[i] * a[0]
         if(AWB_clz(L_tmp2 ^ (L_tmp2 >> 31)) > 4) {
            L_tmp2 <<= 4;
         }
         else if(L_tmp1 < 0) {
            L_tmp2 = 0x80000000L;
         }
         else {
           L_tmp2 = 0x7fff0000L;
         }
         *psig++ = L_tmp2;

         L_tmp4 = AWB_smlawb(L_tmp1, wA, L_tmp4); //L_tmp1
         L_tmp3 = AWB_smlawt(L_tmp1, wB, L_tmp3); //L_tmp1
         L_tmp4 = AWB_smlawt(L_tmp2, wB, L_tmp4); //L_tmp2
         L_tmp3 = AWB_qadd(L_tmp3, AWB_smulwb(L_tmp2, wB)); //L_tmp2

         wExc = *pexc++;
         L_tmp3 = AWB_qsub(AWB_smulbb(wExc, a0), L_tmp3); // x[i] * a[0]
         if(AWB_clz(L_tmp3 ^ (L_tmp3 >> 31)) > 4) {
            L_tmp3 <<= 4;
         }
         else if(L_tmp1 < 0) {
            L_tmp3 = 0x80000000L;
         }
         else {
            L_tmp3 = 0x7ffffff0L;
         }
         *psig++ = L_tmp3;

         L_tmp4 = AWB_qadd(L_tmp4, AWB_smulwb(L_tmp3, wB)); //L_tmp3
         L_tmp4 = AWB_qsub(AWB_smultb(wExc, a0), L_tmp4); // x[i] * a[0]
         if(AWB_clz(L_tmp4 ^ (L_tmp4 >> 31)) > 4) {
            L_tmp4 <<= 4;
         }
         else if(L_tmp1 < 0) {
            L_tmp4 = 0x80000000L;
         }
         else {
            L_tmp4 = 0x7fff0000L;
         }
         *psig++ = L_tmp4;

         psig -= 16;
         aptr += 8;
      } while(--i != 0);

   }

    return;
}
#else
WBAMR_CODE_SECTION void WBAMR_Syn_filt_32(
     fract16 a[],                     /* (i) Q12 : a[m+1] prediction coefficients */
     fract16 exc[],                   /* (i) Qnew: excitation (exc[i] >> Qnew)    */
     fract16 Qnew,                    /* (i)     : exc scaling = 0(min) to 8(max) */
     fract16 sig_hi[],                /* (o) /16 : synthesis high                 */
     fract16 sig_lo[]                /* (o) /16 : synthesis low                  */

)
{
    fract16 i;
    fract16 a0;
    fract32 L_tmp;

#if (AWB_OPT_VER == 1)
    fract32 L_tmp2;

    //a0 = a[0] >> (4 + Qnew);   /* input / 16 and >>Qnew */
    a0 = a[0] >> (4 + Qnew);

    sig_lo -= 1;
    sig_hi -= 1;
    a += 1;
    /* Do the filtering. */

    i = L_SUBFR;
    do
    {
        L_tmp = AWB_smulbb(*sig_lo--, *a++);
        L_tmp = AWB_smlabb(*sig_lo--, *a++, L_tmp);
        L_tmp = AWB_smlabb(*sig_lo--, *a++, L_tmp);
        L_tmp = AWB_smlabb(*sig_lo--, *a++, L_tmp);
        L_tmp = AWB_smlabb(*sig_lo--, *a++, L_tmp);
        L_tmp = AWB_smlabb(*sig_lo--, *a++, L_tmp);
        L_tmp = AWB_smlabb(*sig_lo--, *a++, L_tmp);
        L_tmp = AWB_smlabb(*sig_lo--, *a++, L_tmp);
        L_tmp = AWB_qdsub(0, L_tmp);
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_lo--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_lo--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_lo--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_lo--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_lo--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_lo--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_lo--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_lo--, *a++));
        a -= 16;
        sig_lo += 16;

        //L_tmp >>= 12;        /* -4 : sig_lo[i] << 4 */
        L_tmp >>= 12;
        L_tmp = Mac_fr16_32(L_tmp, *exc++, a0);

        L_tmp2 = AWB_smlabb(*sig_hi--, *a++, 0);
        L_tmp2 = AWB_smlabb(*sig_hi--, *a++, L_tmp2);
        L_tmp2 = AWB_smlabb(*sig_hi--, *a++, L_tmp2);
        L_tmp2 = AWB_smlabb(*sig_hi--, *a++, L_tmp2);
        L_tmp2 = AWB_smlabb(*sig_hi--, *a++, L_tmp2);
        L_tmp2 = AWB_smlabb(*sig_hi--, *a++, L_tmp2);
        L_tmp2 = AWB_smlabb(*sig_hi--, *a++, L_tmp2);
        L_tmp = AWB_qdsub(L_tmp, L_tmp2);
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));
        L_tmp = AWB_qdsub(L_tmp, AWB_smulbb(*sig_hi--, *a++));

        a -= 16;
        sig_hi += 16;

        /* sig_hi = bit16 to bit31 of synthesis */
        L_tmp = AWB_qadd(L_tmp, L_tmp);
        L_tmp = AWB_qadd(L_tmp, L_tmp);
        L_tmp = AWB_qadd(L_tmp, L_tmp);
        *(++sig_hi) = Extract_hi_fr1x32(L_tmp);

        /* sig_lo = bit4 to bit15 of synthesis */
        //L_tmp >>= 4;
        L_tmp >>= 4;             /* 4 : sig_lo[i] >> 4 */
        *(++sig_lo) = Extract_lo_fr1x32(WBAMR_L_msu(L_tmp, *sig_hi, 2048)); 
    } while( --i != 0);
#else
   fract16 j;

    a0 = Shr_fr1x16(a[0], Add_fr1x16(4, Qnew));   /* input / 16 and >>Qnew */

    /* Do the filtering. */

    for (i = 0; i < L_SUBFR; i++)
    {
        L_tmp = 0;
        for (j = 1; j <= M11; j++)
            L_tmp = WBAMR_L_msu(L_tmp, sig_lo[i - j], a[j]);

        L_tmp = Shr_fr1x32(L_tmp, 16 - 4);        /* -4 : sig_lo[i] << 4 */

        L_tmp = Mac_fr16_32(L_tmp, exc[i], a0);

        for (j = 1; j <= M11; j++)
            L_tmp = WBAMR_L_msu(L_tmp, sig_hi[i - j], a[j]);

        /* sig_hi = bit16 to bit31 of synthesis */
        L_tmp = Shl_fr1x32(L_tmp, 3);             /* ai in Q12 */

        sig_hi[i] = Extract_hi_fr1x32(L_tmp);      

        /* sig_lo = bit4 to bit15 of synthesis */
        L_tmp = Shr_fr1x32(L_tmp, 4);             /* 4 : sig_lo[i] >> 4 */
        sig_lo[i] = Extract_lo_fr1x32(WBAMR_L_msu(L_tmp, sig_hi[i], 2048));   
    }

#endif

    return;
}
#endif

#endif

