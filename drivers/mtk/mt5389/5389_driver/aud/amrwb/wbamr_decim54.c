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
 *   WBAMR_DECIM54.C
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
 * $Revision: #1 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 * 12 23 2010 eddy.wu
 * [WCPSP00000503] Checkin SWIP Modification
 * .
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_acelp.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"



WBAMR_CODE_SECTION void WBAMR_Init_Decim_12k8 (
  fract16 mem[]         /* output: memory (2*NB_COEF_DOWN) set to zeros */
)
{

  WBAMR_Set_zero ( mem, 2 * NB_COEF_DOWN );

  return;

}


WBAMR_CODE_SECTION void WBAMR_Decim_12k8 (
  fract16 sig16k[],            /* input:  signal to downsampling  */
  fract16 lg,                  /* input:  length of input         */
  fract16 sig12k8[],           /* output: decimated signal        */
  fract16 mem[],               /* in/out: memory (2*NB_COEF_DOWN) */
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr
)
{

  fract16 lg_down;
  void* wbamr_enc_signal_buf_ptr;
  fract16* signal;


    wbamr_enc_signal_buf_ptr = enc_scr_ptr->u1.signal;


  signal = wbamr_enc_signal_buf_ptr;
  WBAMR_Copy ( mem, signal, 2 * NB_COEF_DOWN );
  WBAMR_Copy ( sig16k, signal + ( 2 * NB_COEF_DOWN ), lg );
  lg_down = Mult_fr1x16 ( lg, DOWN_FAC );
  WBAMR_Down_samp ( signal + NB_COEF_DOWN, sig12k8, lg_down );
  WBAMR_Copy ( signal + lg, mem, 2 * NB_COEF_DOWN );


  return;

}



WBAMR_CODE_SECTION void WBAMR_Init_Oversamp_16k (
  fract16 mem[]           /* output: memory (2*NB_COEF_UP) set to zeros */
)
{

  WBAMR_Set_zero(mem, 2 * NB_COEF_UP);

  return;

}


WBAMR_CODE_SECTION void WBAMR_Oversamp_16k (
  fract16 sig12k8[],     /* input:  signal to oversampling  */
  fract16 lg,            /* input:  length of input         */
  fract16 sig16k[],      /* output: oversampled signal      */
  fract16 mem[],         /* in/out: memory (2*NB_COEF_UP)   */
  WBAMR_Temp_Mem_Dec_Struct  *dec_scr_ptr
)
{

  fract16 lg_up;

  WBAMR_Copy ( mem, dec_scr_ptr->u.signal1, 2 * NB_COEF_UP );
  WBAMR_Copy ( sig12k8, dec_scr_ptr->u.signal1 + ( 2 * NB_COEF_UP ), lg );
  lg_up = Shl_fr1x16 ( Mult_fr1x16 ( lg, UP_FAC ), 1 );
  WBAMR_Up_samp ( dec_scr_ptr->u.signal1 + NB_COEF_UP, sig16k, lg_up );
  WBAMR_Copy ( dec_scr_ptr->u.signal1 + lg, mem, 2 * NB_COEF_UP );

    return;

}




