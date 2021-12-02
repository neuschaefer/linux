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
 *   WBAMR_PRED_LT4.C
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
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Pred_lt4(
  fract16 exc[],                         /* in/out: excitation buffer */
  fract16 T0,                            /* input : integer pitch lag */
  fract16 frac                           /* input : fraction of lag   */
)
{

  int_native i, j;
  fract32 L_sum;
  fract16 *x;

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
  const fract32 *pInter4_2;
  fract16 *pExc;

  fract32 *pX;
  register fract32 wA, wB, wC, wD;
  register fract32 L_sum1, L_sum2, L_sum3, L_sum4;

  x = &exc[-T0];
  frac = Negate_fr1x16(frac);

  if (frac < 0)
  {
    frac = Add_fr1x16(frac, UP_SAMP);
    x--;
  }

  x = x - L_INTERPOL2 + 1;

  if(x + 35 <= exc) {
     if(_awb_check_word_align(x) == 0) {

        pInter4_2 = inter4_2 + frac * 16;
        pExc = exc;
        pX = (fract32 *)x;

        j = L_SUBFR >> 2;
        do
        {
          L_sum1 = L_sum2 = L_sum3 = L_sum4 = 0L;

          wA = *pInter4_2++;
          wC = *pX++;
          L_sum1 = AWB_smlabb(wA, wC, L_sum1);
          L_sum2 = AWB_smlabt(wA, wC, L_sum2);
          L_sum1 = AWB_smlatt(wA, wC, L_sum1);
          wD = *pX++;
          L_sum3 = AWB_smlabb(wA, wD, L_sum3);
          L_sum4 = AWB_smlabt(wA, wD, L_sum4);
          L_sum3 = AWB_smlatt(wA, wD, L_sum3);
          L_sum2 = AWB_smlatb(wA, wD, L_sum2);
          wB = *pInter4_2++;
          L_sum1 = AWB_smlabb(wB, wD, L_sum1);
          L_sum2 = AWB_smlabt(wB, wD, L_sum2);
          L_sum1 = AWB_smlatt(wB, wD, L_sum1);

          i = L_INTERPOL2 - 2;
          do
          {
             wC = *pX++;
             L_sum4 = AWB_smlatb(wA, wC, L_sum4);
             L_sum2 = AWB_smlatb(wB, wC, L_sum2);
             L_sum3 = AWB_smlabb(wB, wC, L_sum3);
             L_sum4 = AWB_smlabt(wB, wC, L_sum4);
             L_sum3 = AWB_smlatt(wB, wC, L_sum3);
             wA = *pInter4_2++;
             L_sum1 = AWB_smlabb(wA, wC, L_sum1);
             L_sum2 = AWB_smlabt(wA, wC, L_sum2);
             L_sum1 = AWB_smlatt(wA, wC, L_sum1);
             wD = *pX++;
             L_sum4 = AWB_smlatb(wB, wD, L_sum4);
             L_sum3 = AWB_smlabb(wA, wD, L_sum3);
             L_sum4 = AWB_smlabt(wA, wD, L_sum4);
             L_sum3 = AWB_smlatt(wA, wD, L_sum3);
             L_sum2 = AWB_smlatb(wA, wD, L_sum2);
             wB = *pInter4_2++;
             L_sum1 = AWB_smlabb(wB, wD, L_sum1);
             L_sum2 = AWB_smlabt(wB, wD, L_sum2);
             L_sum1 = AWB_smlatt(wB, wD, L_sum1);
          } while ((i -= 2) != 0 );
          wC = *pX++;
          L_sum4 = AWB_smlatb(wA, wC, L_sum4);
          L_sum2 = AWB_smlatb(wB, wC, L_sum2);
          L_sum3 = AWB_smlabb(wB, wC, L_sum3);
          L_sum4 = AWB_smlabt(wB, wC, L_sum4);
          L_sum3 = AWB_smlatt(wB, wC, L_sum3);
          wD = *pX++;
          L_sum4 = AWB_smlatb(wB, wD, L_sum4);

          pInter4_2 -= 16;
          pX -= 16;

          L_sum1 = AWB_qadd(L_sum1, L_sum1);
          L_sum2 = AWB_qadd(L_sum2, L_sum2);
          L_sum3 = AWB_qadd(L_sum3, L_sum3);
          L_sum4 = AWB_qadd(L_sum4, L_sum4);
          L_sum1 = AWB_qdadd(0x8000, L_sum1);
          L_sum2 = AWB_qdadd(0x8000, L_sum2);
          L_sum3 = AWB_qdadd(0x8000, L_sum3);
          L_sum4 = AWB_qdadd(0x8000, L_sum4);

          *pExc++ = L_sum1 >> 16;
          *pExc++ = L_sum2 >> 16;
          *pExc++ = L_sum3 >> 16;
          *pExc++ = L_sum4 >> 16;

        } while (--j != 0);

        i = L_INTERPOL2;
        L_sum1 = 0L;
        do
        {
           wA = *pInter4_2++;
           wC = *pX++;
           L_sum1 = AWB_smlabb(wA, wC, L_sum1);
           L_sum1 = AWB_smlatt(wA, wC, L_sum1);
           wB = *pInter4_2++;
           wD = *pX++;
           L_sum1 = AWB_smlabb(wB, wD, L_sum1);
           L_sum1 = AWB_smlatt(wB, wD, L_sum1);
        } while ( (i -= 2) != 0 );
        L_sum1 = AWB_qadd(L_sum1, L_sum1);
        L_sum1 = AWB_qdadd(0x8000, L_sum1);
        *pExc++ = L_sum1 >> 16;
     }
     else {
        pInter4_2 = inter4_2 + frac * 16;
        pExc = exc;
        pX = (fract32 *)(x-1);

        i = L_INTERPOL2;
        L_sum1 = 0L;
        wC = *pX++;
        do
        {
           wA = *pInter4_2++;
           L_sum1 = AWB_smlabt(wA, wC, L_sum1);
           wD = *pX++;
           L_sum1 = AWB_smlatb(wA, wD, L_sum1);
           wB = *pInter4_2++;
           L_sum1 = AWB_smlabt(wB, wD, L_sum1);
           wC = *pX++;
           L_sum1 = AWB_smlatb(wB, wC, L_sum1);
        } while ( (i -= 2) != 0 );
        L_sum1 = AWB_qadd(L_sum1, L_sum1);
        L_sum1 = AWB_qdadd(0x8000, L_sum1);
        *pExc++ = L_sum1 >> 16;

        pInter4_2 -= 16;
        pX = (fract32 *)(x+1);
        j = L_SUBFR >> 2;
        do
        {
          L_sum1 = L_sum2 = L_sum3 = L_sum4 = 0L;

          wA = *pInter4_2++;
          wC = *pX++;
          L_sum1 = AWB_smlabb(wA, wC, L_sum1);
          L_sum2 = AWB_smlabt(wA, wC, L_sum2);
          L_sum1 = AWB_smlatt(wA, wC, L_sum1);
          wD = *pX++;
          L_sum3 = AWB_smlabb(wA, wD, L_sum3);
          L_sum4 = AWB_smlabt(wA, wD, L_sum4);
          L_sum3 = AWB_smlatt(wA, wD, L_sum3);
          L_sum2 = AWB_smlatb(wA, wD, L_sum2);
          wB = *pInter4_2++;
          L_sum1 = AWB_smlabb(wB, wD, L_sum1);
          L_sum2 = AWB_smlabt(wB, wD, L_sum2);
          L_sum1 = AWB_smlatt(wB, wD, L_sum1);

          i = L_INTERPOL2 - 2;
          do
          {
             wC = *pX++;
             L_sum4 = AWB_smlatb(wA, wC, L_sum4);
             L_sum2 = AWB_smlatb(wB, wC, L_sum2);
             L_sum3 = AWB_smlabb(wB, wC, L_sum3);
             L_sum4 = AWB_smlabt(wB, wC, L_sum4);
             L_sum3 = AWB_smlatt(wB, wC, L_sum3);
             wA = *pInter4_2++;
             L_sum1 = AWB_smlabb(wA, wC, L_sum1);
             L_sum2 = AWB_smlabt(wA, wC, L_sum2);
             L_sum1 = AWB_smlatt(wA, wC, L_sum1);
             wD = *pX++;
             L_sum4 = AWB_smlatb(wB, wD, L_sum4);
             L_sum3 = AWB_smlabb(wA, wD, L_sum3);
             L_sum4 = AWB_smlabt(wA, wD, L_sum4);
             L_sum3 = AWB_smlatt(wA, wD, L_sum3);
             L_sum2 = AWB_smlatb(wA, wD, L_sum2);
             wB = *pInter4_2++;
             L_sum1 = AWB_smlabb(wB, wD, L_sum1);
             L_sum2 = AWB_smlabt(wB, wD, L_sum2);
             L_sum1 = AWB_smlatt(wB, wD, L_sum1);
          } while ((i -= 2) != 0 );
          wC = *pX++;
          L_sum4 = AWB_smlatb(wA, wC, L_sum4);
          L_sum2 = AWB_smlatb(wB, wC, L_sum2);
          L_sum3 = AWB_smlabb(wB, wC, L_sum3);
          L_sum4 = AWB_smlabt(wB, wC, L_sum4);
          L_sum3 = AWB_smlatt(wB, wC, L_sum3);
          wD = *pX++;
          L_sum4 = AWB_smlatb(wB, wD, L_sum4);

          pInter4_2 -= 16;
          pX -= 16;

          L_sum1 = AWB_qadd(L_sum1, L_sum1);
          L_sum2 = AWB_qadd(L_sum2, L_sum2);
          L_sum3 = AWB_qadd(L_sum3, L_sum3);
          L_sum4 = AWB_qadd(L_sum4, L_sum4);
          L_sum1 = AWB_qdadd(0x8000, L_sum1);
          L_sum2 = AWB_qdadd(0x8000, L_sum2);
          L_sum3 = AWB_qdadd(0x8000, L_sum3);
          L_sum4 = AWB_qdadd(0x8000, L_sum4);

          *pExc++ = L_sum1 >> 16;
          *pExc++ = L_sum2 >> 16;
          *pExc++ = L_sum3 >> 16;
          *pExc++ = L_sum4 >> 16;

        } while (--j != 0);
     }
  }
  else {
     pInter4_2 = inter4_2 + frac * 16;
     pExc = exc;

     j = L_SUBFR;
     do
     {
       L_sum = 0L;

       i = L_INTERPOL2;
       do
       {
          L_sum = AWB_smlabb(*x++, *pInter4_2  , L_sum);
          L_sum = AWB_smlabt(*x++, *pInter4_2++, L_sum);
       } while (--i != 0 );
       pInter4_2 -= 16;
       x -= 31;

       L_sum = AWB_qadd(L_sum, L_sum);
       L_sum = AWB_qdadd(0x8000, L_sum);

       *pExc++ = L_sum >> 16;

     } while (j-- != 0);
  }

#else
  int_native k;

  x = &exc[-T0];
  frac = Negate_fr1x16(frac);

  if (frac < 0)
  {
    frac = Add_fr1x16(frac, UP_SAMP);
    x--;
  }

  x = x - L_INTERPOL2 + 1;

  for (j = 0; j < L_SUBFR+1; j++)
  {
    L_sum = 0L;

    for (i = 0, k = Sub_fr1x16(Sub_fr1x16(UP_SAMP, 1), frac);
                   i < 2 * L_INTERPOL2; i++, k += UP_SAMP)

    {
      L_sum = Mac_fr16_32(L_sum, x[i], inter4_2[k]);
    }

    L_sum = Shl_fr1x32(L_sum, 1);

    exc[j] = Round_fr1x32(L_sum);

    x++;
  }
#endif

  return;

}

#endif


