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
 *   WBAMR_P_MED_OL.C
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
#include "wbamr_oper_32b.h"
#include "wbamr_math_op.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"



WBAMR_CODE_SECTION fract16 WBAMR_Pitch_med_ol(
			   /* o : open loop pitch lag */
  fract16 wsp[],           /* i : signal used to compute the open loop pitch */
                           /*  wsp[-pit_max] to wsp[-1] should be known*/
  fract16 L_min,           /* input : minimum pitch lag */
  fract16 L_max,           /* input : maximum pitch lag */
  fract16 L_frame,         /* input : length of frame to compute pitch */
  fract16 L_0,             /* input : old_ open-loop pitch */
  fract16 * gain,          /* o: norm correlation of hp_wsp for the Lag  */
  fract16 * hp_wsp_mem,    /* i:o   : memory of the hi filter for hp_wsp[] */
  fract16 * old_hp_wsp,    /* i:o   : hypass wsp[] */
  fract16 wght_flg,        /* input : is weighting function used */
  fract16* enc_obj_temp_ptr
)
{

  int_native i, j;
  fract32 max, R0, R1, R2;
  fract16 Tm;
  fract16 hi, lo;
  const fract16 *ww, *we;
  fract16 *hp_wsp;
  fract16 exp_R0, exp_R1, exp_R2;

#if (AWB_OPT_VER == 2)
  int_native k;
  fract32 R3, L_frame_2, wA, wB, wC, wD;
  fract32 *wsp_s, *wsp_o;
  fract32 tmpRx[4] = {0, 0, 0, 0};

  ww = &corrweight[198];
  we = &corrweight[98 + L_max - L_0];
  max = MIN_32;
  Tm = 0;

  if(_awb_check_word_align(wsp) == 0)
  {
     for (i = L_max; i > L_min; i--)
     {
       /* Compute the correlation */
       R0 = 0;

       for (j = 0; j < L_frame; j++)
         R0 = Mac_fr16_32(R0, wsp[j], wsp[j - i]);

       //printf("%d\n", R0);
       /* Weighting of the correlation function.   */
       WBAMR_L_Extract(R0, &hi, &lo);
       R0 = WBAMR_Mpy_32_16(hi, lo, *ww);
       ww--;

       if ((L_0 > 0) && (wght_flg > 0))
       {
         /* Weight the neighbourhood of the old lag. */
         WBAMR_L_Extract(R0, &hi, &lo);
         R0 = WBAMR_Mpy_32_16(hi, lo, *we);
         we--;
       }

       if (Sub_fr1x32(R0, max) >= 0)
       {
         max = R0;
         Tm =(fract16) i;
       }
     }
  }
  else // not word_aligned
  {
     //4p
     L_0 = ((L_0 > 0) && (wght_flg > 0)) ? 0 : 1;
     L_frame_2 = (L_frame >> 3) - 1;
     wsp_s = (fract32 *)(wsp - 115);
     wsp_o = (fract32 *)(wsp - 1);
     i = 96;
     do
     {
        wC = *wsp_s++;
        wA = *wsp_o++;
        wD = *wsp_s++;
        k = 2;
        do
        {
           R0 = R1 = R2 = R3 = 0;
           R0 = AWB_smlatb(wA, wC, R0);
           R1 = AWB_smlatt(wA, wC, R1);
           wB = *wsp_o++;
           R0 = AWB_smlabt(wB, wC, R0);
           R2 = AWB_smlatb(wA, wD, R2);
           R3 = AWB_smlatt(wA, wD, R3);
           R1 = AWB_smlabb(wB, wD, R1);
           R2 = AWB_smlabt(wB, wD, R2);
           R0 = AWB_smlatb(wB, wD, R0);
           R1 = AWB_smlatt(wB, wD, R1);
           wA = *wsp_o++;
           R0 = AWB_smlabt(wA, wD, R0);
           j = L_frame_2;
           do
           {
              wC = *wsp_s++;
              R3 = AWB_smlabb(wB, wC, R3);
              R2 = AWB_smlatb(wB, wC, R2);
              R3 = AWB_smlatt(wB, wC, R3);
              R1 = AWB_smlabb(wA, wC, R1);
              R2 = AWB_smlabt(wA, wC, R2);
              R0 = AWB_smlatb(wA, wC, R0);
              R1 = AWB_smlatt(wA, wC, R1);
              wB = *wsp_o++;
              R0 = AWB_smlabt(wB, wC, R0);
              wD = *wsp_s++;
              R3 = AWB_smlabb(wA, wD, R3);
              R2 = AWB_smlatb(wA, wD, R2);
              R3 = AWB_smlatt(wA, wD, R3);
              R1 = AWB_smlabb(wB, wD, R1);
              R2 = AWB_smlabt(wB, wD, R2);
              R0 = AWB_smlatb(wB, wD, R0);
              R1 = AWB_smlatt(wB, wD, R1);
              wA = *wsp_o++;
              R0 = AWB_smlabt(wA, wD, R0);
           } while( --j != 0);
           wC = *wsp_s++;
           R3 = AWB_smlabb(wB, wC, R3);
           R2 = AWB_smlatb(wB, wC, R2);
           R3 = AWB_smlatt(wB, wC, R3);
           R1 = AWB_smlabb(wA, wC, R1);
           R2 = AWB_smlabt(wA, wC, R2);
           wD = *wsp_s++;
           R3 = AWB_smlabb(wA, wD, R3);
           if(k == 2) {
              tmpRx[0] = R0;
              tmpRx[1] = R1;
              tmpRx[2] = R2;
              tmpRx[3] = R3;
           }
        } while(--k != 0);

        R0 = AWB_qadd(R0, tmpRx[0]);
        R1 = AWB_qadd(R1, tmpRx[1]);
        R2 = AWB_qadd(R2, tmpRx[2]);
        R3 = AWB_qadd(R3, tmpRx[3]);

        R0 = AWB_smulwb(R0, (fract32)(*ww--));
        R1 = AWB_smulwb(R1, (fract32)(*ww--));
        R2 = AWB_smulwb(R2, (fract32)(*ww--));
        R3 = AWB_smulwb(R3, (fract32)(*ww--));


        if(L_0 == 0) {
           R0 = AWB_smulwb(R0, (fract32)(*we--));
           R1 = AWB_smulwb(R1, (fract32)(*we--));
           R2 = AWB_smulwb(R2, (fract32)(*we--));
           R3 = AWB_smulwb(R3, (fract32)(*we--));
        }
        if (Sub_fr1x32(R0, max) >= 0) {
          max = R0;
          Tm =(fract16) i;
        }
        if (Sub_fr1x32(R1, max) >= 0) {
          max = R1;
          Tm =(fract16) i - 1;
        }
        if (Sub_fr1x32(R2, max) >= 0) {
          max = R2;
          Tm =(fract16) i - 2;
        }
        if (Sub_fr1x32(R3, max) >= 0) {
          max = R3;
          Tm =(fract16) i - 3;
        }
        R0 = (5 + (L_frame_2 << 2));
        wsp_o -= R0;
        wsp_s -= (R0 - 1);
     } while( (i -= 4) != 0);
     Tm += 19;

     R0 = R1 = 0;
     wC = *wsp_s++;
     wA = *wsp_o++;
     k = 2;
     do
     {
        R0 = AWB_smlatb(wA, wC, R0);
        R1 = AWB_smlatt(wA, wC, R1);
     wB = *wsp_o++;
        R0 = AWB_smlabt(wB, wC, R0);
     wD = *wsp_s++;
        R1 = AWB_smlabb(wB, wD, R1);
        R0 = AWB_smlatb(wB, wD, R0);
        R1 = AWB_smlatt(wB, wD, R1);
     wA = *wsp_o++;
        R0 = AWB_smlabt(wA, wD, R0);
     j = L_frame_2;
     do
     {
        wC = *wsp_s++;
           R1 = AWB_smlabb(wA, wC, R1);
           R0 = AWB_smlatb(wA, wC, R0);
           R1 = AWB_smlatt(wA, wC, R1);
        wB = *wsp_o++;
           R0 = AWB_smlabt(wB, wC, R0);
        wD = *wsp_s++;
           R1 = AWB_smlabb(wB, wD, R1);
           R0 = AWB_smlatb(wB, wD, R0);
           R1 = AWB_smlatt(wB, wD, R1);
        wA = *wsp_o++;
           R0 = AWB_smlabt(wA, wD, R0);
     } while( --j != 0);
     wC = *wsp_s++;
        R1 = AWB_smlabb(wA, wC, R1);
        if(k == 2) {
           tmpRx[0] = R0;
           tmpRx[1] = R1;
        }
     } while(--k != 0);
     R0 = AWB_qadd(R0, tmpRx[0]);
     R1 = AWB_qadd(R1, tmpRx[1]);

     R0 = AWB_smulwb(R0, (fract32)(*ww--));
     R1 = AWB_smulwb(R1, (fract32)(*ww--));

     if(L_0 == 0) {
        R0 = AWB_smulwb(R0, (fract32)(*we--));
        R1 = AWB_smulwb(R1, (fract32)(*we--));

     }
     if (Sub_fr1x32(R0, max) >= 0) {
       max = R0;
       Tm =(fract16) 19;
     }
     if (Sub_fr1x32(R1, max) >= 0) {
       max = R1;
       Tm =(fract16) 18;
     }
  }

#elif (AWB_OPT_VER == 1)
   fract32 R3, L_frame_2, wA, wB, wC, wD;
   fract32 *wsp_s, *wsp_o;

   ww = &corrweight[198];
   we = &corrweight[98 + L_max - L_0];
   max = MIN_32;
   Tm = 0;

   if(_awb_check_word_align(wsp) == 0)
   {
      for (i = L_max; i > L_min; i--)
      {
         /* Compute the correlation */
         R0 = 0;

         for (j = 0; j < L_frame; j++)
            R0 = Mac_fr16_32(R0, wsp[j], wsp[j - i]);

         /* Weighting of the correlation function.   */
         WBAMR_L_Extract(R0, &hi, &lo);
         R0 = WBAMR_Mpy_32_16(hi, lo, *ww);
         ww--;

         if ((L_0 > 0) && (wght_flg > 0))
         {
            /* Weight the neighbourhood of the old lag. */
            WBAMR_L_Extract(R0, &hi, &lo);
            R0 = WBAMR_Mpy_32_16(hi, lo, *we);
            we--;
         }

         if (Sub_fr1x32(R0, max) >= 0)
         {
            max = R0;
            Tm =(fract16) i;
         }
      }
   }
   else
   {
      // not word_aligned
      L_0 = ((L_0 > 0) && (wght_flg > 0)) ? 0 : 1;
      L_frame_2 = (L_frame >> 2) - 1;
      wsp_s = (fract32 *)(wsp - 115);
      wsp_o = (fract32 *)(wsp - 1);
      i = 96;
      do
      {
         R0 = R1 = R2 = R3 = 0;
         wC = *wsp_s++;
         wA = *wsp_o++;
         R0 = AWB_qdadd(R0, AWB_smultb(wA, wC));
         R1 = AWB_qdadd(R1, AWB_smultt(wA, wC));
         wB = *wsp_o++;
         R0 = AWB_qdadd(R0, AWB_smulbt(wB, wC));
         wD = *wsp_s++;
         R2 = AWB_qdadd(R2, AWB_smultb(wA, wD));
         R3 = AWB_qdadd(R3, AWB_smultt(wA, wD));
         R1 = AWB_qdadd(R1, AWB_smulbb(wB, wD));
         R2 = AWB_qdadd(R2, AWB_smulbt(wB, wD));
         R0 = AWB_qdadd(R0, AWB_smultb(wB, wD));
         R1 = AWB_qdadd(R1, AWB_smultt(wB, wD));
         wA = *wsp_o++;
         R0 = AWB_qdadd(R0, AWB_smulbt(wA, wD));
         j = L_frame_2;
         do
         {
            wC = *wsp_s++;
            R3 = AWB_qdadd(R3, AWB_smulbb(wB, wC));
            R2 = AWB_qdadd(R2, AWB_smultb(wB, wC));
            R3 = AWB_qdadd(R3, AWB_smultt(wB, wC));
            R1 = AWB_qdadd(R1, AWB_smulbb(wA, wC));
            R2 = AWB_qdadd(R2, AWB_smulbt(wA, wC));
            R0 = AWB_qdadd(R0, AWB_smultb(wA, wC));
            R1 = AWB_qdadd(R1, AWB_smultt(wA, wC));
            wB = *wsp_o++;
            R0 = AWB_qdadd(R0, AWB_smulbt(wB, wC));
            wD = *wsp_s++;
            R3 = AWB_qdadd(R3, AWB_smulbb(wA, wD));
            R2 = AWB_qdadd(R2, AWB_smultb(wA, wD));
            R3 = AWB_qdadd(R3, AWB_smultt(wA, wD));
            R1 = AWB_qdadd(R1, AWB_smulbb(wB, wD));
            R2 = AWB_qdadd(R2, AWB_smulbt(wB, wD));
            R0 = AWB_qdadd(R0, AWB_smultb(wB, wD));
            R1 = AWB_qdadd(R1, AWB_smultt(wB, wD));
            wA = *wsp_o++;
            R0 = AWB_qdadd(R0, AWB_smulbt(wA, wD));
         } while( --j != 0);
         wC = *wsp_s++;
         R3 = AWB_qdadd(R3, AWB_smulbb(wB, wC));
         R2 = AWB_qdadd(R2, AWB_smultb(wB, wC));
         R3 = AWB_qdadd(R3, AWB_smultt(wB, wC));
         R1 = AWB_qdadd(R1, AWB_smulbb(wA, wC));
         R2 = AWB_qdadd(R2, AWB_smulbt(wA, wC));
         wD = *wsp_s++;
         R3 = AWB_qdadd(R3, AWB_smulbb(wA, wD));

         WBAMR_L_Extract(R0, &hi, &lo);
         R0 = WBAMR_Mpy_32_16(hi, lo, *ww--);

         WBAMR_L_Extract(R1, &hi, &lo);
         R1 = WBAMR_Mpy_32_16(hi, lo, *ww--);

         WBAMR_L_Extract(R2, &hi, &lo);
         R2 = WBAMR_Mpy_32_16(hi, lo, *ww--);

         WBAMR_L_Extract(R3, &hi, &lo);
         R3 = WBAMR_Mpy_32_16(hi, lo, *ww--);

         if(L_0 == 0) {
            WBAMR_L_Extract(R0, &hi, &lo);
            R0 = WBAMR_Mpy_32_16(hi, lo, *we--);

            WBAMR_L_Extract(R1, &hi, &lo);
            R1 = WBAMR_Mpy_32_16(hi, lo, *we--);

            WBAMR_L_Extract(R2, &hi, &lo);
            R2 = WBAMR_Mpy_32_16(hi, lo, *we--);

            WBAMR_L_Extract(R3, &hi, &lo);
            R3 = WBAMR_Mpy_32_16(hi, lo, *we--);
         }
         if (Sub_fr1x32(R0, max) >= 0) {
            max = R0;
            Tm =(fract16) i;
         }
         if (Sub_fr1x32(R1, max) >= 0) {
            max = R1;
            Tm =(fract16) i - 1;
         }
         if (Sub_fr1x32(R2, max) >= 0) {
            max = R2;
            Tm =(fract16) i - 2;
         }
         if (Sub_fr1x32(R3, max) >= 0) {
            max = R3;
            Tm =(fract16) i - 3;
         }
         wsp_o -= (L_frame >> 1) + 1; // 33
         wsp_s -= (L_frame >> 1); // 32
      } while( (i -= 4) != 0);
      Tm += 19;

      R0 = R1 = 0;
      wC = *wsp_s++;
      wA = *wsp_o++;
      R0 = AWB_qdadd(R0, AWB_smultb(wA, wC));
      R1 = AWB_qdadd(R1, AWB_smultt(wA, wC));
      wB = *wsp_o++;
      R0 = AWB_qdadd(R0, AWB_smulbt(wB, wC));
      wD = *wsp_s++;
      R1 = AWB_qdadd(R1, AWB_smulbb(wB, wD));
      R0 = AWB_qdadd(R0, AWB_smultb(wB, wD));
      R1 = AWB_qdadd(R1, AWB_smultt(wB, wD));
      wA = *wsp_o++;
      R0 = AWB_qdadd(R0, AWB_smulbt(wA, wD));
      j = L_frame_2;
      do
      {
         wC = *wsp_s++;
         R1 = AWB_qdadd(R1, AWB_smulbb(wA, wC));
         R0 = AWB_qdadd(R0, AWB_smultb(wA, wC));
         R1 = AWB_qdadd(R1, AWB_smultt(wA, wC));
         wB = *wsp_o++;
         R0 = AWB_qdadd(R0, AWB_smulbt(wB, wC));
         wD = *wsp_s++;
         R1 = AWB_qdadd(R1, AWB_smulbb(wB, wD));
         R0 = AWB_qdadd(R0, AWB_smultb(wB, wD));
         R1 = AWB_qdadd(R1, AWB_smultt(wB, wD));
         wA = *wsp_o++;
         R0 = AWB_qdadd(R0, AWB_smulbt(wA, wD));
      } while( --j != 0);
      wC = *wsp_s++;
      R1 = AWB_qdadd(R1, AWB_smulbb(wA, wC));

      WBAMR_L_Extract(R0, &hi, &lo);
      R0 = WBAMR_Mpy_32_16(hi, lo, *ww--);

      WBAMR_L_Extract(R1, &hi, &lo);
      R1 = WBAMR_Mpy_32_16(hi, lo, *ww--);


      if(L_0 == 0) {
         WBAMR_L_Extract(R0, &hi, &lo);
         R0 = WBAMR_Mpy_32_16(hi, lo, *we--);

         WBAMR_L_Extract(R1, &hi, &lo);
         R1 = WBAMR_Mpy_32_16(hi, lo, *we--);

      }
      if (Sub_fr1x32(R0, max) >= 0) {
         max = R0;
         Tm =(fract16) 19;
      }
      if (Sub_fr1x32(R1, max) >= 0) {
         max = R1;
         Tm =(fract16) 18;
      }
   }

#else

  ww = &corrweight[198];
  we = &corrweight[98 + L_max - L_0];
  max = MIN_32;
  Tm = 0;

  for (i = L_max; i > L_min; i--)
  {
    /* Compute the correlation */
    R0 = 0;

    for (j = 0; j < L_frame; j++)
      R0 = Mac_fr16_32(R0, wsp[j], wsp[j - i]);

    /* Weighting of the correlation function.   */
    WBAMR_L_Extract(R0, &hi, &lo);
    R0 = WBAMR_Mpy_32_16(hi, lo, *ww);
    ww--;

    if ((L_0 > 0) && (wght_flg > 0))
    {
      /* Weight the neighbourhood of the old lag. */
      WBAMR_L_Extract(R0, &hi, &lo);
      R0 = WBAMR_Mpy_32_16(hi, lo, *we);
      we--;
    }

    if (Sub_fr1x32(R0, max) >= 0)
    {
      max = R0;
      Tm =(fract16) i;
    }
  }
#endif

  /* Hypass the wsp[] vector */
  hp_wsp = old_hp_wsp + L_max;
  WBAMR_Hp_wsp(wsp, hp_wsp, L_frame, hp_wsp_mem);

  /* Compute normalize correlation at delay Tm */
  R0 = 0;
  R1 = 1L;
  R2 = 1L;

  for (j = 0; j < L_frame; j++)
  {
    R0 = Mac_fr16_32(R0, hp_wsp[j], hp_wsp[j - Tm]);
    R1 = Mac_fr16_32(R1, hp_wsp[j - Tm], hp_wsp[j - Tm]);
    R2 = Mac_fr16_32(R2, hp_wsp[j], hp_wsp[j]);
  }

  /* gain = R0/ sqrt(R1*R2) */
  exp_R0 = Norm_fr1x32(R0);
  R0 = Shl_fr1x32(R0, exp_R0);
  exp_R1 = Norm_fr1x32(R1);
  R1 = Shl_fr1x32(R1, exp_R1);
  exp_R2 = Norm_fr1x32(R2);
  R2 = Shl_fr1x32(R2, exp_R2);
  R1 = Mult_fr1x32(Round_fr1x32(R1), Round_fr1x32(R2));
  i = Norm_fr1x32(R1);
  R1 = Shl_fr1x32(R1, i);
  exp_R1 = Add_fr1x16(exp_R1, exp_R2);
  exp_R1 = Add_fr1x16(exp_R1, (fract16)i);
  exp_R1 = Sub_fr1x16(62, exp_R1);
  WBAMR_Isqrt_n(&R1, &exp_R1);
  R0 = Mult_fr1x32(Round_fr1x32(R0), Round_fr1x32(R1));
  exp_R0 = Sub_fr1x16(31, exp_R0);
  exp_R0 = Add_fr1x16(exp_R0, exp_R1);
  *gain = Round_fr1x32(Shl_fr1x32(R0, exp_R0));

  /* Shitf hp_wsp[] for next frame */
  for (i = 0; i < L_max; i++)
  {
    old_hp_wsp[i] = old_hp_wsp[i + L_frame];
  }

  return (Tm);

}