WBAMR_CODE_SECTION void WBAMR_Down_samp (
  fract16 * sig,            /* input:  signal to downsampling  */
  fract16 * sig_d,          /* output: downsampled signal      */
  fract16 L_frame_d         /* input:  length of output        */
)
{
#if (AWB_OPT_VER == 2)
   int_native i, j;
  fract32 *fir, *psig;
  fract32 L_sum1, L_sum2, L_sum3, L_sum4;
  fract32 wA, wB, wC, wD;
  int flip;

  if(_awb_check_word_align(sig) == 0) {
     psig = (fract32 *)(sig - NB_COEF_DOWN + 1);
     flip = 0;
  }
  else {
     psig = (fract32 *)(sig - NB_COEF_DOWN);
     flip = 1;
  }

  fir = (fract32 *)(fir_down);
//4p
  j = L_frame_d >> 2;
  do
  {
     if(flip == 0) {
       L_sum1 = L_sum2 = L_sum3 = L_sum4 = 0x2000;
       wA = *psig++;
       wC = *fir++;
       L_sum1 = AWB_smlabb(wA, wC, L_sum1);
       L_sum1 = AWB_smlatt(wA, wC, L_sum1);
       wD = *fir++;
       L_sum2 = AWB_smlatb(wA, wD, L_sum2);
       wA = *psig++;
       wC = *fir++;
       L_sum1 = AWB_smlabb(wA, wC, L_sum1);
       L_sum1 = AWB_smlatt(wA, wC, L_sum1);
       wC = *fir++;
       L_sum2 = AWB_smlabb(wA, wC, L_sum2);
       L_sum2 = AWB_smlatt(wA, wC, L_sum2);
       wC = *fir++;
       L_sum3 = AWB_smlabb(wA, wC, L_sum3);
       L_sum3 = AWB_smlatt(wA, wC, L_sum3);
       L_sum4 = AWB_smlatt(wA, wD, L_sum4);

       i = 13;
       do {
          wA = *psig++;
          wC = *fir++;
          L_sum1 = AWB_smlabb(wA, wC, L_sum1);
          L_sum1 = AWB_smlatt(wA, wC, L_sum1);
          wC = *fir++;
          L_sum2 = AWB_smlabb(wA, wC, L_sum2);
          L_sum2 = AWB_smlatt(wA, wC, L_sum2);
          wC = *fir++;
          L_sum3 = AWB_smlabb(wA, wC, L_sum3);
          L_sum3 = AWB_smlatt(wA, wC, L_sum3);
          wC = *fir++;
          L_sum4 = AWB_smlabb(wA, wC, L_sum4);
          L_sum4 = AWB_smlatt(wA, wC, L_sum4);
       } while(--i != 0);
       wA = *psig++;
       wD = *fir++;
       L_sum2 = AWB_smlabb(wA, wD, L_sum2);
       wC = *fir++;
       L_sum3 = AWB_smlabb(wA, wC, L_sum3);
       L_sum3 = AWB_smlatt(wA, wC, L_sum3);
       wC = *fir++;
       L_sum4 = AWB_smlabb(wA, wC, L_sum4);
       L_sum4 = AWB_smlatt(wA, wC, L_sum4);
       wA = *psig++;
       L_sum4 = AWB_smlabt(wA, wD, L_sum4);

       *sig_d++ = L_sum1 >> 14;
       *sig_d++ = L_sum2 >> 14;
       *sig_d++ = L_sum3 >> 14;
       *sig_d++ = L_sum4 >> 14;

       psig -= 15;
       fir -= 60;
       flip = 1;
     }
     else
     {

       // for signal not aligned
       L_sum1 = L_sum2 = L_sum3 = L_sum4 = 0x2000;
       wA = *psig++;
       wB = *psig++;
       wC = *fir++;
       L_sum1 = AWB_smlatb(wA, wC, L_sum1);
       L_sum1 = AWB_smlabt(wB, wC, L_sum1);
       wD = *fir++;
       L_sum2 = AWB_smlabb(wB, wD, L_sum2);
       wA = wB;
       wB = *psig++;
       wC = *fir++;
       L_sum1 = AWB_smlatb(wA, wC, L_sum1);
       L_sum1 = AWB_smlabt(wB, wC, L_sum1);
       wC = *fir++;
       L_sum2 = AWB_smlatb(wA, wC, L_sum2);
       L_sum2 = AWB_smlabt(wB, wC, L_sum2);
       wC = *fir++;
       L_sum3 = AWB_smlatb(wA, wC, L_sum3);
       L_sum3 = AWB_smlabt(wB, wC, L_sum3);
       L_sum4 = AWB_smlabt(wB, wD, L_sum4);

       i = 13;
       do {
          wA = wB;
          wB = *psig++;
          wC = *fir++;
          L_sum1 = AWB_smlatb(wA, wC, L_sum1);
          L_sum1 = AWB_smlabt(wB, wC, L_sum1);
          wC = *fir++;
          L_sum2 = AWB_smlatb(wA, wC, L_sum2);
          L_sum2 = AWB_smlabt(wB, wC, L_sum2);
          wC = *fir++;
          L_sum3 = AWB_smlatb(wA, wC, L_sum3);
          L_sum3 = AWB_smlabt(wB, wC, L_sum3);
          wC = *fir++;
          L_sum4 = AWB_smlatb(wA, wC, L_sum4);
          L_sum4 = AWB_smlabt(wB, wC, L_sum4);
       } while(--i != 0);
       wA = wB;
       wB = *psig++;
       wD = *fir++;
       L_sum2 = AWB_smlatb(wA, wD, L_sum2);
       wC = *fir++;
       L_sum3 = AWB_smlatb(wA, wC, L_sum3);
       L_sum3 = AWB_smlabt(wB, wC, L_sum3);
       wC = *fir++;
       L_sum4 = AWB_smlatb(wA, wC, L_sum4);
       L_sum4 = AWB_smlabt(wB, wC, L_sum4);
       L_sum4 = AWB_smlatt(wB, wD, L_sum4);

       *sig_d++ = L_sum1 >> 14;
       *sig_d++ = L_sum2 >> 14;
       *sig_d++ = L_sum3 >> 14;
       *sig_d++ = L_sum4 >> 14;

       psig -= 14;
       fir -= 60;
       flip = 0;
     }

  } while( --j != 0);
#elif (AWB_OPT_VER == 1)
   int_native j, i1;
   fract32 L_sum;
   fract16 *fir;

   sig = sig - NB_COEF_DOWN + 1;
   fir = &fir_down[0];

   j = L_frame_d >> 2;
   do
   {
      L_sum = 0L;
      i1 = 30;
      do {
         L_sum = AWB_smlabb(*sig++, *fir++, L_sum);
      } while(--i1 != 0);

      L_sum = Shl_fr1x32(L_sum, 2);          /* saturation can occur here */
      *sig_d++ = Round_fr1x32(L_sum);
      sig -= 29;

      L_sum = 0L;
      i1 = 30;
      do {
         L_sum = AWB_smlabb(*sig++, *fir++, L_sum);
      } while(--i1 != 0);

      L_sum = Shl_fr1x32(L_sum, 2);          /* saturation can occur here */
      *sig_d++ = Round_fr1x32(L_sum);
      sig -= 29;

      L_sum = 0L;
      i1 = 30;
      do {
         L_sum = AWB_smlabb(*sig++, *fir++, L_sum);
      } while(--i1 != 0);

      L_sum = Shl_fr1x32(L_sum, 2);          /* saturation can occur here */
      *sig_d++ = Round_fr1x32(L_sum);
      sig -= 29;

      L_sum = 0L;
      i1 = 30;
      do {
         L_sum = AWB_smlabb(*sig++, *fir++, L_sum);
      } while(--i1 != 0);

      L_sum = Shl_fr1x32(L_sum, 2);          /* saturation can occur here */
      *sig_d++ = Round_fr1x32(L_sum);
      sig -= 28;
      fir -= 120;
   } while( --j != 0);
#else
  int_native i, j, i1, k;
  fract32 L_sum;
  fract16 frac, pos;
  fract16 *x;

  pos = 0;                 /* position is in Q2 -> 1/4 resolution  */

  for (j = 0; j < L_frame_d; j++)
  {
    i = Shr_fr1x16(pos, 2);               /* integer part     */
    frac = (fract16) (pos & 3);           /* fractional part */
    x=&sig[i - NB_COEF_DOWN + 1];
    L_sum = 0L;

    for (i1 = 0, k = Sub_fr1x16(Sub_fr1x16(FAC4, 1), frac);
             i1 < 2 * NB_COEF_DOWN; i1++, k = (fract16) (k + FAC4))
    {
      L_sum = Mac_fr16_32(L_sum, x[i1], fir_down[k]);
    }

    L_sum = Shl_fr1x32(L_sum, 1);          /* saturation can occur here */

    sig_d[j]=Round_fr1x32(L_sum);

    pos = Add_fr1x16(pos, FAC5);          /* pos + 5/4 */

  }
#endif

  return;

}