/*____________________________________________________________________
 |
 |
 |  FUNCTION NAME WBAMR_Median5
 |
 |      Returns the median of the set {X[-2], X[-1],..., X[2]},
 |      whose elements are 16-bit integers.
 |
 |  INPUT
 |      X[-2:2]   16-bit integers.
 |
 |  RETURN VALUE
 |      The median of {X[-2], X[-1],..., X[2]}.
 |_____________________________________________________________________
 */


WBAMR_CODE_SECTION fract16 WBAMR_Median5(fract16 x[])
{
  fract16 x1, x2, x3, x4, x5;
  fract16 tmp;

  x1 = x[-2];
  x2 = x[-1];
  x3 = x[0];
  x4 = x[1];
  x5 = x[2];

  if (Sub_fr1x16(x2, x1) < 0)
  {
    tmp = x1;
    x1 = x2;
    x2 = tmp;
  }

  if (Sub_fr1x16(x3, x1) < 0)
  {
    tmp = x1;
    x1 = x3;
    x3 = tmp;
  }

  if (Sub_fr1x16(x4, x1) < 0)
  {
    tmp = x1;
    x1 = x4;
    x4 = tmp;
  }

  if (Sub_fr1x16(x5, x1) < 0)
  {
    x5 = x1;
  }

  if (Sub_fr1x16(x3, x2) < 0)
  {
    tmp = x2;
    x2 = x3;
    x3 = tmp;
  }

  if (Sub_fr1x16(x4, x2) < 0)
  {
    tmp = x2;
    x2 = x4;
    x4 = tmp;
  }

  if (Sub_fr1x16(x5, x2) < 0)
  {
    x5 = x2;
  }

  if (Sub_fr1x16(x4, x3) < 0)
  {
    x3 = x4;
  }

  if (Sub_fr1x16(x5, x3) < 0)
  {
    x3 = x5;
  }

  return (x3);

}



/*____________________________________________________________________
 |
 |
 |  FUNCTION NAME WBAMR_Med_olag
 |
 |
 |_____________________________________________________________________
 */


WBAMR_CODE_SECTION fract16 WBAMR_Med_olag(
			  /* output : median of  5 previous open-loop lags */
  fract16 prev_ol_lag,    /* input  : previous open-loop lag */
  fract16 old_ol_lag[5]
)
{

  fract16 i;

  /* Use median of 5 previous open-loop lags as old lag */
  for (i = 4; i > 0; i--)
  {
    old_ol_lag[i] = old_ol_lag[i - 1];
  }

  old_ol_lag[0] = prev_ol_lag;

  i = WBAMR_Median5(&old_ol_lag[2]);

  return i;

}

#endif