WBAMR_CODE_SECTION void WBAMR_Up_samp(
  fract16 * sig_d,            /* input:  signal to oversampling  */
  fract16 * sig_u,            /* output: oversampled signal      */
  fract16 L_frame             /* input:  length of output        */
)
{

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
   fract32 *pX0, *pXn;
   fract32 L_sum1, L_sum2, L_sum3, L_sum4;
   fract32 wFir, wX0, wXn;
   int_native i, j;
   const fract32 *pFir;

   pFir = fir_up;
   pX0 = (fract32 *)(&sig_d[-12]);
   pXn = pX0 + 13;

   L_frame = L_frame / 5;
   for (j = 0; j < L_frame; j++) {
      wFir = *pFir++;
      wX0 = *pX0++;
      wXn = *pXn--;
      L_sum1 = L_sum2 = L_sum3 = L_sum4 = 0;
      L_sum1 = AWB_smlatb(wX0, wFir, L_sum1);
      L_sum4 = AWB_smlatb(wXn, wFir, L_sum4);
      wFir = *pFir++;
      L_sum3 = AWB_smlabt(wXn, wFir, L_sum3);
      L_sum4 = AWB_smlabb(wXn, wFir, L_sum4);
      i = 5;
      do {
         wX0 = *pX0++;
         L_sum1 = AWB_smlabb(wX0, wFir, L_sum1);
         L_sum2 = AWB_smlabt(wX0, wFir, L_sum2);
         wFir = *pFir++;
         wXn = *pXn--;
         L_sum3 = AWB_smlatt(wXn, wFir, L_sum3);
         L_sum4 = AWB_smlatb(wXn, wFir, L_sum4);
         L_sum1 = AWB_smlatb(wX0, wFir, L_sum1);
         L_sum2 = AWB_smlatt(wX0, wFir, L_sum2);
         wFir = *pFir++;
         L_sum3 = AWB_smlabt(wXn, wFir, L_sum3);
         L_sum4 = AWB_smlabb(wXn, wFir, L_sum4);
      } while(--i != 0);
      wX0 = *pX0++;
      *sig_u++ = (fract16)((wX0 << 16) >> 16);
      L_sum1 = AWB_smlabb(wX0, wFir, L_sum1);
      L_sum2 = AWB_smlabt(wX0, wFir, L_sum2);
      i = 6;
      do {
         wFir = *pFir++;
         wXn = *pXn--;
         L_sum3 = AWB_smlatt(wXn, wFir, L_sum3);
         L_sum4 = AWB_smlatb(wXn, wFir, L_sum4);
         L_sum1 = AWB_smlatb(wX0, wFir, L_sum1);
         L_sum2 = AWB_smlatt(wX0, wFir, L_sum2);
         wFir = *pFir++;
         L_sum3 = AWB_smlabt(wXn, wFir, L_sum3);
         L_sum4 = AWB_smlabb(wXn, wFir, L_sum4);
         wX0 = *pX0++;
         L_sum1 = AWB_smlabb(wX0, wFir, L_sum1);
         L_sum2 = AWB_smlabt(wX0, wFir, L_sum2);
      } while(--i != 0);
      wFir = *pFir;
      wXn = *pXn;
      L_sum3 = AWB_smlatt(wXn, wFir, L_sum3);
      L_sum2 = AWB_smlatt(wX0, wFir, L_sum2);

      L_sum1 = AWB_qadd(L_sum1, L_sum1);
      L_sum1 = AWB_qdadd(0x8000, L_sum1);
      L_sum2 = AWB_qadd(L_sum2, L_sum2);
      L_sum2 = AWB_qdadd(0x8000, L_sum2);
      L_sum3 = AWB_qadd(L_sum3, L_sum3);
      L_sum3 = AWB_qdadd(0x8000, L_sum3);
      L_sum4 = AWB_qadd(L_sum4, L_sum4);
      L_sum4 = AWB_qdadd(0x8000, L_sum4);

      *sig_u++ = (fract16)(L_sum1 >> 16);
      *sig_u++ = (fract16)(L_sum2 >> 16);
      *sig_u++ = (fract16)(L_sum3 >> 16);
      *sig_u++ = (fract16)(L_sum4 >> 16);

      pFir -= 24;
      pX0 -= 11;
      pXn += 14;

   }
#else
  int_native i, j, k, i1;
  fract32 L_sum;
  fract16 pos, frac;
  fract16 *x;

  pos = 0;                               /* position with 1/5 resolution */

  for (j = 0; j < L_frame; j++)
  {
    i = Mult_fr1x16(pos, INV_FAC5);      /* integer part = pos * 1/5 */

    /* frac = pos - (pos/5)*5   */
    frac = Sub_fr1x16(pos, Add_fr1x16(Shl_fr1x16((fract16)i, 2), (fract16)i));
    x=&sig_d[i - NB_COEF_UP + 1];
    L_sum = 0L;

    for (i1 = 0, k = Sub_fr1x16(Sub_fr1x16(FAC5, 1), frac);
                i1 < 2 * NB_COEF_UP; i1++, k = (fract16) (k + FAC5))
    {
      L_sum = Mac_fr16_32(L_sum, x[i1], fir_up[k]);
    }

    L_sum = Shl_fr1x32(L_sum, 1);        /* saturation can occur here */
    sig_u[j]=Round_fr1x32(L_sum);
    pos = Add_fr1x16(pos, FAC4);         /* position + 4/5 */

  }
#endif
  return;

}

#endif

